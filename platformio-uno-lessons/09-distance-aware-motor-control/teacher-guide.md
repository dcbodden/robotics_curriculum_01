# Lesson 09 Teacher Guide: Distance-Aware Motor Control

Use the [student lesson](README.md) to demonstrate a sensor-informed motor policy while the rig remains immobile. The conceptual focus is the distinction between a joystick-requested command and the command actually applied after the latest completed Echo result.

## Learning Focus

Students should be able to:

- distinguish requested motor PWM from applied motor PWM;
- state that a valid Echo below 580 microseconds selects 50% output, while the boundary and farther readings select full output;
- recognize that invalid/no Echo restores full requested control in this demonstration;
- explain why invalid data proves neither zero distance nor a clear path;
- expect raw ON/OFF chatter near the 10 cm threshold; and
- observe the policy only with a stationary target, secured bare motor, unloaded servo, and immobile rig.

ISR ordering, persistent policy state, AVR timer registers, atomic snapshots, and integer boundary tests are teacher/deeper-reference topics.

## Non-Safety-System Limitation

Lesson 09 is not collision avoidance, a protective stop, a fail-safe controller, or a mobile-robot trial. Missing or late Echo data explicitly selects **full requested motor control**. That choice supports the requested classroom comparison on a secured, immobile rig, but it would be inappropriate evidence that a real path is clear.

Do not attach a wheel, propeller, gear, linkage, load, or other mechanism to the motor or servo. Do not move the rig toward a target. Keep the sensor, Arduino, breadboard, bare motor, and unloaded servo fixed. A student may reposition the target by hand only with the joystick released and hands outside actuator motion areas. For observing invalid results or threshold chatter, use zero motor request or keep external actuator power off.

## Preparation and Power Safety

Lesson 09 retains lesson 06's complete actuator power requirements. Confirm actual motor and servo voltage ranges and peak/stall-current evidence, then choose a regulated 5 V supply with suitable margin, transient behavior, current limiting, connectors, branch wiring, MOSFET, and diode ratings. Do not infer capacity from kit labels or unloaded motion, and do not deliberately stall an actuator.

USB powers the Uno. Arduino 5 V powers the joystick and HC-SR04 only. External +5 V powers servo red and motor positive on separate rated branches. Keep the positive rails separate; join Arduino GND and external negative as the common reference. Route actuator returns so motor/servo current does not flow through joystick or sensor return wiring. Add verified local decoupling where the actual supply and measurements call for it; capacitance does not replace adequate supply capacity.

Before class, secure the bare motor without an attachment, secure the unloaded servo, stabilize the sensor and target support, build the firmware, and run all three host checks. Test USB-only telemetry first. Enable actuator power only after released motor requests repeatedly read zero and all wiring, power, and motion-area checks pass.

## Required Power-Off Trace

Remove USB and external power before inspection or changes.

| Subsystem | Required connections |
| --- | --- |
| HC-SR04 | VCC → Arduino 5 V; Trig → D4; Echo → D2/INT0; GND → shared ground |
| Joystick | +5V → Arduino 5 V; GND → shared ground; VRx → A0; VRy → A1; SW open |
| Servo | Orange → D9; red → external +5 V; brown → shared ground |
| Motor switch | D3 → 220–330 ohms → Gate; 10 kilohms Gate → ground; Source → ground; Drain → motor negative |
| Motor power/protection | External +5 V → motor positive; 1N5817 banded cathode → positive; unbanded anode → Drain |

Verify actual module labels, MOSFET terminals, servo lead functions, split breadboard rails, supply polarity, every shared-ground connection, and positive-rail separation. Sensor GND and servo brown never connect to the switched Drain. Keep actuator power off during Build, Upload, neutral checks, invalid-Echo demonstrations, and initial sensor/input checks.

## Explain the Policy Boundary

The code compares raw Echo duration, avoiding floating-point work in the edge ISR:

| Latest completed result | Slowdown state | Applied command |
| --- | --- | --- |
| Valid, 579 us or less | ON | `requested / 2` |
| Valid, exactly 580 us | OFF | `requested` |
| Valid, 581 us or more | OFF | `requested` |
| Invalid/no Echo | OFF | `requested` |

The rule is strictly less than 580 microseconds, the approximate round-trip time at 10 cm. Integer division floors odd requests: 201 becomes 100 and 255 becomes 127. Zero remains zero, and every applied result remains in the 0–255 PWM range. Make clear that PWM is a software command, not measured rotational speed.

Avoid promising ruler-level precision. Temperature, target geometry, reflections, sensor variation, and timing quantization all influence readings. The lesson intentionally uses one approximate threshold without calibration, filtering, or hysteresis.

## Acquisition State and Persistent Policy State

Two kinds of state coexist:

