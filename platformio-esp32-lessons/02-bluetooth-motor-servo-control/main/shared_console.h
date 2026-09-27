// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace shared_console {

// Attach command input to the UART driver that btstack_stdio_init() already
// owns. This function does not install, resize, or restart the UART driver.
bool begin();

// Return one queued command byte, or -1 when no byte is available.
int read();

// Queue a complete record only when BTstack's native TX ring has enough free
// capacity. Returning zero asks the caller to retry on a later loop pass.
size_t tryWrite(const uint8_t* data, size_t length);

// Formatted lifecycle diagnostics use the ESP-IDF VFS that BTstack bound to
// its existing UART driver.
int printf(const char* format, ...) __attribute__((format(printf, 1, 2)));

}  // namespace shared_console
