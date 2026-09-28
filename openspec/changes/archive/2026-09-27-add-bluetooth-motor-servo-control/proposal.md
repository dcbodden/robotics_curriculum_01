## Why

The repository proves that an SZ-4015B controller can communicate with an ESP32-WROOM and separately teaches joystick-driven motor and servo commands, but it does not yet combine those capabilities into a physically verified wireless actuator-control lesson. A standalone ESP32 bench lesson will establish that integration before batteries, power conversion, chassis construction, or complete rover wiring are introduced later.

## What Changes

- Add `platformio-esp32-lessons/02-bluetooth-motor-servo-control/` as an independently buildable ESP32-WROOM lesson that retains the pinned Bluepad32 Bluetooth Classic connection mechanism and diagnostic evidence from the controller baseline experiment.
- Translate the PS4-compatible controller's left stick into a signed forward/stop/reverse motor command and a bounded servo-position command with calibrated dead zones.
- Drive one Elegoo Arduino Car V1-style 48:1 geared motor through one channel of an HW-627 DRV8833 module and drive one externally powered bench servo from a separate ESP32 PWM output.
- Add explicit arming, neutral-entry, stale-input, disconnection, disarming, and direction-reversal safety behavior so motor output cannot resume from an old controller command.
- Document a USB-powered ESP32 and regulated bench actuator supply with a shared ground, secured unloaded actuators, local supply bypass and bulk capacitance, brushed-motor noise suppression, and the reason a single flywheel diode must not be placed across a bidirectional motor.
- Add host-verifiable control mapping and safety-policy checks, reproducible firmware build evidence, and a staged physical bench procedure that distinguishes calculated commands from observed actuator behavior.
- Keep batteries, DC/DC regulators, chassis assembly, wheels, mechanical steering linkage, and complete rover wiring out of scope.

## Capabilities

### New Capabilities

- `esp32-bluetooth-actuator-experiment`: Defines the standalone wireless controller-to-DRV8833-and-servo ESP32 bench lesson, its control and failsafe behavior, its electrical guidance, and its verification evidence.

### Modified Capabilities

None. The existing Bluetooth controller baseline remains an independent diagnostic experiment; the new capability adds Lesson 02 without changing the requirements already defined for Lesson 01.

## Impact

- Adds numbered Lesson 02 under `platformio-esp32-lessons/`, derived from the ESP32 Bluetooth controller baseline rather than modifying that baseline in place.
- Reuses the supported 30-pin ESP32-WROOM DevKit V1, pinned pioarduino/ESP-IDF/Arduino/Bluepad32 dependency model, project-local PlatformIO wrapper approach, and 115200-baud diagnostics.
- Introduces ESP32 PWM output handling for one DRV8833 H-bridge channel and one hobby servo, plus pure mapping and safety-policy code suitable for host checks.
- Adds student-facing bench missions plus teacher guidance for wiring, power integrity, safety, dependencies, and verification of the HW-627 module, geared motor, and servo.
- Updates the ESP32 lesson collection index and repository navigation to identify the new lesson and its relationship to the controller baseline experiment and future rover work.
- Does not alter the Arduino Uno lesson sequence or claim that the later wireless rover power and mechanical integration is complete.
