// SPDX-License-Identifier: Apache-2.0

#include "control_telemetry.h"

#include <Arduino.h>

#include <cstdio>

#include "bridge_command.h"

namespace {

constexpr unsigned int kControlProtocolVersion = 1;

const char* stateName(control::ControlState state) {
    switch (state) {
        case control::ControlState::Disconnected:
            return "disconnected";
        case control::ControlState::Disarmed:
            return "disarmed";
        case control::ControlState::Armed:
            return "armed";
        case control::ControlState::Failsafe:
            return "failsafe";
    }
    return "unknown";
}

const char* transitionName(control::TransitionReason reason) {
    switch (reason) {
        case control::TransitionReason::Reset:
            return "reset";
        case control::TransitionReason::ControllerSelected:
            return "controller_selected";
        case control::TransitionReason::NeutralObserved:
            return "neutral_observed";
        case control::TransitionReason::Armed:
            return "armed";
        case control::TransitionReason::Disarmed:
            return "disarmed";
        case control::TransitionReason::ControllerDisconnected:
            return "controller_disconnected";
        case control::TransitionReason::InvalidController:
            return "invalid_controller";
        case control::TransitionReason::WatchdogExpired:
            return "watchdog_expired";
    }
    return "unknown";
}

}  // namespace

namespace control {

bool ControlTelemetryWriter::enqueue(const ControlTelemetrySnapshot& snapshot, bool significantTransition) {
    // Keep one queue slot available for an asynchronous safety transition.
    if ((!significantTransition && count_ >= kQueueCapacity - 1) || count_ >= kQueueCapacity) return false;

    LineSlot& slot = queue_[(head_ + count_) % kQueueCapacity];
    const int motorCommand = snapshot.output.reversalInterlock ? 0 : snapshot.output.appliedMotor;
    const BridgeCommand bridge = bridgeCommandFromMotor(motorCommand);
    const char* bridgeDirection = bridge.ain1Duty > 0 ? "forward" : bridge.ain2Duty > 0 ? "reverse" : "coast";
    const unsigned int bridgeDuty = bridge.ain1Duty > 0 ? bridge.ain1Duty : bridge.ain2Duty;

    char freshness[16];
    if (snapshot.freshnessAgeMs == UINT32_MAX) {
        snprintf(freshness, sizeof(freshness), "null");
    } else {
        snprintf(freshness, sizeof(freshness), "%lu", static_cast<unsigned long>(snapshot.freshnessAgeMs));
    }

    const int length = snprintf(
        slot.bytes, sizeof(slot.bytes),
        "CONTROL {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"%s\","
        "\"controller_index\":%d,\"state\":\"%s\",\"freshness_age_ms\":%s,"
        "\"axis_x\":%d,\"axis_y\":%d,\"target_motor\":%d,\"applied_motor\":%d,"
        "\"bridge_direction\":\"%s\",\"bridge_duty\":%u,\"servo_pulse_us\":%u,"
        "\"reversal_interlock\":%s,\"transition_reason\":\"%s\",\"actuator_ready\":%s}\n",
        kControlProtocolVersion, static_cast<unsigned long>(snapshot.elapsedMs),
        significantTransition ? "control_transition" : "control_status", snapshot.activeController,
        stateName(snapshot.state), freshness, snapshot.axisX, snapshot.axisY, snapshot.output.targetMotor,
        snapshot.output.appliedMotor, bridgeDirection, bridgeDuty,
        static_cast<unsigned int>(snapshot.output.servoPulseUs),
        snapshot.output.reversalInterlock ? "true" : "false", transitionName(snapshot.transitionReason),
        snapshot.actuatorReady ? "true" : "false");

    if (length < 0 || static_cast<size_t>(length) >= sizeof(slot.bytes)) return false;

    slot.length = static_cast<uint16_t>(length);
    slot.offset = 0;
    ++count_;
    return true;
}

void ControlTelemetryWriter::service() {
    if (count_ == 0) return;

    const int writable = Serial.availableForWrite();
    if (writable <= 0) return;

    LineSlot& slot = queue_[head_];
    const size_t remaining = slot.length - slot.offset;
    if (static_cast<size_t>(writable) < remaining) return;

    // Queue the complete line atomically. Partial writes would allow a
    // synchronous BTDIAG record to split this JSON object between loop passes.
    const size_t written = Serial.write(reinterpret_cast<const uint8_t*>(slot.bytes + slot.offset), remaining);
    slot.offset = static_cast<uint16_t>(slot.offset + written);

    if (slot.offset == slot.length) {
        slot.length = 0;
        slot.offset = 0;
        head_ = static_cast<uint8_t>((head_ + 1) % kQueueCapacity);
        --count_;
    }
}

}  // namespace control
