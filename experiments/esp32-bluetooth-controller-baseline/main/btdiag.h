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

}  // namespace btdiag