- **acquisition state** says whether the current ping is idle, waiting for an Echo rise, or waiting for its fall;
- **policy state** says whether the most recently completed attempt selected slowdown.

Starting a new ping does not clear the previous policy. This keeps the output stable while the next measurement is in progress. A valid falling edge publishes the duration, updates slowdown, and immediately applies that policy to the latest requested command. Each later Timer1 frame applies the same policy to its newly calculated request.

If an attempt is still incomplete at the next 20 ms Timer1 frame, the routine marks it invalid, clears slowdown, and immediately applies the full latest request. Later unmatched D2 edges are ignored. Thus an invalid transition may increase applied PWM from half to full without a joystick change. Demonstrate that transition only USB-only or with a zero request.

## Timing and ISR Ordering

| Work | Timing and ownership |
| --- | --- |
| Joystick/control update | Timer1 overflow ISR every nominal 20 ms; both ADC axes and both commands always update |
| Servo signal | Timer1 hardware OC1A on D9; one position pulse per 20 ms frame |
| Motor PWM | Timer2 hardware on D3; faster PWM continues independently |
| Sensor Trigger | End of every fourth Timer1 frame; D4 HIGH for 10 us, giving 80 ms spacing |
| Echo capture/policy update | D2/INT0 CHANGE ISR timestamps recognized edges and evaluates completed duration |
| Serial telemetry | Foreground atomic snapshot and report followed by `delay(500)` |

The Timer1 overflow sequence is:

```text
expire any still-pending Echo attempt and restore full control
read A0 and A1
calculate requested motor PWM and servo command
apply the persistent distance policy to the request
publish control telemetry
on every fourth frame: arm Echo, pulse D4, finish the ISR
```

Trigger remains last so a near Echo has almost a full frame to complete and no joystick/control work is skipped. D2 accepts only the expected rising then falling edge. Unsigned subtraction handles `micros()` rollover. Neither ISR performs Serial output or floating-point conversion.

The one-frame acquisition window and the 80 ms trigger schedule serve different purposes. The short window expires an ambiguous attempt before another control update; 80 ms spacing exceeds the sensor's recommended 60 ms cycle interval. A 4 m Echo is about 23.2 ms and cannot complete in this deliberately shorter window. Lesson 07 covers advertised-range blocking measurement.

## Atomic Telemetry

The ATmega328P copies one byte at a time, while duration, ADC, and servo values span multiple bytes. `takeTelemetrySnapshot()` briefly prevents interruption while copying validity, Echo duration, requested/applied PWM, slowdown, ADC readings, and servo command as one consistent set. It restores the prior interrupt state before any unit conversion or Serial printing.

One approximately 500 ms report is a snapshot, not a record of every 20 ms control update or 80 ms sensor attempt. A completed policy change can affect the motor before the next printed line.

## Facilitate the Stationary-Target Core Mission

1. Require the complete power-off trace and immobile, secured, unloaded setup. Identify actuator motion areas and the external-power disconnect before power is applied.
2. USB-only, verify Build/Upload, 9,600-baud telemetry, repeated zero requested/applied commands at release, changing A0/A1 values, and a valid target reading.
3. With requested PWM zero, demonstrate invalid/no Echo by turning the target away. Ask what it proves. The correct answer is only that no valid measurement completed; it proves neither obstacle distance nor clearance. Point out full-control policy.
4. Place the target around 15 cm and clear all hands. Enable actuator power only after the gate passes. Have the student hold one brief moderate request and record requested = applied with slowdown OFF.
5. Release the joystick. Reposition the target to 5–8 cm, keep it stationary, and clear hands. Repeat approximately the same request and record applied ≈ requested/2 with slowdown ON.
6. Release before every target move. Do not ask students to create a no-Echo condition while commanding the powered motor.
7. If time permits, turn external actuator power off or keep request zero, position the target near 10 cm, and observe ON/OFF chatter without trying to eliminate it.
8. End with external power off first, Monitor closed, and USB removed.

Do not grade physical motor speed as exactly half. Supply/load behavior and starting friction make shaft motion nonlinear. Grade the labeled requested/applied telemetry and policy reasoning.

## Expected Telemetry

Far or boundary-valid result:

```text
Echo: valid | Echo delay: 0.870 ms | Distance: 15.0 cm | Motor ADC: 876 | Requested motor PWM: 175 | Applied motor PWM: 175 | Slowdown: OFF (full control) | Servo ADC: 512 | Servo pulse command (us): 1500
```

Valid near result:

```text
Echo: valid | Echo delay: 0.464 ms | Distance: 8.0 cm | Motor ADC: 876 | Requested motor PWM: 175 | Applied motor PWM: 87 | Slowdown: ON (50%) | Servo ADC: 512 | Servo pulse command (us): 1500
```

Invalid/missing result:

