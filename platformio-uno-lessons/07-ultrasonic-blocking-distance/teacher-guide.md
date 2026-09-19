# Lesson 07 Teacher Guide: Blocking Ultrasonic Distance

Use this guide to prepare and interpret the HC-SR04 demonstration. Students can complete the Core Mission in the [lesson README](README.md) without learning the implementation details here.

## Learning Focus

Students should be able to:

- identify Trigger as an Arduino output and Echo as an Arduino input;
- connect a longer Echo HIGH time with a farther target;
- explain that the sound makes a round trip while the reported distance is one way;
- distinguish a fractional-millisecond measurement from the 500 ms program delay; and
- recognize that a missing Echo is invalid data, not a zero-centimeter reading.

`pulseIn`, floating-point formatting, the speed-of-sound approximation, and timeout internals are supporting mechanics rather than required student mastery.

## Equipment and Preparation

- Arduino Uno and known-good USB data cable;
- four-pin 5 V HC-SR04 with readable VCC, Trig, Echo, and GND labels;
- secure jumper connections and a stable sensor support;
- a broad, rigid target such as a book or stiff cardboard; and
- a ruler or tape measure for approximate placement.

Before class, inspect the module for bent pins or damage and confirm the label order on the actual unit. HC-SR04 boards and clones can look similar while connector orientation differs. Follow labels rather than a memorized left-to-right order.

Build the project and run the host check using the commands under [Verification](#verification). If practical, test one target between 10 and 30 cm and one between 40 and 100 cm. The demonstration does not require agreement to the millimeter.

## Required Power-Off Check

Keep USB unplugged while wiring or inspecting connections.

| HC-SR04 | Uno | Check |
| --- | --- | --- |
| VCC | 5 V | The sensor uses the Uno's regulated 5 V rail |
| Trig | D4 | Arduino output that begins a measurement |
| Echo | D2/INT0 | 5 V-compatible Uno input for direct pulse measurement in lesson 07 |
| GND | GND | Shared reference and return path |

Confirm that no connection bridges 5 V directly to GND, no header is shifted by one position, and the sensor is mechanically stable. Lesson 07 does not use an external supply, joystick, servo, motor, MOSFET, or actuator wiring. Those lesson 06 parts should be disconnected and set aside.

Students move only the target after power is restored. They should not handle wires, lift the sensor, place it against a person, or point it directly into someone's ear. Although the activity contains no actuator motion, immediately unplug USB for heat, unusual smell, smoke, sparks, damaged wiring, or unstable behavior.

## Timing and Distance Reference

The module is triggered with a HIGH pulse of 10 microseconds. Its Echo HIGH interval represents acoustic travel to the target and back. The common module approximation is:

```text
distance_cm = echo_duration_us / 58
round_trip_ms = echo_duration_us / 1000
```

Representative idealized values:

| One-way distance | Approximate Echo duration | Displayed round-trip time |
| ---: | ---: | ---: |
| 2 cm | 116 us | 0.116 ms |
| 10 cm | 580 us | 0.580 ms |
| 50 cm | 2,900 us | 2.900 ms |
| 100 cm | 5,800 us | 5.800 ms |
| 400 cm | 23,200 us | 23.200 ms |

The firmware uses a 25,000 us timeout, slightly beyond the advertised 4 m timing. A return value of zero is treated as invalid/no Echo and is never converted into a reported zero distance.

The program waits in two places: first during the bounded Echo measurement, then during `delay(500)`. Reports are therefore approximately, not exactly, 500 ms apart. This is the intended blocking baseline for comparison with the later interrupt lesson.

The HC-SR04 documentation commonly recommends more than 60 ms between triggers. Even a maximum-length valid lesson 07 measurement plus its 500 ms delay leaves substantially more than that interval before the next trigger.

## Facilitate the Core Mission

1. Require the power-off wiring trace before USB is connected.
2. Have the student keep one target material and orientation for both trials so distance is the main planned change.
3. Start near 10 cm and far near 50 cm, but allow different positions if the available workspace requires them.
4. Ask the student to copy one representative line at each position rather than claiming every reading will match.
5. Ask which changed together: target distance, Echo delay, and calculated centimeters.
6. Use `580 us → 0.580 ms → about 10 cm` as the compact round-trip example.

Accept measurements that vary modestly or differ from ruler distance. A good conclusion is based on the observed direction of change and acknowledges variation. If no valid far reading is available, a student can still document the invalid outcome and explain how target material, angle, range, or wiring might affect reflection.

## Keep the Explanation Accurate

- The module measures pulse duration; the program calculates distance.
- Echo duration represents the outward and return path, so distance uses half the total acoustic travel.
- The divisor of 58 is an approximation based on the speed of sound. Temperature and air conditions affect that speed.
- Reported centimeters are not evidence of the module's advertised millimeter-scale accuracy on this particular setup.
- An invalid result does not distinguish among no target, weak reflection, out-of-range target, incorrect wiring, or a failed sensor.
- This stationary activity does not demonstrate collision avoidance or guarantee that an object is absent.

## Code Map for Teachers

- [`include/ultrasonic_math.h`](include/ultrasonic_math.h) contains the 23,200 us advertised-range reference, 25,000 us timeout, validity check, and unit conversions.
- [`src/main.cpp`](src/main.cpp) configures D4/D2, emits the 10 us Trigger, calls the bounded pulse measurement, formats valid or invalid output, and delays 500 ms.
- [`test/test_ultrasonic_math.cpp`](test/test_ultrasonic_math.cpp) checks invalid boundaries, 10 cm, and the 4,000 mm reference without Arduino hardware.

Students should focus on the input/output/timing chain rather than memorize helper implementation.

## Troubleshooting and Stop Conditions

1. **No Serial output:** Verify the correct lesson directory is open, Build and Upload succeed, Monitor uses 9,600 baud, and no other process owns the port.
2. **Only invalid results:** Try a rigid target at 10–30 cm. With USB removed, trace the four labeled connections, including Echo → D2/INT0, and inspect for a shifted header.
3. **Implausibly short result:** Clear nearby reflecting objects and check that the target is in front of both transducers.
4. **Intermittent or long result:** Square a broader target to the sensor and reduce its distance. Cloth and angled surfaces can scatter sound.
5. **Unstable values:** Secure the sensor, target, USB cable, and jumpers; then reduce movement around the measurement path.
6. **Heat, odor, smoke, or sparking:** Disconnect USB immediately, keep students from touching the hardware, and inspect the supply connections before replacing or reconnecting parts.

## Verification

From `platformio-uno-lessons/07-ultrasonic-blocking-distance`, build the firmware:

```bash
pio run
```

Run the deterministic host check:

```bash
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson07-ultrasonic-check
/tmp/lesson07-ultrasonic-check
```

Expected final lines include PlatformIO `SUCCESS` and:

```text
Ultrasonic checks passed: validity, time, 10 cm, and 4,000 mm range.
```

Actual-hardware verification should confirm labeled output for a near and farther rigid target plus explicit invalid/no-Echo output when no complete pulse arrives before 25 ms. Hardware observations remain separate from the automated build and host checks.
