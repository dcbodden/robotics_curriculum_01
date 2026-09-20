// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>
#include <Bluepad32.h>

#include <cstdio>
#include <cstring>

#include "btdiag.h"

namespace {
constexpr unsigned long kSerialSpeed = 115200;
constexpr unsigned long kConnectionTimeoutMs = 75000;
constexpr unsigned long kWaitingIntervalMs = 5000;
constexpr unsigned long kInputObservationIntervalMs = 100;

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
        if (controller == nullptr || !controller->isConnected() || !controller->isGamepad() ||
            !controller->hasData()) {
            continue;
        }

        InputTracker& tracker = inputTrackers[index];
        const GamepadSnapshot snapshot = readGamepad(controller);

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
}

void onControllerDisconnected(ControllerPtr controller) {
    emitControllerIdentity(controller, false);

    const int index = controller->index();
    if (validControllerIndex(index) && controllers[index] != nullptr && connectedControllerCount > 0) {
        --connectedControllerCount;
        controllers[index] = nullptr;
        inputTrackers[index] = {};
    }
}
}

void setup() {
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

    delay(10);
}
