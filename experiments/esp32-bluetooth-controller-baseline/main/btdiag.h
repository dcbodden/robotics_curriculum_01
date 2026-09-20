// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace btdiag {

inline constexpr unsigned int kProtocolVersion = 1;

void emitFirmwareStarted();
void emitDependencyIdentity();
void emitBluetoothReady();
void emitAcceptingConnections(unsigned long timeoutMs);
void emitWaitingForController(unsigned long waitedMs, unsigned long remainingMs);
void emitConnectionTimeout(unsigned long waitedMs, unsigned long timeoutMs);

}  // namespace btdiag
