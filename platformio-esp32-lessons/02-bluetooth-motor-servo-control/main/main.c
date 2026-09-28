// SPDX-License-Identifier: Apache-2.0
// Derived from the Bluepad32 Arduino template at the revision in DEPENDENCIES.md.

#include "sdkconfig.h"

#include <stddef.h>

#include <arduino_platform.h>
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include <btstack_stdio_esp32.h>
#include <uni.h>

int app_main(void) {
#ifndef CONFIG_ESP_CONSOLE_UART_NONE
#ifndef CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
    btstack_stdio_init();
#endif
#endif

    btstack_init();
    uni_platform_set_custom(get_arduino_platform());
    uni_init(0, NULL);

    // Bluepad32 starts the Arduino setup()/loop() task from its custom
    // platform bootstrap. This call owns the ESP-IDF application task.
    btstack_run_loop_execute();
    return 0;
}
