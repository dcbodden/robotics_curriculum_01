# Lesson 01 Verification Record

## Automated Build

- **Date:** 2026-09-19
- **Command:** `pio run` from `platformio-esp32-lessons/01-hello-world`
- **Environment:** `esp32doit-devkit-v1`
- **Result:** PASS — PlatformIO completed the release build without compiler or configuration errors.
- **Reported usage:** 21,464 bytes RAM (6.6%) and 269,305 bytes application flash (20.5%).

## Specification Review

- **Date:** 2026-09-19
- **Result:** PASS
- The standalone project contains `platformio.ini`, `src/main.cpp`, and student instructions.
- The configuration selects `espressif32`, `esp32doit-devkit-v1`, Arduino, and a 115200-baud monitor.
- The firmware uses `LED_BUILTIN`, prints `Blinking Fibonacci N`, calls groups `1, 1, 2, 3, 5` in order, uses 250-millisecond on/off phases, and adds a 1,000-millisecond group pause.
- The student instructions describe the exact messages and blink groups, distinguish the controllable GPIO 2 LED from the power indicator, and require no external circuit.
- Strict OpenSpec validation passed for `add-esp32-hello-world-lesson`.

## Physical Hardware Test

- **Date:** 2026-09-19
- **Hardware:** Supported 30-pin ESP32-WROOM DevKit
- **Evidence source:** User-reported physical observation; not independently observed by the coding agent.
- **Result:** PASS — after selecting the correct `esp32doit-devkit-v1` PlatformIO project, the user reported that the upload and lesson worked.
- **Recorded observations:** 115200-baud Fibonacci messages, matching onboard LED groups, visible group boundaries, and restart at one after the five-blink group all worked as intended.

An earlier failed upload built the unrelated `uno` environment and invoked `avrdude`; it was diagnosed as the wrong active PlatformIO project and is not a failure of this ESP32 lesson configuration.
