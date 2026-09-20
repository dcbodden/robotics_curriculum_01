// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>
#include <Bluepad32.h>

#include "btdiag.h"

namespace {
constexpr unsigned long kSerialSpeed = 115200;
constexpr unsigned long kConnectionTimeoutMs = 75000;
constexpr unsigned long kWaitingIntervalMs = 5000;

unsigned long connectionWindowStartedAt = 0;
unsigned long lastWaitingReportAt = 0;
unsigned int connectedControllerCount = 0;
bool connectionWindowTimedOut = false;

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
    ++connectedControllerCount;
    emitControllerIdentity(controller, true);
}

void onControllerDisconnected(ControllerPtr controller) {
    emitControllerIdentity(controller, false);

    if (connectedControllerCount > 0) {
        --connectedControllerCount;
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
