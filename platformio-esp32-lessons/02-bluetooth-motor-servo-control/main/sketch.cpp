// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>
#include <Bluepad32.h>

#include <cstdio>
#include <cstring>

#include "actuator_output.h"
#include "btdiag.h"
#include "control_policy.h"

namespace {
constexpr unsigned long kSerialSpeed = 115200;
constexpr unsigned long kConnectionTimeoutMs = 75000;
constexpr unsigned long kWaitingIntervalMs = 5000;
constexpr unsigned long kInputObservationIntervalMs = 100;
constexpr unsigned long kBondClearConfirmationTimeoutMs = 10000;
constexpr char kBondClearRequestCommand[] = "CLEAR_BONDS";
constexpr char kBondClearConfirmCommand[] = "CONFIRM_CLEAR_BONDS";
constexpr char kBondClearCancelCommand[] = "CANCEL_CLEAR_BONDS";
constexpr size_t kSerialCommandBufferSize = 32;
constexpr size_t kMaximumSerialBytesPerLoop = 32;

struct GamepadSnapshot {
    uint16_t buttons;
    uint16_t miscButtons;
    uint8_t dpad;
    int32_t axisX;
    int32_t axisY;
    int32_t axisRX;
    int32_t axisRY;
    int32_t brake;
    int32_t throttle;
};

struct InputTracker {
    bool firstInputSeen = false;
    bool observationValid = false;
    unsigned long lastObservationAt = 0;
    GamepadSnapshot lastObservation{};
};

unsigned long connectionWindowStartedAt = 0;
unsigned long lastWaitingReportAt = 0;
unsigned int connectedControllerCount = 0;
bool connectionWindowTimedOut = false;
ControllerPtr controllers[BP32_MAX_CONTROLLERS]{};
InputTracker inputTrackers[BP32_MAX_CONTROLLERS]{};
bool bondClearConfirmationPending = false;
unsigned long bondClearRequestedAt = 0;
char serialCommandBuffer[kSerialCommandBufferSize]{};
size_t serialCommandLength = 0;
bool serialCommandOverflowed = false;
control::ControlPolicy controlPolicy;
control::ActuatorOutput actuatorOutput;
bool actuatorReady = false;
bool hardwareOutputValid = false;
control::ControlOutput lastHardwareOutput{};
int latestActiveAxisX = 0;
int latestActiveAxisY = 0;

bool validControllerIndex(int index) {
    return index >= 0 && index < BP32_MAX_CONTROLLERS;
}

void appendName(char* buffer, size_t bufferSize, const char* name) {
    const size_t used = strlen(buffer);
    if (used >= bufferSize - 1) {
        return;
    }

    snprintf(buffer + used, bufferSize - used, "%s%s", used == 0 ? "" : ",", name);
}

void describeButtons(ControllerPtr controller, char* buffer, size_t bufferSize) {
    buffer[0] = '\0';
    if (controller->a()) appendName(buffer, bufferSize, "cross");
    if (controller->b()) appendName(buffer, bufferSize, "circle");
    if (controller->x()) appendName(buffer, bufferSize, "square");
    if (controller->y()) appendName(buffer, bufferSize, "triangle");
    if (controller->l1()) appendName(buffer, bufferSize, "l1");
    if (controller->l2()) appendName(buffer, bufferSize, "l2");
    if (controller->r1()) appendName(buffer, bufferSize, "r1");
    if (controller->r2()) appendName(buffer, bufferSize, "r2");
    if (controller->thumbL()) appendName(buffer, bufferSize, "left_stick");
    if (controller->thumbR()) appendName(buffer, bufferSize, "right_stick");
    if (controller->miscSelect()) appendName(buffer, bufferSize, "share");
    if (controller->miscStart()) appendName(buffer, bufferSize, "options");
    if (controller->miscSystem()) appendName(buffer, bufferSize, "home");
    if (controller->miscCapture()) appendName(buffer, bufferSize, "capture");
    if (buffer[0] == '\0') appendName(buffer, bufferSize, "none");
}

void describeDpad(uint8_t dpad, char* buffer, size_t bufferSize) {
    buffer[0] = '\0';
    if (dpad & DPAD_UP) appendName(buffer, bufferSize, "up");
    if (dpad & DPAD_DOWN) appendName(buffer, bufferSize, "down");
    if (dpad & DPAD_LEFT) appendName(buffer, bufferSize, "left");
    if (dpad & DPAD_RIGHT) appendName(buffer, bufferSize, "right");
    if (buffer[0] == '\0') appendName(buffer, bufferSize, "none");
}

GamepadSnapshot readGamepad(ControllerPtr controller) {
    return {controller->buttons(), controller->miscButtons(), controller->dpad(), controller->axisX(),
            controller->axisY(), controller->axisRX(), controller->axisRY(), controller->brake(),
            controller->throttle()};
}

bool snapshotsDiffer(const GamepadSnapshot& left, const GamepadSnapshot& right) {
    return left.buttons != right.buttons || left.miscButtons != right.miscButtons || left.dpad != right.dpad ||
           left.axisX != right.axisX || left.axisY != right.axisY || left.axisRX != right.axisRX ||
           left.axisRY != right.axisRY || left.brake != right.brake || left.throttle != right.throttle;
}

bool outputsEqual(const control::ControlOutput& left, const control::ControlOutput& right) {
    return left.targetMotor == right.targetMotor && left.appliedMotor == right.appliedMotor &&
           left.servoPulseUs == right.servoPulseUs && left.reversalInterlock == right.reversalInterlock;
}

void applyControlOutput() {
    if (!actuatorReady) return;

    const control::ControlOutput output = controlPolicy.output();
    if (hardwareOutputValid && outputsEqual(lastHardwareOutput, output)) return;

    if (!actuatorOutput.apply(output)) {
        actuatorOutput.safeStop();
        actuatorReady = false;
        hardwareOutputValid = false;
        return;
    }

    lastHardwareOutput = output;
    hardwareOutputValid = true;
}

void emitInput(ControllerPtr controller, const GamepadSnapshot& snapshot, bool firstInput) {
    char pressedButtons[96];
    char pressedDirections[24];
    describeButtons(controller, pressedButtons, sizeof(pressedButtons));
    describeDpad(snapshot.dpad, pressedDirections, sizeof(pressedDirections));

    if (firstInput) {
        btdiag::emitControllerFirstInput(controller->index(), snapshot.buttons, snapshot.miscButtons, snapshot.dpad,
                                         pressedButtons, pressedDirections, snapshot.axisX, snapshot.axisY,
                                         snapshot.axisRX, snapshot.axisRY, snapshot.brake, snapshot.throttle);
    } else {
        btdiag::emitControllerInput(controller->index(), snapshot.buttons, snapshot.miscButtons, snapshot.dpad,
                                    pressedButtons, pressedDirections, snapshot.axisX, snapshot.axisY,
                                    snapshot.axisRX, snapshot.axisRY, snapshot.brake, snapshot.throttle);
    }
}

void processControllerReports() {
    const unsigned long now = millis();

    for (int index = 0; index < BP32_MAX_CONTROLLERS; ++index) {
        ControllerPtr controller = controllers[index];
        if (controller == nullptr || !controller->isConnected() || !controller->hasData()) {
            continue;
        }

        if (!controller->isGamepad()) {
            controlPolicy.submitReport({index, now, 0, 0, false, false, false});
            applyControlOutput();
            continue;
        }

        InputTracker& tracker = inputTrackers[index];
        const GamepadSnapshot snapshot = readGamepad(controller);
        const bool reportAccepted = controlPolicy.submitReport(
            {index, now, static_cast<int>(snapshot.axisX), static_cast<int>(snapshot.axisY), true,
             controller->miscStart(), controller->b()});
        if (reportAccepted) {
            latestActiveAxisX = static_cast<int>(snapshot.axisX);
            latestActiveAxisY = static_cast<int>(snapshot.axisY);
            applyControlOutput();
        }

        if (!tracker.firstInputSeen) {
            emitInput(controller, snapshot, true);
            tracker.firstInputSeen = true;
            tracker.observationValid = true;
            tracker.lastObservationAt = now;
            tracker.lastObservation = snapshot;
            continue;
        }

        if (tracker.observationValid && !snapshotsDiffer(tracker.lastObservation, snapshot)) {
            continue;
        }

        if (now - tracker.lastObservationAt < kInputObservationIntervalMs) {
            continue;
        }

        emitInput(controller, snapshot, false);
        tracker.observationValid = true;
        tracker.lastObservationAt = now;
        tracker.lastObservation = snapshot;
    }
}

void expireBondClearRequest() {
    if (!bondClearConfirmationPending) {
        return;
    }

    const unsigned long waitedMs = millis() - bondClearRequestedAt;
    if (waitedMs < kBondClearConfirmationTimeoutMs) {
        return;
    }

    bondClearConfirmationPending = false;
    btdiag::emitBondClearExpired(waitedMs, kBondClearConfirmationTimeoutMs);
}

void handleSerialCommand(const char* command) {
    if (strcmp(command, kBondClearRequestCommand) == 0) {
        bondClearRequestedAt = millis();
        bondClearConfirmationPending = true;
        btdiag::emitBondClearConfirmationRequired(kBondClearConfirmationTimeoutMs);
        return;
    }

    if (strcmp(command, kBondClearConfirmCommand) == 0) {
        if (!bondClearConfirmationPending) {
            btdiag::emitBondClearConfirmationRejected("no_pending_request");
            return;
        }

        const unsigned long waitedMs = millis() - bondClearRequestedAt;
        if (waitedMs >= kBondClearConfirmationTimeoutMs) {
            bondClearConfirmationPending = false;
            btdiag::emitBondClearExpired(waitedMs, kBondClearConfirmationTimeoutMs);
            return;
        }

        bondClearConfirmationPending = false;
        BP32.forgetBluetoothKeys();
        btdiag::emitBondClearCompleted();
        return;
    }

    if (strcmp(command, kBondClearCancelCommand) == 0) {
        const bool hadPendingRequest = bondClearConfirmationPending;
        bondClearConfirmationPending = false;
        btdiag::emitBondClearCancelled(hadPendingRequest);
    }
}

void processSerialCommands() {
    expireBondClearRequest();

    size_t processed = 0;
    while (processed < kMaximumSerialBytesPerLoop && Serial.available() > 0) {
        const char value = static_cast<char>(Serial.read());
        ++processed;

        if (value == '\r') {
            continue;
        }

        if (value != '\n') {
            if (serialCommandLength < sizeof(serialCommandBuffer) - 1) {
                serialCommandBuffer[serialCommandLength++] = value;
            } else {
                serialCommandOverflowed = true;
            }
            continue;
        }

        serialCommandBuffer[serialCommandLength] = '\0';
        if (!serialCommandOverflowed && serialCommandLength > 0) {
            handleSerialCommand(serialCommandBuffer);
        }

        serialCommandLength = 0;
        serialCommandOverflowed = false;
        expireBondClearRequest();
    }
}

void emitControllerIdentity(ControllerPtr controller, bool connected) {
    const ControllerProperties properties = controller->getProperties();
    const String modelName = controller->getModelName();

    if (connected) {
        btdiag::emitControllerConnected(controller->index(), modelName.c_str(), properties.type, properties.subtype,
                                        properties.vendor_id, properties.product_id, properties.btaddr);
    } else {
        btdiag::emitControllerDisconnected(controller->index(), modelName.c_str(), properties.type, properties.subtype,
                                           properties.vendor_id, properties.product_id, properties.btaddr);
    }
}

void onControllerConnected(ControllerPtr controller) {
    const int index = controller->index();
    if (validControllerIndex(index)) {
        if (controllers[index] == nullptr) {
            ++connectedControllerCount;
        }
        controllers[index] = controller;
        inputTrackers[index] = {};
    }
    emitControllerIdentity(controller, true);
    controlPolicy.controllerConnected(index, controller->isGamepad());
    applyControlOutput();
}

void onControllerDisconnected(ControllerPtr controller) {
    emitControllerIdentity(controller, false);

    const int index = controller->index();
    const bool activeControllerDisconnected = index == controlPolicy.activeController();
    if (validControllerIndex(index) && controllers[index] != nullptr && connectedControllerCount > 0) {
        --connectedControllerCount;
        controllers[index] = nullptr;
        inputTrackers[index] = {};
    }
    controlPolicy.controllerDisconnected(index);
    if (activeControllerDisconnected) {
        latestActiveAxisX = 0;
        latestActiveAxisY = 0;
    }
    applyControlOutput();
}
}