```text
Echo: invalid/no echo | Motor ADC: 876 | Requested motor PWM: 175 | Applied motor PWM: 175 | Slowdown: OFF (full control) | Servo ADC: 512 | Servo pulse command (us): 1500
```

Actual ADC values and commands vary. A report near a transition may reflect the latest internally consistent snapshot, not the target state at some earlier printed line.

## Threshold Chatter Expectations

Around 580 microseconds, normal measurement variation can alternate the raw comparison between near and non-near. Telemetry and applied PWM can therefore alternate between slowdown ON/half and OFF/full. This is expected, not proof of an interrupt failure. Lesson 09 intentionally omits averaging and hysteresis so students can see the single decision boundary.

Keep actuator power off or requested PWM zero for this observation. Filtering, hysteresis, debouncing, calibrated thresholds, and multiple-sensor safety logic are possible later design topics, not fixes to add during this lesson.

## Troubleshooting Sequence

1. **No/garbled Serial:** Select lesson 09, confirm Build/Upload, use 9,600 baud, and close competing port users.
2. **All Echo attempts invalid:** With both supplies removed, trace sensor labels to 5 V/D4/D2/ground. Then test USB-only with a broad rigid target at 5–20 cm. Inspect shifted headers, target angle, and loose returns.
3. **Near valid, far invalid:** Explain the one-frame acquisition bound. Use lesson 07 for full-range behavior; do not lengthen the window or skip joystick frames in this lesson.
4. **Near Echo but slowdown OFF:** Confirm the displayed delay is strictly below 0.580 ms. Exactly 0.580 ms is full control. Stabilize a broad target closer than 10 cm and compare the labeled fields.
5. **Slowdown ON but applied looks unexpected:** Divide the requested integer by two and round down. Confirm the report's requested/applied values rather than estimating shaft speed.
6. **Slowdown chatters near the boundary:** Expected raw-threshold behavior. Set request to zero or remove actuator power for observation; do not add filters within this lesson task.
7. **Invalid result restores full PWM:** Expected policy, but not a safe-clear indication. Release the joystick/remove actuator power and resolve sensing before continuing.
8. **Released motor request is nonzero:** Keep actuator power off. Record A0 neutral readings and verify joystick wiring before adjusting neutral or deadband constants.
9. **Servo binds/buzzes or motor does not move for a clear command:** Remove both supplies. Verify unloaded mechanics, pulse limits, external branch, MOSFET pins, gate network, shared ground, motor continuity, and diode polarity. Do not leave an actuator stalled.
10. **Resets or unstable readings during motion:** Remove both supplies. Check supply peak capability, distribution, returns, connector ratings, voltage at loads, transient capture, and local decoupling.
11. **Heat, odor, smoke, sparks, or unexpected motion:** Remove external power and USB immediately, isolate the setup, and resolve the cause before reuse.

## Code Map

- [`src/main.cpp`](src/main.cpp) owns Timer1 control scheduling, D2 acquisition, persistent policy state, atomic snapshots, and foreground reports.
- [`include/distance_motor_policy.h`](include/distance_motor_policy.h) contains the strict threshold and requested-to-applied pure functions.
- [`include/control_mapping.h`](include/control_mapping.h) preserves lesson 06 motor and servo mappings.
- [`include/ultrasonic_math.h`](include/ultrasonic_math.h) contains foreground unit conversion.
- [`test/test_distance_motor_policy.cpp`](test/test_distance_motor_policy.cpp) checks boundary, validity, and all PWM requests.
- [`test/test_control_mapping.cpp`](test/test_control_mapping.cpp) exhaustively checks all ADC inputs and mapping boundaries.
- [`test/test_ultrasonic_math.cpp`](test/test_ultrasonic_math.cpp) checks representative time/distance conversions.

## Verification

From `platformio-uno-lessons/09-distance-aware-motor-control`:

```bash
pio run
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson09-ultrasonic-check
/tmp/lesson09-ultrasonic-check
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_distance_motor_policy.cpp -o /tmp/lesson09-policy-check
/tmp/lesson09-policy-check
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_control_mapping.cpp -o /tmp/lesson09-mapping-check
/tmp/lesson09-mapping-check
```

Expected final results include PlatformIO `SUCCESS` and:

```text
Ultrasonic checks passed: zero, 10 cm, 50 cm, and 4,000 mm conversions.
Distance policy checks passed: 579/580/581 us, invalid readings, and all 256 PWM requests.
Mapping checks passed: neutral, boundaries, endpoints, clamps, and all 1024 ADC values.
```

Hardware verification is separate evidence: confirm valid-near, valid-far, and invalid reports; requested/applied policy behavior; zero on release; and safe operation of the secured rig. Record unperformed current, voltage, timing, or waveform measurements as not measured.
