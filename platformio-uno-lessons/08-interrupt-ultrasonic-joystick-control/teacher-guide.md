# Lesson 08 Teacher Guide: Interrupt Ranging During Control

Use the [student lesson](README.md) to compare interrupt-driven HC-SR04 ranging with continued joystick motor/servo control. The conceptual goal is that sensing, control, and reporting can run at different rhythms without the foreground waiting for each Echo.

## Learning Focus

Students should be able to:

- distinguish the 20 ms control, 80 ms Trigger, Echo-pulse, and approximately 500 ms report rhythms;
- observe that motor/servo commands can change between Serial reports;
- connect Echo HIGH duration with round-trip travel and calculated one-way distance;
- treat an invalid Echo as missing/expired data rather than zero distance; and
- describe an interrupt as a short response to an event, not as a second program running independently.

ISR ordering, AVR timer registers, unsigned rollover, and atomic multi-byte copies are teacher/deeper-reference topics.

## Preparation and Power Safety

Lesson 08 retains lesson 06's complete actuator power requirements. Confirm the actual servo and motor voltage ranges and peak/stall-current evidence, then choose a regulated 5 V supply with suitable margin, transient behavior, current limiting, connectors, branch wiring, MOSFET, and diode ratings. Do not infer current capacity from kit labels or unloaded motion, and do not deliberately stall an actuator.

USB powers the Uno. Arduino 5 V powers the joystick and HC-SR04 only. External +5 V powers servo red and motor positive on separate rated branches. These positive rails remain separate; Arduino GND and external negative share a reference. Route actuator returns so motor/servo current does not flow through the joystick or sensor return wiring. Add verified local decoupling near actuator branches as indicated by the actual supply and measured behavior; capacitors do not replace adequate supply capacity.

Before class, secure the bare motor without an attachment, secure the unloaded servo, stabilize the sensor, inspect the rigid target, build the firmware, and run both host checks. Test USB-only telemetry first. Enable actuator power only after the released motor repeatedly commands zero and all wiring/power checks pass.

## Required Power-Off Trace

Remove USB and external power before inspection or changes.

| Subsystem | Required connections |
| --- | --- |
| HC-SR04 | VCC → Arduino 5 V; Trig → D4; Echo → D2/INT0; GND → shared ground |
| Joystick | +5V → Arduino 5 V; GND → shared ground; VRx → A0; VRy → A1; SW open |
| Servo | Orange → D9; red → external +5 V; brown → shared ground |
| Motor switch | D3 → 220–330 ohms → Gate; 10 kilohms Gate → ground; Source → ground; Drain → motor negative |
| Motor power/protection | External +5 V → motor positive; 1N5817 banded cathode → positive; unbanded anode → Drain |

Verify actual module labels, MOSFET terminals, servo lead functions, split breadboard rails, supply polarity, continuity of every shared ground, and separation of positive rails. Sensor GND and servo brown never connect to the switched Drain. Keep actuator power off during Build, Upload, neutral adjustment, and initial sensor/input checks.

## Interpret the Timing Rhythms

| Work | Timing and ownership |
| --- | --- |
| Joystick/control update | Timer1 overflow ISR every nominal 20 ms; both ADC axes and both commands always update |
| Servo signal | Timer1 hardware OC1A on D9; one position pulse per 20 ms frame |
| Motor PWM | Timer2 hardware on D3; faster PWM continues independently |
| Sensor Trigger | End of every fourth Timer1 frame; D4 HIGH for 10 us, giving 80 ms spacing |
| Echo capture | D2/INT0 CHANGE ISR timestamps recognized rise/fall edges |
| Serial telemetry | Foreground atomic snapshot and report followed by `delay(500)` |

Timer1 uses Fast PWM mode 14, prescaler 8, and ICR1 = 39999 at 16 MHz. Each tick is 0.5 microseconds, so 40,000 ticks form 20 ms. OCR1A is buffered; a new servo command applies to a later hardware frame rather than changing a pulse already underway. Timer0 continues Arduino timekeeping, and Timer2 continues D3 motor PWM.

The 500 ms delay is approximate report pacing, not control pacing. Interrupts remain enabled during `delay`, so roughly 25 control frames and six or seven 80 ms sensor schedules can occur between reports. Serial formatting time adds to the report interval.

## ISR Ordering and the Bounded Echo Window

The Timer1 overflow routine performs this sequence:

```text
expire any still-pending Echo attempt
read A0 and A1
calculate and issue D3/D9 commands
publish control telemetry
on every fourth frame: arm Echo, pulse D4, finish the ISR
```

Trigger is last so a near Echo has almost a full frame to complete and control work is never skipped. If a D2 edge becomes pending before Timer1 returns, INT0 is serviced immediately afterward.

The D2 CHANGE handler accepts only the expected sequence:

```text
WAITING_FOR_RISE + HIGH -> save micros(), wait for fall
WAITING_FOR_FALL + LOW  -> unsigned elapsed time, publish valid, become idle
anything else           -> ignore
```

Unsigned subtraction remains correct across `micros()` rollover. The handler performs no Serial output and no floating-point calculation.

At the next 20 ms frame, an attempt still waiting for either edge expires: state becomes idle, duration becomes zero, and validity becomes false. Later unmatched edges are ignored. This is a deliberate curriculum bound, not the HC-SR04's maximum capability. A 4 m Echo is about 23.2 ms and cannot fit; the practical window is slightly less than 20 ms because triggering occurs after the frame's control work. Avoid promising an exact maximum accepted distance. Lesson 07 remains the advertised-range activity.

