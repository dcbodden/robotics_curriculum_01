// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "control_config.h"
#include "control_mapping.h"

namespace control {

constexpr int kNoController = -1;
enum class ControlState {
    Disconnected,
    Disarmed,
    Armed,
    Failsafe,
};

enum class TransitionReason {
    Reset,
    ControllerSelected,
    NeutralObserved,
    Armed,
    Disarmed,
    ControllerDisconnected,
    InvalidController,
    WatchdogExpired,
};

struct ControllerReport {
    int controllerIndex;
    uint32_t receivedAtMs;
    int axisX;
    int axisY;
    bool validGamepad;
    bool optionsPressed;
    bool circlePressed;
};

struct ControlOutput {
    int targetMotor;
    int appliedMotor;
    uint16_t servoPulseUs;
    bool reversalInterlock;
};

class ControlPolicy {
public:
    ControlPolicy() = default;

    bool controllerConnected(int controllerIndex, bool validGamepad) {
        if (controllerIndex < 0 || !validGamepad || activeController_ != kNoController) return false;

        activeController_ = controllerIndex;
        state_ = ControlState::Disarmed;
        resetControlSession();
        lastTransition_ = TransitionReason::ControllerSelected;
        return true;
    }

    void controllerDisconnected(int controllerIndex) {
        if (controllerIndex != activeController_) return;

        activeController_ = kNoController;
        enterSafetyState(ControlState::Disconnected, TransitionReason::ControllerDisconnected);
    }

    bool submitReport(const ControllerReport& report) {
        if (report.controllerIndex != activeController_) return false;

        if (!report.validGamepad) {
            enterSafetyState(ControlState::Failsafe, TransitionReason::InvalidController);
            return true;
        }

        const bool optionsEdge = report.optionsPressed && !previousOptionsPressed_;
        const bool circleEdge = report.circlePressed && !previousCirclePressed_;
        previousOptionsPressed_ = report.optionsPressed;
        previousCirclePressed_ = report.circlePressed;
        hasFreshReport_ = true;
        lastFreshReportAtMs_ = report.receivedAtMs;

        if (report.circlePressed) {
            if (circleEdge) {
                enterSafetyState(ControlState::Disarmed, TransitionReason::Disarmed);
                // enterSafetyState() clears the session, so restore the current
                // button levels to keep a held CIRCLE from becoming a new edge.
                previousOptionsPressed_ = report.optionsPressed;
                previousCirclePressed_ = true;
            }
            return true;
        }

        const bool neutralNow = leftStickIsNeutral(report.axisX, report.axisY);
        const MappedCommands mapped = mapLeftStick(report.axisX, report.axisY);

        if (state_ == ControlState::Armed) {
            targetMotor_ = mapped.motor;
            servoPulseUs_ = mapped.servoPulseUs;
            beginReversalInterlockIfNeeded(report.receivedAtMs);
            return true;
        }

        const bool wasNeutralReady = neutralReady_;
        if (neutralNow && !neutralReady_) {
            neutralReady_ = true;
            lastTransition_ = TransitionReason::NeutralObserved;
        }

        // The neutral report must precede the arm edge; an OPTIONS edge on the
        // first neutral report cannot arm the controller.
        if (optionsEdge && wasNeutralReady && neutralNow) {
            state_ = ControlState::Armed;
            targetMotor_ = 0;
            appliedMotor_ = 0;
            servoPulseUs_ = kServoCenterPulseUs;
            reversalInterlock_ = false;
            updateClockInitialized_ = true;
            lastControlUpdateAtMs_ = report.receivedAtMs;
            lastTransition_ = TransitionReason::Armed;
        }
        return true;
    }

    void update(uint32_t nowMs) {
        if (state_ == ControlState::Armed &&
            (!hasFreshReport_ || elapsed(nowMs, lastFreshReportAtMs_) >= kFreshReportWatchdogMs)) {
            enterSafetyState(ControlState::Failsafe, TransitionReason::WatchdogExpired);
            return;
        }

        if (state_ != ControlState::Armed) return;

        if (!updateClockInitialized_) {
            updateClockInitialized_ = true;
            lastControlUpdateAtMs_ = nowMs;
            return;
        }
        if (elapsed(nowMs, lastControlUpdateAtMs_) < kControlUpdateIntervalMs) return;

        lastControlUpdateAtMs_ = nowMs;
        if (reversalInterlock_) {
            // Preserve a complete scheduled update at zero before allowing the
            // opposite direction to begin ramping on a later update.
            reversalInterlock_ = false;
            return;
        }

        appliedMotor_ = moveToward(appliedMotor_, targetMotor_, kMotorSlewPerUpdate);
    }

    int activeController() const { return activeController_; }
    ControlState state() const { return state_; }
    TransitionReason lastTransition() const { return lastTransition_; }
    bool neutralReady() const { return neutralReady_; }
    bool armed() const { return state_ == ControlState::Armed; }
    bool hasFreshReport() const { return hasFreshReport_; }
    uint32_t lastFreshReportAtMs() const { return lastFreshReportAtMs_; }
    uint32_t freshnessAgeMs(uint32_t nowMs) const {
        return hasFreshReport_ ? elapsed(nowMs, lastFreshReportAtMs_) : UINT32_MAX;
    }
    ControlOutput output() const {
        return {targetMotor_, appliedMotor_, servoPulseUs_, reversalInterlock_};
    }

private:
    static uint32_t elapsed(uint32_t nowMs, uint32_t thenMs) { return nowMs - thenMs; }

    static int signOf(int value) {
        if (value > 0) return 1;
        if (value < 0) return -1;
        return 0;
    }

    static int moveToward(int current, int target, int maximumStep) {
        if (current < target) {
            const int next = current + maximumStep;
            return next > target ? target : next;
        }
        if (current > target) {
            const int next = current - maximumStep;
            return next < target ? target : next;
        }
        return current;
    }

    void resetControlSession() {
        neutralReady_ = false;
        hasFreshReport_ = false;
        lastFreshReportAtMs_ = 0;
        previousOptionsPressed_ = false;
        previousCirclePressed_ = false;
        targetMotor_ = 0;
        appliedMotor_ = 0;
        servoPulseUs_ = kServoCenterPulseUs;
        reversalInterlock_ = false;
        updateClockInitialized_ = false;
        lastControlUpdateAtMs_ = 0;
    }

    void enterSafetyState(ControlState state, TransitionReason reason) {
        state_ = state;
        resetControlSession();
        lastTransition_ = reason;
    }

    void beginReversalInterlockIfNeeded(uint32_t nowMs) {
        if (signOf(appliedMotor_) == 0 || signOf(targetMotor_) == 0 ||
            signOf(appliedMotor_) == signOf(targetMotor_)) {
            return;
        }

        appliedMotor_ = 0;
        reversalInterlock_ = true;
        updateClockInitialized_ = true;
        lastControlUpdateAtMs_ = nowMs;
    }

    int activeController_ = kNoController;
    ControlState state_ = ControlState::Disconnected;
    TransitionReason lastTransition_ = TransitionReason::Reset;
    bool neutralReady_ = false;
    bool hasFreshReport_ = false;
    uint32_t lastFreshReportAtMs_ = 0;
    bool previousOptionsPressed_ = false;
    bool previousCirclePressed_ = false;
    int targetMotor_ = 0;
    int appliedMotor_ = 0;
    uint16_t servoPulseUs_ = kServoCenterPulseUs;
    bool reversalInterlock_ = false;
    bool updateClockInitialized_ = false;
    uint32_t lastControlUpdateAtMs_ = 0;
};

}  // namespace control
