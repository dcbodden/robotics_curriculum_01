// SPDX-License-Identifier: Apache-2.0

#include "btdiag.h"

#include <Arduino.h>
#include <btstack_version.h>
#include <esp_arduino_version.h>
#include <esp_system.h>
#include <uni_version.h>

namespace {

constexpr char kFirmwareName[] = "esp32-bluetooth-controller-baseline";
constexpr char kFirmwareVersion[] = "0.1.0";
constexpr char kBoard[] = "esp32doit-devkit-v1";
constexpr char kPlatform[] = "pioarduino-54.03.21";
constexpr char kTemplateRevision[] = "d07a9385f46f7215f51fc3eb5e40c5a484cfe102";

unsigned long elapsedMs() {
    return millis();
}

}  // namespace

namespace btdiag {

void emitFirmwareStarted() {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"firmware_started\","
        "\"firmware\":\"%s\",\"firmware_version\":\"%s\"}\n",
        kProtocolVersion, elapsedMs(), kFirmwareName, kFirmwareVersion);
}

void emitDependencyIdentity() {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"dependency_identity\","
        "\"board\":\"%s\",\"platform\":\"%s\",\"template_revision\":\"%s\","
        "\"arduino\":\"%s\",\"esp_idf\":\"%s\",\"bluepad32\":\"%s\",\"btstack\":\"%s\"}\n",
        kProtocolVersion, elapsedMs(), kBoard, kPlatform, kTemplateRevision, ESP_ARDUINO_VERSION_STR,
        esp_get_idf_version(), UNI_VERSION_STRING, BTSTACK_VERSION_STRING);
}

void emitBluetoothReady() {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"bluetooth_ready\","
        "\"transport\":\"BR/EDR\"}\n",
        kProtocolVersion, elapsedMs());
}

void emitAcceptingConnections(unsigned long timeoutMs) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"accepting_connections\","
        "\"transport\":\"BR/EDR\",\"controller_mode\":\"ps4_compatible\",\"timeout_ms\":%lu}\n",
        kProtocolVersion, elapsedMs(), timeoutMs);
}

void emitWaitingForController(unsigned long waitedMs, unsigned long remainingMs) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"waiting_for_controller\","
        "\"waited_ms\":%lu,\"remaining_ms\":%lu}\n",
        kProtocolVersion, elapsedMs(), waitedMs, remainingMs);
}

void emitConnectionTimeout(unsigned long waitedMs, unsigned long timeoutMs) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"connection_timeout\","
        "\"waited_ms\":%lu,\"timeout_ms\":%lu,\"outcome\":\"no_controller_observed\"}\n",
        kProtocolVersion, elapsedMs(), waitedMs, timeoutMs);
}

}  // namespace btdiag
