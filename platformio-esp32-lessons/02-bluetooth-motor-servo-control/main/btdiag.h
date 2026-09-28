// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace btdiag {

inline constexpr unsigned int kProtocolVersion = 1;

void emitFirmwareStarted();
void emitDependencyIdentity();
void emitBluetoothReady();
void emitAcceptingConnections(unsigned long timeoutMs);
void emitWaitingForController(unsigned long waitedMs, unsigned long remainingMs);
void emitConnectionTimeout(unsigned long waitedMs, unsigned long timeoutMs);
void emitControllerConnected(int index,
                             const char* modelName,
                             uint16_t modelId,
                             uint8_t subtype,
                             uint16_t vendorId,
                             uint16_t productId,
                             const uint8_t address[6]);
void emitControllerDisconnected(int index,
                                const char* modelName,
                                uint16_t modelId,
                                uint8_t subtype,
                                uint16_t vendorId,
                                uint16_t productId,
                                const uint8_t address[6]);
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
                              int32_t throttle);
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
                         int32_t throttle);
void emitBondClearConfirmationRequired(unsigned long timeoutMs);
void emitBondClearConfirmationRejected(const char* reason);
void emitBondClearCancelled(bool hadPendingRequest);
void emitBondClearExpired(unsigned long waitedMs, unsigned long timeoutMs);
void emitBondClearCompleted();

}  // namespace btdiag
