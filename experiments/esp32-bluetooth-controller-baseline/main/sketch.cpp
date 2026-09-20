// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>

#include "btdiag.h"

namespace {
constexpr unsigned long kSerialSpeed = 115200;
}

void setup() {
    Serial.begin(kSerialSpeed);

    btdiag::emitFirmwareStarted();
    btdiag::emitDependencyIdentity();

    // Bluepad32 launches the Arduino task only after its Bluetooth setup has
    // completed, so reaching setup() is the readiness boundary for this port.
    btdiag::emitBluetoothReady();
}

void loop() {
    delay(1000);
}
