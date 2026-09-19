## Why

The curriculum has an advanced ESP32 robotics template but no small, independently buildable ESP32 lesson that verifies a new board, upload path, Serial Monitor, and controllable onboard LED before more complex work. A USB-only Fibonacci blink lesson provides an unambiguous first success and establishes a home for a future numbered ESP32 lesson series.

## What Changes

- Add a new `platformio-esp32-lessons/` collection parallel to `platformio-uno-lessons/`, with a collection index that explains how to open and use its standalone projects.
- Add `platformio-esp32-lessons/01-hello-world/` as an independently buildable Arduino-framework PlatformIO project for a 30-pin DOIT ESP32 DevKit V1 carrying an ESP32-WROOM module.
- Make the lesson repeatedly announce and blink the visible Fibonacci sequence `1, 1, 2, 3, 5`, using 250 milliseconds on, 250 milliseconds off, and an additional one-second pause after each group.
- Provide student instructions that use the onboard controllable LED and USB Serial Monitor as two correlated forms of evidence, while distinguishing the controllable LED from the power LED and documenting compatible-board troubleshooting.
- Update the repository guide to identify the new ESP32 lesson collection without changing the existing canonical beginner route.

## Capabilities

### New Capabilities

- `esp32-lessons`: Defines the standalone ESP32 lesson collection, the supported first-lesson hardware and PlatformIO configuration, and the observable Fibonacci blink-and-serial behavior.

### Modified Capabilities

- `repository-structure`: Adds the numbered `platformio-esp32-lessons/` collection to the documented top-level curriculum layout.

## Impact

- Adds a new top-level lesson collection, collection README, and one PlatformIO project with source and student instructions.
- Targets PlatformIO's `esp32doit-devkit-v1` board definition with the Arduino framework and a 115200-baud Serial Monitor.
- Reuses the existing PlatformIO toolchain; no new runtime libraries or external circuit components are required.
- Updates the root repository guide while leaving `platformio-esp32-robotics-template/`, the Uno lesson series, and the canonical beginner sequence intact.
