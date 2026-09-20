// SPDX-License-Identifier: Apache-2.0

#include <Arduino.h>
#include <Bluepad32.h>

namespace {
constexpr unsigned long kSerialSpeed = 115200;
}

void setup() {
    Serial.begin(kSerialSpeed);
    Serial.println("ESP32 Bluetooth controller baseline foundation ready");
    Serial.printf("Bluepad32 firmware: %s\n", BP32.firmwareVersion());
}

void loop() {
    delay(1000);
}