The 80 ms Trigger spacing and one-frame validity window answer different questions: spacing avoids retriggering the module too rapidly, while the short validity window guarantees the next control frame resets unfinished acquisition without skipping joystick work.

## Atomic Telemetry and Foreground Conversion

The ATmega328P is an 8-bit processor. Echo duration and ADC/servo values span multiple bytes; copying them while an ISR updates them could combine bytes from different moments. `takeTelemetrySnapshot()` uses an atomic block only for the copies, including validity and duration, then restores the previous interrupt state.

Serial output and these floating-point conversions occur after interrupts are restored:

```text
round_trip_ms = echo_duration_us / 1000
distance_cm   = echo_duration_us / 58
```

This provides a consistent report without disabling interrupts throughout slow formatting. A snapshot can legitimately show whichever completed or expired attempt was latest when copied; it is not a log of all sensor attempts.

## Facilitate the Core Mission

1. Require the complete power-off trace and secured-motion check.
2. With actuator power off, verify build/upload, 9,600-baud telemetry, repeated released motor command zero, changing A0/A1 values, and a valid near-target Echo.
3. If Echo is invalid, resolve sensor wiring/target geometry before adding actuator motion. Invalid is acceptable as an observed outcome only after wiring is known safe; it is never zero distance.
4. Enable actuator power only after the USB-only gate passes. Keep the fixed rig immobile and move the target by hand only while the joystick is released and hands remain outside actuator motion areas.
5. Ask the student to make one small servo-axis change and one brief moderate motor-axis command with a stationary target. Look for physical response between reports, but accept “hard to tell.”
6. Compare a near and farther target using the same material/orientation. Do not require exact ruler agreement.
7. End with external power off first, then Monitor closed and USB removed.

This lesson observes distance but does not alter motor power based on it. Do not imply collision avoidance, fail-safe behavior, calibrated ranging, or a guaranteed reaction time.

## Expected Telemetry

A valid line includes all measurement and control fields:

```text
Echo: valid | Echo delay: 0.580 ms | Distance: 10.0 cm | Motor ADC: 506 | Motor PWM command: 0 | Servo ADC: 512 | Servo pulse command (us): 1500
```

An unfinished/missing attempt reports:

```text
Echo: invalid/no echo | Motor ADC: 506 | Motor PWM command: 0 | Servo ADC: 512 | Servo pulse command (us): 1500
```

ADC values vary by joystick. PWM is a command, not measured motor speed; servo microseconds are a position command, not guaranteed physical angle.

## Troubleshooting Sequence

1. **No/garbled Serial:** Select lesson 08, confirm Build/Upload, use 9,600 baud, and close competing port users.
2. **All Echo attempts invalid:** With both supplies removed, trace sensor labels to 5 V/D4/D2/ground. Then test USB-only with a broad rigid target at 10–30 cm. Inspect shifted headers, target angle, and loose returns.
3. **Near valid, far invalid:** Explain the one-frame bound. Confirm lesson 07 if full-range behavior needs testing; do not lengthen the window or skip joystick frames for this lesson.
4. **Released motor is nonzero:** Keep actuator power off. Record repeated A0 neutral values and verify joystick wiring before adjusting `MOTOR_NEUTRAL_ADC` or deadband.
5. **Servo binds/buzzes:** Remove both supplies. Verify unloaded mechanics, orange/red/brown routing, supply stability, and pulse limits; narrow travel toward center rather than forcing an endpoint.
6. **High motor command but no motion:** Remove both supplies promptly. Verify external motor branch, MOSFET pins, gate resistor/pulldown, shared ground, motor continuity, and diode polarity. Do not leave it stalled.
7. **Resets, chatter, or unstable readings during motion:** Remove both supplies. Check supply peak capability, distribution, return paths, connector ratings, voltage at loads, transient capture, and local decoupling.
8. **Heat, odor, smoke, sparks, or unexpected motion:** Remove external power and USB immediately, isolate the setup, and resolve the cause before reuse.

## Code Map

- [`src/main.cpp`](src/main.cpp) owns Timer1 control scheduling, 80 ms Trigger division, D2 acquisition state, atomic snapshots, and foreground reports.
- [`include/control_mapping.h`](include/control_mapping.h) preserves lesson 06 motor and servo mappings.
- [`include/ultrasonic_math.h`](include/ultrasonic_math.h) contains foreground unit conversion.
- [`test/test_control_mapping.cpp`](test/test_control_mapping.cpp) exhaustively checks all ADC inputs and boundaries.
- [`test/test_ultrasonic_math.cpp`](test/test_ultrasonic_math.cpp) checks representative time/distance conversions.

## Verification

From `platformio-uno-lessons/08-interrupt-ultrasonic-joystick-control`:

```bash
pio run
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson08-ultrasonic-check
/tmp/lesson08-ultrasonic-check
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_control_mapping.cpp -o /tmp/lesson08-mapping-check
/tmp/lesson08-mapping-check
```

Expected final results include PlatformIO `SUCCESS` and:

```text
Ultrasonic checks passed: zero, 10 cm, 50 cm, and 4,000 mm conversions.
Mapping checks passed: neutral, boundaries, endpoints, clamps, and all 1024 ADC values.
```

Hardware verification is separate evidence: confirm labeled valid and invalid reports, reasonable near/far direction of change, repeated zero command on release, and continued actuator response between foreground reports. Record unperformed electrical or waveform measurements as not measured.
