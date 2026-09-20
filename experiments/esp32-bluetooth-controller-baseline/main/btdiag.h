// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace btdiag {

inline constexpr unsigned int kProtocolVersion = 1;

void emitFirmwareStarted();
void emitDependencyIdentity();
void emitBluetoothReady();

}  // namespace btdiag
