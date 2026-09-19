# ESP32 Lessons Specification

## Purpose

Defines independently buildable ESP32 lessons that give beginners clear physical and serial evidence that their board, toolchain, and uploaded program are operating correctly.

## Requirements

### Requirement: Standalone ESP32 lesson collection
The curriculum SHALL provide `platformio-esp32-lessons/` with a collection index and numbered, independently buildable PlatformIO projects. The first project SHALL be `01-hello-world/` and SHALL contain its own configuration, source code, and student instructions.

#### Scenario: Open the first ESP32 lesson
- **WHEN** a student opens `platformio-esp32-lessons/01-hello-world/` by itself in VS Code
- **THEN** the directory contains everything PlatformIO needs to build and upload the lesson without compiling another lesson or the advanced ESP32 robotics template

### Requirement: Supported ESP32 DevKit configuration
Lesson 01 SHALL target a 30-pin DOIT ESP32 DevKit V1 carrying an ESP32-WROOM module, use the Arduino framework, and configure the Serial Monitor for 115200 baud. The lesson SHALL identify the controllable onboard LED through the board definition rather than requiring an external LED circuit.

#### Scenario: Inspect the project configuration
- **WHEN** a contributor opens lesson 01's PlatformIO configuration
- **THEN** it selects the Espressif 32 platform, `esp32doit-devkit-v1` board, Arduino framework, and 115200-baud monitor

#### Scenario: Prepare the hardware
- **WHEN** a student follows the lesson's required setup
- **THEN** the student needs only the supported ESP32 board and a USB data cable, with no breadboard, jumper wiring, external LED, or external power supply

### Requirement: Repeating Fibonacci blink groups
Lesson 01 SHALL repeat the visible Fibonacci sequence `1, 1, 2, 3, 5` forever. At the start of each group it SHALL print `Blinking Fibonacci N`, where `N` is the group's blink count, and then blink the controllable onboard LED exactly `N` times. Each blink SHALL hold the LED on for 250 milliseconds and off for 250 milliseconds, followed by an additional 1,000-millisecond pause after the group.

#### Scenario: Observe one complete sequence
- **WHEN** the uploaded lesson runs through five consecutive groups
- **THEN** the Serial Monitor displays `Blinking Fibonacci 1`, `Blinking Fibonacci 1`, `Blinking Fibonacci 2`, `Blinking Fibonacci 3`, and `Blinking Fibonacci 5` in order while the onboard LED produces matching groups of 1, 1, 2, 3, and 5 blinks

#### Scenario: Observe continued operation
- **WHEN** the group of five blinks and its group pause finish
- **THEN** the next group returns to `Blinking Fibonacci 1` and the same five-group sequence continues

### Requirement: Correlated beginner evidence
Lesson 01 SHALL guide the student to use the Serial Monitor message and the corresponding onboard LED blink count as separate, correlated evidence of operation. It SHALL distinguish the controllable onboard LED from the always-on power LED and SHALL provide concise troubleshooting for upload, monitor-speed, and nominally compatible boards without a controllable GPIO 2 LED.

#### Scenario: Verify a group using two outputs
- **WHEN** a student observes a `Blinking Fibonacci N` message and the following blink group
- **THEN** the lesson prompts the student to confirm that the message value matches the number of visible blinks

#### Scenario: Encounter only a power indicator
- **WHEN** upload and serial output succeed but no controllable onboard LED blinks
- **THEN** the lesson directs the student to verify the exact board variant and GPIO 2 LED rather than treating an always-on power LED as program output
