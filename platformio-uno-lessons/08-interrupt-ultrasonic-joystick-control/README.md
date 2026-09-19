# Lesson 08: Interrupt Ultrasonic and Joystick Control

Lesson 07 waited for each ultrasonic measurement. This lesson adds the four-pin 5 V HC-SR04 to lesson 06's joystick, motor, and servo circuit, then uses an interrupt on **D2/INT0** to capture Echo edges. The Arduino still reads both joystick axes and updates both actuator commands every **20 milliseconds**.

Three rhythms happen together:

- every **20 ms**, the program reads A0/A1 and updates the motor and servo commands;
- every **80 ms**, after the fourth control update, D4 sends a 10 microsecond Trigger pulse; and
- approximately every **500 ms**, the foreground prints one snapshot to Serial Monitor.

The half-second foreground wait does not stop the timer or Echo interrupts. A report is one snapshot, not every control update or sensor attempt.

## Recommended Prior Learning

Complete [lesson 07: Blocking Ultrasonic Distance](../07-ultrasonic-blocking-distance/README.md) first. Lesson 08 also reuses the complete protected actuator circuit and control mappings from [lesson 06: Joystick Motor and Servo Control](../06-joystick-motor-servo-control/README.md).

Teachers: prepare the circuit, power system, timing explanation, and gates with the [teacher guide](teacher-guide.md).

## What You Need

- Arduino Uno and USB data cable;
- four-pin, 5 V HC-SR04 labeled VCC, Trig, Echo, and GND;
- Elegoo joystick module labeled GND, +5V, VRx, VRy, and SW;
- unloaded SG90 position servo and secured bare 3–6 V yellow gearbox motor;
- RFP30N06LE MOSFET, 220–330 ohm gate resistor, 10 kilohm pulldown, and 1N5817 flywheel diode;
- teacher-approved regulated 5 V actuator supply and suitably rated wiring;
- breadboard, jumper wires, and secure supports; and
- a broad, rigid, movable target such as a book or stiff cardboard.

## Core Mission: Observe Sensing and Control Together

**Goal:** Compare a near and farther target while confirming that joystick motor and servo commands keep changing between the approximately 500 ms reports.

**Prepare and stay safe:** The rig remains immobile. Secure the bare motor with **no wheel, propeller, gear, or shaft attachment** and secure the unloaded servo with a clear motion area. Move only the stationary target; never move the rig toward an obstacle. Keep hands, hair, clothing, target, and other objects out of both actuator motion areas. Remove **USB and external actuator power before every wiring or mechanical change**. Keep external +5 V separate from Arduino 5 V/VIN. A zero command is not an emergency power switch, and a motor can coast. This is a timing demonstration, not a collision-avoidance or safety system.

### 1. Wire and inspect with all power removed

