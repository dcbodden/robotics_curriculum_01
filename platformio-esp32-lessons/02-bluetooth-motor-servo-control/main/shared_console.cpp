// SPDX-License-Identifier: Apache-2.0

#include "shared_console.h"

#include <btstack_stdin.h>
#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <sdkconfig.h>

#include <cstdarg>
#include <cstdio>

namespace {

constexpr size_t kInputQueueCapacity = 64;
QueueHandle_t inputQueue = nullptr;

void receiveCharacter(char value) {
    if (inputQueue == nullptr) return;
    xQueueSend(inputQueue, &value, 0);
}

}  // namespace

namespace shared_console {

bool begin() {
    if (inputQueue == nullptr) {
        inputQueue = xQueueCreate(kInputQueueCapacity, sizeof(char));
    }
    if (inputQueue == nullptr) return false;

    btstack_stdin_setup(receiveCharacter);
    return true;
}

int read() {
    char value = 0;
    if (inputQueue == nullptr || xQueueReceive(inputQueue, &value, 0) != pdTRUE) return -1;
    return static_cast<unsigned char>(value);
}

size_t tryWrite(const uint8_t* data, size_t length) {
    if (data == nullptr || length == 0) return 0;

    const uart_port_t port = static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM);
    size_t available = 0;
    if (uart_get_tx_buffer_free_size(port, &available) != ESP_OK || available < length) {
        return 0;
    }

    const int written = uart_write_bytes(port, data, length);
    return written == static_cast<int>(length) ? length : 0;
}

int printf(const char* format, ...) {
    va_list arguments;
    va_start(arguments, format);
    const int written = vprintf(format, arguments);
    va_end(arguments);
    return written;
}

}  // namespace shared_console