void setup() {
    actuatorReady = actuatorOutput.begin();
    if (actuatorReady) {
        lastHardwareOutput = controlPolicy.output();
        hardwareOutputValid = true;
    }

    Serial.begin(kSerialSpeed);

    btdiag::emitFirmwareStarted();
    btdiag::emitDependencyIdentity();

    // Bluepad32 launches the Arduino task only after its Bluetooth setup has
    // completed, so reaching setup() is the readiness boundary for this port.
    btdiag::emitBluetoothReady();

    // Do not filter by advertised name, VID, or PID: PS4-compatible clones can
    // identify themselves differently. The controller selects PS4 mode.
    BP32.setup(onControllerConnected, onControllerDisconnected, true);

    connectionWindowStartedAt = millis();
    lastWaitingReportAt = connectionWindowStartedAt;
    btdiag::emitAcceptingConnections(kConnectionTimeoutMs);
}

void loop() {
    // Bluepad32 delivers connect, disconnect, and controller data callbacks
    // from update(), so it must continue running throughout the wait window.
    BP32.update();
    processControllerReports();
    controlPolicy.update(millis());
    applyControlOutput();
    processSerialCommands();

    if (connectedControllerCount == 0 && !connectionWindowTimedOut) {
        const unsigned long now = millis();
        const unsigned long waitedMs = now - connectionWindowStartedAt;

        if (waitedMs >= kConnectionTimeoutMs) {
            BP32.enableNewBluetoothConnections(false);
            connectionWindowTimedOut = true;
            btdiag::emitConnectionTimeout(waitedMs, kConnectionTimeoutMs);
        } else if (now - lastWaitingReportAt >= kWaitingIntervalMs) {
            lastWaitingReportAt = now;
            btdiag::emitWaitingForController(waitedMs, kConnectionTimeoutMs - waitedMs);
        }
    }

}
