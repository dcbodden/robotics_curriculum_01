// SPDX-License-Identifier: Apache-2.0

#include "btdiag.h"

#include <Arduino.h>
#include <btstack_version.h>
#include <esp_arduino_version.h>
#include <esp_system.h>
#include <uni_version.h>

#include <cstdio>

namespace {

constexpr char kFirmwareName[] = "02-bluetooth-motor-servo-control";
constexpr char kFirmwareVersion[] = "0.2.0";
constexpr char kBoard[] = "esp32doit-devkit-v1";
constexpr char kPlatform[] = "pioarduino-54.03.21";
constexpr char kTemplateRevision[] = "d07a9385f46f7215f51fc3eb5e40c5a484cfe102";
constexpr char kDerivedFrom[] = "esp32-bluetooth-controller-baseline";
constexpr char kBaselineTree[] = "4bf49e1acf66f3b483e9e57247cb9616626efbad";

unsigned long elapsedMs() {
    return millis();
}

const char* optionalId(uint16_t id, char* buffer, size_t bufferSize) {
    if (id == 0) {
        return "null";
    }

    snprintf(buffer, bufferSize, "%u", static_cast<unsigned int>(id));
    return buffer;
}

void emitControllerEvent(const char* event,
                         int index,
                         const char* modelName,
                         uint16_t modelId,
                         uint8_t subtype,
                         uint16_t vendorId,
                         uint16_t productId,
                         const uint8_t address[6]) {
    char vendorIdJson[6];
    char productIdJson[6];

    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"%s\",\"controller_index\":%d,"
        "\"bluetooth_address\":\"%02x:%02x:%02x:%02x:%02x:%02x\",\"model_name\":\"%s\","
        "\"model_id\":%u,\"subtype\":%u,\"vendor_id\":%s,\"product_id\":%s}\n",
        btdiag::kProtocolVersion, elapsedMs(), event, index, static_cast<unsigned int>(address[0]),
        static_cast<unsigned int>(address[1]), static_cast<unsigned int>(address[2]),
        static_cast<unsigned int>(address[3]), static_cast<unsigned int>(address[4]),
        static_cast<unsigned int>(address[5]), modelName, static_cast<unsigned int>(modelId),
        static_cast<unsigned int>(subtype), optionalId(vendorId, vendorIdJson, sizeof(vendorIdJson)),
        optionalId(productId, productIdJson, sizeof(productIdJson)));
}

void emitControllerInputEvent(const char* event,
                              int index,
                              uint16_t buttons,
                              uint16_t miscButtons,
                              uint8_t dpad,
                              const char* pressedButtons,
                              const char* pressedDirections,
                              int32_t axisX,
                              int32_t axisY,
                              int32_t axisRX,
                              int32_t axisRY,
                              int32_t brake,
                              int32_t throttle) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"%s\",\"controller_index\":%d,"
        "\"valid_report\":true,\"buttons\":%u,\"misc_buttons\":%u,\"dpad\":%u,"
        "\"pressed_buttons\":\"%s\",\"pressed_directions\":\"%s\","
        "\"axis_x\":%ld,\"axis_y\":%ld,\"axis_rx\":%ld,\"axis_ry\":%ld,"
        "\"brake\":%ld,\"throttle\":%ld}\n",
        btdiag::kProtocolVersion, elapsedMs(), event, index, static_cast<unsigned int>(buttons),
        static_cast<unsigned int>(miscButtons), static_cast<unsigned int>(dpad), pressedButtons, pressedDirections,
        static_cast<long>(axisX), static_cast<long>(axisY), static_cast<long>(axisRX), static_cast<long>(axisRY),
        static_cast<long>(brake), static_cast<long>(throttle));
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
        "\"derived_from\":\"%s\",\"baseline_tree\":\"%s\","
        "\"arduino\":\"%s\",\"esp_idf\":\"%s\",\"bluepad32\":\"%s\",\"btstack\":\"%s\"}\n",
        kProtocolVersion, elapsedMs(), kBoard, kPlatform, kTemplateRevision, kDerivedFrom, kBaselineTree,
        ESP_ARDUINO_VERSION_STR, esp_get_idf_version(), UNI_VERSION_STRING, BTSTACK_VERSION_STRING);
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

void emitControllerConnected(int index,
                             const char* modelName,
                             uint16_t modelId,
                             uint8_t subtype,
                             uint16_t vendorId,
                             uint16_t productId,
                             const uint8_t address[6]) {
    emitControllerEvent("controller_connected", index, modelName, modelId, subtype, vendorId, productId, address);
}

void emitControllerDisconnected(int index,
                                const char* modelName,
                                uint16_t modelId,
                                uint8_t subtype,
                                uint16_t vendorId,
                                uint16_t productId,
                                const uint8_t address[6]) {
    emitControllerEvent("controller_disconnected", index, modelName, modelId, subtype, vendorId, productId, address);
}

void emitControllerFirstInput(int index,
                              uint16_t buttons,
                              uint16_t miscButtons,
                              uint8_t dpad,
                              const char* pressedButtons,
                              const char* pressedDirections,
                              int32_t axisX,
                              int32_t axisY,
                              int32_t axisRX,
                              int32_t axisRY,
                              int32_t brake,
                              int32_t throttle) {
    emitControllerInputEvent("controller_first_input", index, buttons, miscButtons, dpad, pressedButtons,
                             pressedDirections, axisX, axisY, axisRX, axisRY, brake, throttle);
}

void emitControllerInput(int index,
                         uint16_t buttons,
                         uint16_t miscButtons,
                         uint8_t dpad,
                         const char* pressedButtons,
                         const char* pressedDirections,
                         int32_t axisX,
                         int32_t axisY,
                         int32_t axisRX,
                         int32_t axisRY,
                         int32_t brake,
                         int32_t throttle) {
    emitControllerInputEvent("controller_input", index, buttons, miscButtons, dpad, pressedButtons,
                             pressedDirections, axisX, axisY, axisRX, axisRY, brake, throttle);
}

void emitBondClearConfirmationRequired(unsigned long timeoutMs) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,"
        "\"event\":\"bond_clear_confirmation_required\",\"confirm_command\":\"CONFIRM_CLEAR_BONDS\","
        "\"cancel_command\":\"CANCEL_CLEAR_BONDS\",\"timeout_ms\":%lu,\"bonds_changed\":false}\n",
        kProtocolVersion, elapsedMs(), timeoutMs);
}

void emitBondClearConfirmationRejected(const char* reason) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,"
        "\"event\":\"bond_clear_confirmation_rejected\",\"reason\":\"%s\",\"bonds_changed\":false}\n",
        kProtocolVersion, elapsedMs(), reason);
}

void emitBondClearCancelled(bool hadPendingRequest) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"bond_clear_cancelled\","
        "\"had_pending_request\":%s,\"bonds_changed\":false}\n",
        kProtocolVersion, elapsedMs(), hadPendingRequest ? "true" : "false");
}

void emitBondClearExpired(unsigned long waitedMs, unsigned long timeoutMs) {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"bond_clear_expired\","
        "\"waited_ms\":%lu,\"timeout_ms\":%lu,\"bonds_changed\":false}\n",
        kProtocolVersion, elapsedMs(), waitedMs, timeoutMs);
}

void emitBondClearCompleted() {
    Serial.printf(
        "BTDIAG {\"protocol_version\":%u,\"elapsed_ms\":%lu,\"event\":\"bond_clear_completed\","
        "\"bonds_changed\":true,\"restart_required\":true}\n",
        kProtocolVersion, elapsedMs());
}

}  // namespace btdiag
