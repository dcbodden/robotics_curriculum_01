## 1. Lesson 07 Blocking Distance Measurement

- [x] 1.1 Scaffold `07-ultrasonic-blocking-distance` as an independent Uno PlatformIO project with the repository's standard editor recommendations and 9,600-baud monitor configuration.
- [x] 1.2 Add deterministic Echo-duration conversion helpers and host checks for representative millisecond/centimeter values, the 4,000 mm range, and invalid input handling.
- [x] 1.3 Implement D4 Trigger, D2 Echo, a timeout slightly beyond the advertised maximum Echo duration, explicit no-Echo reporting, fractional-millisecond and centimeter output, and the 500 ms blocking loop delay.
- [x] 1.4 Write the lesson 07 README and teacher reference with the Core Mission, exact power-off HC-SR04 wiring, target-position activity, round-trip explanation, timeout behavior, troubleshooting, and documented build/test commands.

## 2. Lesson 08 Interrupt-Driven Measurement

- [x] 2.1 Scaffold `08-interrupt-ultrasonic-joystick-control` from lesson 06 as a standalone project, preserving its control mapping, protected actuator circuit, Timer1/Timer2 ownership, and existing mapping checks.
- [x] 2.2 Add the D2 CHANGE-interrupt acquisition state, rollover-safe Echo timestamps, unmatched-edge rejection, and atomic measurement telemetry snapshots without Serial or floating-point conversion inside the edge ISR.
- [x] 2.3 Extend the Timer1 routine to expire incomplete acquisition, perform every existing joystick/control update, divide frames into an 80 ms sensor schedule, and issue the D4 Trigger pulse as the scheduled frame's final operation.
- [x] 2.4 Add approximately 500 ms foreground reports that distinguish measurement validity, fractional Echo milliseconds, calculated centimeters, joystick readings, and continuing motor/servo commands.
- [x] 2.5 Add or extend host checks for ultrasonic conversion and preserved lesson 06 mappings, then document the commands and expected results.
- [x] 2.6 Write the lesson 08 README and teacher timing reference with its Core Mission, complete lesson 06 plus HC-SR04 wiring, 20/80/500 ms rhythm comparison, deliberately bounded Echo window, atomic-data explanation, safety gates, and troubleshooting.

## 3. Lesson 09 Distance-Aware Motor Control

- [x] 3.1 Scaffold `09-distance-aware-motor-control` from lesson 08 as a standalone project with the same interrupt acquisition and coordinated joystick/servo baseline.
- [ ] 3.2 Add pure requested-to-applied PWM policy helpers for the strict 580 microsecond/10 cm boundary, 50 percent slowdown, full-control valid-far behavior, full-control invalid behavior, zero preservation, and 0–255 bounds.
- [ ] 3.3 Implement separate acquisition and persistent policy states so a completed near reading immediately reduces the latest request, later Timer1 frames preserve slowdown for new requests, and the next timeout or valid non-near result restores full control.
- [ ] 3.4 Publish and atomically report measurement validity, Echo time, distance, requested PWM, applied PWM, slowdown status, and existing servo/control telemetry approximately every 500 ms.
- [ ] 3.5 Extend host checks to cover durations immediately below, at, and above 580 microseconds; representative and maximum PWM requests; invalid results; and unchanged lesson 06 joystick/servo mappings.
- [ ] 3.6 Write the lesson 09 README and teacher reference with a stationary-target Core Mission, requested-versus-applied observations, full-control no-Echo policy, threshold chatter expectations, immobile secured-actuator rules, non-safety-system limitation, and troubleshooting.

## 4. Curriculum Integration and Verification

- [ ] 4.1 Update the PlatformIO Uno lesson index, repository beginner sequence, and prerequisite/next-lesson links to present lessons 07–09 in canonical order.
- [ ] 4.2 Review all three lessons for consistent D4/D2 wiring, units, trigger spacing, report cadence, terminology, power-removal steps, teacher checkpoints, and stop conditions.
- [ ] 4.3 Run every documented host check and resolve failures or warning-level compiler diagnostics.
- [ ] 4.4 Build lessons 07, 08, and 09 for the Uno with PlatformIO and confirm each project remains independent of sibling lesson directories.
- [ ] 4.5 Perform a final requirements review against both delta specs and run strict OpenSpec validation for the change.