With USB unplugged and external actuator power disconnected, build the [complete fixed circuit](#complete-power-off-wiring-reference). Follow printed module labels and verified component terminals rather than guessing orientation.

**Teacher checkpoint:** Trace every row in the wiring tables. Confirm:

- HC-SR04 VCC → Arduino 5 V, Trig → D4, Echo → D2, and GND → shared ground;
- joystick +5V → Arduino 5 V, GND → shared ground, VRx → A0, and VRy → A1;
- servo orange → D9, red → external +5 V, and brown → shared ground;
- the protected D3/MOSFET motor path and diode polarity are correct;
- Arduino 5 V and external +5 V are not joined;
- every required ground is shared; and
- the motor and servo are secured, unloaded, and clear.

Do not restore power until the teacher approves the circuit.

### 2. Check inputs and sensing with actuator power off

1. Leave the external actuator supply **off**. Connect USB and open `platformio-uno-lessons/08-interrupt-ultrasonic-joystick-control` as the PlatformIO project. Under **Project Tasks > uno > General**, choose **Build** and wait for `SUCCESS`. Close Monitor, choose **Upload**, and wait for `SUCCESS`. Keep actuator power off during every upload.
2. Open **Monitor** at **9,600 baud**. Hold a flat target about 10–30 cm in front of the fixed sensor. A line should resemble:

   ```text
   Echo: valid | Echo delay: 0.580 ms | Distance: 10.0 cm | Motor ADC: 506 | Motor PWM command: 0 | Servo ADC: 512 | Servo pulse command (us): 1500
   ```

   Your readings will differ. `Echo: invalid/no echo` is an explicit missing or expired measurement, not zero centimeters.
3. Release the joystick repeatedly. Confirm **Motor PWM command: 0**. Move each axis gently and verify that Motor ADC/Motor PWM and Servo ADC/Servo pulse command change as they did in lesson 06. With actuator power off, no actuator can move.

**Gate before actuator power:** Stop here unless the teacher sees repeated zero motor commands at release, changing A0/A1 readings, reasonable sensor output, stable USB operation, correct supply polarity/capacity, fixed wiring, and clear actuator motion areas.

### 3. Compare target distances while control continues

1. Return the joystick to center. Without moving wires, the teacher enables the external actuator supply. The servo may move to its middle command immediately.
2. Place the target about 15 cm in front of the sensor, then move your hand out of all actuator motion areas. Record one valid line. Move only the servo axis a small approved amount and return it to center. The servo can begin responding before the next report because control updates occur every 20 ms.
3. With the target still fixed, move the motor axis briefly in its active direction to a moderate approved command, then release it. Observe from a safe distance. The motor can respond and return to a zero command between two 500 ms reports; a report only shows the state at its snapshot time.
4. Release the joystick. Move the same target to about 50 cm, keep it facing the sensor, and move your hand clear. Record another valid line. The farther target should usually show a longer Echo delay and larger distance.

| Target | Echo delay (ms) | Distance (cm) | Motor command tried | Servo command tried | Response before next report? |
| --- | ---: | ---: | ---: | ---: | --- |
| Near | | | | | Yes / No / Hard to tell |
| Far | | | | | Yes / No / Hard to tell |

**Notice:** Which changed more often: the control commands, sensor attempts, or printed reports? What evidence showed that control continued during the foreground wait?

**Stop safely:** Release the joystick, turn off external actuator power first, close Monitor, then unplug USB. Remove both power sources before touching wiring, sensor position, motor, or servo.

## Name the Idea: Interrupts Share Time

The HC-SR04 Echo pin changes from LOW to HIGH and later HIGH to LOW. D2's external interrupt notices both edges:

```text
rising edge  -> remember the start time
falling edge -> subtract start time and publish a valid duration
```

The subtraction gives the Echo HIGH duration even if Arduino's microsecond counter rolls over between the two edges. No Serial output or floating-point distance calculation occurs inside the Echo interrupt. The foreground later converts the captured duration:

```text
round-trip milliseconds = Echo microseconds / 1000
one-way centimeters     = Echo microseconds / 58
```

The distance divides the acoustic round trip into a one-way result. For example, 580 microseconds is 0.580 ms round trip and approximately 10 cm one way.

## Compare the 20, 80, and 500 ms Rhythms

| Rhythm | What happens |
| ---: | --- |
| 20 ms | Read both joystick axes; update D3 motor PWM and the D9 servo command |
| 80 ms | After every fourth control frame, arm Echo capture and pulse D4 Trigger |
| Echo pulse | D2 interrupts timestamp the recognized rising and falling edges |
| ~500 ms | Foreground atomically copies one snapshot, prints it, then waits |

`delay(500)` pauses only the foreground. Timer1 and D2 interrupts continue during that delay, so joystick control and ranging continue. D3's faster PWM is generated by Timer2, and D9's servo signal is generated by Timer1 hardware.

## Why the Echo Window Is Deliberately Short

Lesson 08 gives a sensor attempt until the **next 20 ms control frame** to complete both Echo edges. At that next frame, any unfinished attempt expires as invalid before the normal joystick work runs. Later unmatched edges are ignored. The program never skips a joystick cycle to wait for a long Echo.

This is intentionally shorter than lesson 07's full-range wait. A 4 m target corresponds to about 23.2 ms of Echo time and therefore cannot complete inside lesson 08's one-frame window. Even somewhat shorter readings may expire because the usable time is slightly less than a full frame. Use lesson 07 for the advertised-range investigation; lesson 08 emphasizes continued control and comfortably covers the near distances needed by the next lesson.

The 80 ms interval between Trigger pulses is separate from this validity window. It satisfies the more-than-60-ms sensor spacing while allowing a failed attempt to expire promptly.

## Why the Snapshot Is Atomic

An Uno processes one byte at a time, but the Echo duration and several telemetry fields use more than one byte. An interrupt could otherwise change part of a value while the foreground copies it. The program briefly prevents interruption only while copying one consistent snapshot, then restores interrupts **before** converting units or printing. This avoids mixed-byte telemetry without pausing control throughout slow Serial output.

## Complete Power-Off Wiring Reference

### Keep the two positive supplies separate

| Power connection | Destination |
| --- | --- |
| USB / Arduino electronics | Uno, Arduino 5 V rail |
| Arduino 5 V | Joystick +5V and HC-SR04 VCC only |
| External regulated +5 V | Servo red and DC motor positive, on separate rated branches |
| External negative | Shared ground |
| Arduino GND | Shared ground |

Arduino GND, external negative, joystick GND, HC-SR04 GND, servo brown, and MOSFET Source share the same reference. Never connect external +5 V to Arduino 5 V, VIN, a signal pin, the joystick, or the sensor.

### HC-SR04 and joystick

| Part label | Connect to | Purpose |
| --- | --- | --- |
| HC-SR04 VCC | Arduino 5 V | Sensor power |
| HC-SR04 Trig | Arduino D4 | 10 us measurement request |
| HC-SR04 Echo | Arduino D2 / INT0 | Interrupt pulse input |
| HC-SR04 GND | Shared ground | Sensor return/reference |
| Joystick +5V | Arduino 5 V | Joystick power |
| Joystick GND | Shared ground | Joystick reference |
| Joystick VRx | Arduino A0 | Center-off motor request |
| Joystick VRy | Arduino A1 | Servo position request |
| Joystick SW | Leave disconnected | Button unused |

### Servo

Trace the original lead colors through any extensions; connector orientation and extension colors can mislead.

| Servo wire | Connect to |
| --- | --- |
| Orange, signal | Arduino D9 |
| Red, power | External regulated +5 V |
| Brown, return | Shared ground/external negative |

Servo brown connects directly to shared ground, never to the motor's switched Drain.

### Protected DC motor stage

Verify the RFP30N06LE Gate, Drain, and Source terminals before connecting.

| From | To |
| --- | --- |
| External +5 V | Motor positive |
| Motor negative | MOSFET Drain, pin 2 |
| MOSFET Source, pin 3 | Shared ground |
| Arduino D3 | 220–330 ohm resistor, then MOSFET Gate, pin 1 |
| MOSFET Gate, pin 1 | 10 kilohm pulldown, then shared ground |
| 1N5817 banded cathode | Motor positive/external +5 V |
| 1N5817 unbanded anode | Motor negative/MOSFET Drain |

Place the diode near the motor connections. Reversing it can short the actuator supply when the MOSFET turns on.

## Build and Host Checks

From this lesson directory:

```bash
pio run
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson08-ultrasonic-check
/tmp/lesson08-ultrasonic-check
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_control_mapping.cpp -o /tmp/lesson08-mapping-check
/tmp/lesson08-mapping-check
```

Expected host results:

```text
Ultrasonic checks passed: zero, 10 cm, 50 cm, and 4,000 mm conversions.
Mapping checks passed: neutral, boundaries, endpoints, clamps, and all 1024 ADC values.
```

The 4,000 mm host case verifies conversion math, not lesson 08's shorter interrupt acquisition window. See the [host-check notes](test/README.md).

## Troubleshooting and Stop Conditions

- **Nothing or garbled text in Monitor:** Keep actuator power off. Confirm lesson 08 is the selected project, Build and Upload succeed, Monitor is at 9,600 baud, and no other program owns the port.
- **Only `Echo: invalid/no echo`:** Keep the circuit powered down while inspecting. Try a broad rigid target at 10–30 cm, then remove both supplies and trace VCC → Arduino 5 V, Trig → D4, Echo → D2, and GND → shared ground. Check for shifted connectors and loose wires.
- **Short targets work but far targets become invalid:** This can be expected. Lesson 08 expires any Echo not completed before the next 20 ms frame; use lesson 07 for full-range measurements.
- **Distance jumps or is implausibly short:** Secure the sensor and target, face a broad rigid target toward both transducers, and clear nearby reflecting objects.
- **Released motor command is nonzero:** Keep actuator power off. Check joystick GND/+5V/VRx and ask the teacher to measure neutral before changing mapping constants.
- **Servo binds or buzzes, a clear high motor command does not turn the motor, or the Uno resets:** Turn off external power and unplug USB. Do not force or stall a shaft. Have the teacher inspect travel limits, shared ground, supply capacity, branch wiring, MOSFET stage, diode, and local decoupling.
- **Unexpected motion, unstable power, heat, unusual smell, smoke, or sparks:** Remove external power and USB immediately. Avoid hot parts and do not reconnect until the teacher resolves the cause.

Next, continue to [lesson 09: Distance-Aware Motor Control](../09-distance-aware-motor-control/) to use the same interrupt measurement for a distance-aware motor slowdown.
