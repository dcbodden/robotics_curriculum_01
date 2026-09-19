## Why

The Arduino Uno sequence currently ends with coordinated joystick control but does not let students measure the environment or use a sensor reading to modify an actuator command. A three-lesson HC-SR04 progression will connect blocking measurement, external interrupts, and sensor-informed motor control using the timing and safety patterns students already learned.

## What Changes

- Add lesson 07 as a standalone HC-SR04 project that uses a bounded blocking Echo measurement, reports fractional round-trip milliseconds and calculated centimeters, and waits 500 ms between reports.
- Add lesson 08 as a standalone extension of lesson 06 that preserves 20 ms joystick motor/servo control while D2 external interrupts capture HC-SR04 Echo edges. D4 initiates a measurement every four control frames, providing 80 ms trigger spacing.
- Add lesson 09 as a standalone distance-aware extension that distinguishes requested and applied motor PWM, applies a 50% slowdown for valid measurements closer than 10 cm, and retains full joystick control for invalid or missing Echo measurements.
- Add pure-function host checks for distance conversion, the 10 cm boundary, slowdown mapping, invalid-reading behavior, bounds, and unchanged joystick/servo mappings where applicable.
- Add beginner-facing Core Missions, exact wiring and safe-use guidance, teacher timing references, troubleshooting, and navigation updates for lessons 07–09.
- Extend the canonical beginner route from coordinated joystick control through blocking ranging, interrupt-driven ranging, and distance-informed control.

## Capabilities

### New Capabilities

- `arduino-uno-ultrasonic-lessons`: Defines the standalone HC-SR04 measurement, interrupt integration, distance-aware motor behavior, reporting, testing, wiring, and instructional requirements for lessons 07–09.

### Modified Capabilities

- `beginner-curriculum-progression`: Extends the canonical learning route and prerequisite links through the three ultrasonic lessons.

## Impact

- Adds three independently buildable projects under `platformio-uno-lessons/` and updates that collection's index and relevant repository navigation.
- Reuses lesson 06's A0/A1 joystick inputs, D3 motor PWM, D9 servo output, Timer1 control frame, external actuator supply, and safety model in lessons 08 and 09 without changing lesson 06 behavior.
- Adds HC-SR04 wiring on Arduino 5 V/GND, D4 Trigger, and D2/INT0 Echo; no third-party sensor library is required.
- Introduces external-interrupt state shared with the Timer1 ISR and foreground reporting, requiring bounded ISRs and atomic snapshots of multi-byte telemetry.
- Adds no breaking changes to existing lesson projects.
