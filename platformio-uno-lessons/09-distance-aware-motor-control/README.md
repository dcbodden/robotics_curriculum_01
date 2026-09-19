# Lesson 09: Distance-Aware Motor Control

Lesson 08 measured distance while joystick control continued. This lesson uses the same interrupt-driven HC-SR04 measurement to change the motor command. The joystick still produces a **requested motor PWM**, but the program may send a smaller **applied motor PWM** to D3:

- a valid Echo shorter than **580 microseconds** (strictly closer than about **10 cm**) applies 50% of the request;
- a valid Echo of 580 microseconds or longer applies the full request; and
- an invalid or missing Echo also applies the full request.

The invalid/no-Echo choice is for this secured classroom demonstration. It is **not fail-safe behavior**, does not prove the path is clear, and must not be used as collision avoidance or a safety system.

## Recommended Prior Learning

Complete [lesson 08: Interrupt Ultrasonic and Joystick Control](../08-interrupt-ultrasonic-joystick-control/README.md) first. Lesson 09 keeps its D2 Echo interrupt, 20 ms control frames, 80 ms Trigger spacing, actuator circuit, and joystick/servo mappings.

Teachers: prepare the circuit, power gates, threshold activity, and policy explanation with the [teacher guide](teacher-guide.md).

## What You Need

- Arduino Uno and USB data cable;
- four-pin, 5 V HC-SR04 labeled VCC, Trig, Echo, and GND;
- Elegoo joystick module labeled GND, +5V, VRx, VRy, and SW;
- unloaded SG90 position servo and secured bare 3–6 V yellow gearbox motor;
- RFP30N06LE MOSFET, 220–330 ohm gate resistor, 10 kilohm pulldown, and 1N5817 flywheel diode;
- teacher-approved regulated 5 V actuator supply and suitably rated wiring;
- breadboard, jumper wires, and secure supports; and
- a broad, rigid target such as a book or stiff cardboard.

## Core Mission: Compare Requested and Applied Motor Power

**Goal:** Keep the test rig immobile and compare the requested and applied PWM with a stationary target first beyond 10 cm and then closer than 10 cm.

**Prepare and stay safe:** Secure the bare motor with **no wheel, propeller, gear, or shaft attachment**. Secure the unloaded servo and sensor. Keep the motor, servo, and entire rig fixed in place. Move only the target, and only while the joystick is released and hands are outside both actuator motion areas. Remove **USB and external actuator power before every wiring or mechanical change**. Keep external +5 V separate from Arduino 5 V/VIN. A zero command is not an emergency power switch, and the motor can coast.

### 1. Wire and inspect with all power removed

With USB unplugged and external actuator power disconnected, build the [complete fixed circuit](#complete-power-off-wiring-reference). Follow the printed module labels and verified component terminals rather than guessing orientation.

**Teacher checkpoint:** Trace every row in the wiring tables. Confirm:

- HC-SR04 VCC → Arduino 5 V, Trig → D4, Echo → D2, and GND → shared ground;
- joystick +5V → Arduino 5 V, GND → shared ground, VRx → A0, and VRy → A1;
- servo orange → D9, red → external +5 V, and brown → shared ground;
- the protected D3/MOSFET motor path and diode polarity are correct;
- Arduino 5 V and external +5 V are not joined;
- every required ground is shared; and
- the motor and servo are secured, unloaded, and clear.

Do not restore power until the teacher approves the circuit.

### 2. Pass the USB-only gate

1. Leave the external actuator supply **off**. Connect USB and open `platformio-uno-lessons/09-distance-aware-motor-control` as the PlatformIO project. Under **Project Tasks > uno > General**, choose **Build** and wait for `SUCCESS`. Close Monitor, choose **Upload**, and wait for `SUCCESS`. Keep actuator power off during every upload.
2. Open **Monitor** at **9,600 baud**. Release the joystick and hold a flat target about 15 cm from the fixed sensor. A line should resemble:

   ```text
   Echo: valid | Echo delay: 0.870 ms | Distance: 15.0 cm | Motor ADC: 506 | Requested motor PWM: 0 | Applied motor PWM: 0 | Slowdown: OFF (full control) | Servo ADC: 512 | Servo pulse command (us): 1500
   ```

3. Confirm repeated released readings show requested and applied PWM of zero. Move each joystick axis gently and verify its telemetry changes. With actuator power off, no actuator can move.
4. With the motor request at zero, turn the target away briefly. `Echo: invalid/no echo` should appear with `Slowdown: OFF (full control)`. This means the program has no valid measurement and chooses full joystick authority; it does **not** mean zero distance or a clear path.

**Gate before actuator power:** Stop here unless the teacher sees a repeated zero motor request at release, matching requested/applied zero, changing A0/A1 readings, reasonable sensor output, stable USB operation, correct supply polarity/capacity, fixed wiring, and clear motion areas.

### 3. Observe full control with a stationary far target

1. Release the joystick. Place the target about 15 cm from the sensor, facing both transducers. Move your hand and target completely outside the actuator motion areas.
2. Without moving the target, the teacher enables external actuator power.
3. Move the motor axis to a moderate teacher-approved request and hold it briefly. Record a valid report. At or beyond 10 cm, `Slowdown` should be `OFF`, and requested and applied PWM should match.
4. Release the joystick before anyone reaches toward the setup.

### 4. Observe slowdown with a stationary near target

1. With the joystick released, move the target to about 5–8 cm from the sensor. Keep it broadside and stationary, then move your hand clear.
2. Apply approximately the same moderate motor request. A valid Echo below 580 microseconds should show `Slowdown: ON (50%)`; applied PWM should be half the requested value. Integer division rounds an odd request down, so 201 becomes 100.
3. Release the joystick and compare the observations.

| Stationary target | Echo delay (ms) | Distance (cm) | Requested PWM | Applied PWM | Slowdown |
| --- | ---: | ---: | ---: | ---: | --- |
| Beyond 10 cm | | | | | |
| Closer than 10 cm | | | | | |

**Notice:** The joystick request describes what the operator asked for. The applied value describes what the program sent to D3 after using the latest completed sensor result.

**Stop safely:** Release the joystick, turn off external actuator power first, close Monitor, then unplug USB. Remove both power sources before touching wiring, sensor position, motor, or servo.

## Name the Idea: Requested Versus Applied

Lesson 09 separates intent from output:

```text
joystick on A0 -> requested PWM -> distance policy -> applied PWM on D3
```

| Latest completed sensor result | Policy | Applied motor PWM |
| --- | --- | --- |
| Valid, Echo < 580 us | Slowdown ON | requested / 2 |
| Valid, Echo = 580 us | Slowdown OFF | full request |
| Valid, Echo > 580 us | Slowdown OFF | full request |
| Invalid or no Echo | Slowdown OFF | full request |

The comparison is strict: exactly 580 microseconds belongs to full control. A zero request stays zero, and applied PWM remains in the 0–255 range. PWM is a command, not a direct measurement of motor speed.

## What Happens Between Measurements

The program keeps lesson 08's timing:

| Rhythm | What happens |
| ---: | --- |
| 20 ms | Read A0/A1 and update motor and servo commands |
| 80 ms | After every fourth control frame, send a 10 us D4 Trigger pulse |
| Echo pulse | D2 interrupts capture the recognized rising and falling edges |
| ~500 ms | Print one atomic telemetry snapshot, then wait in the foreground |

The latest completed policy persists while the next measurement is in progress. A new valid near result can reduce the latest motor request immediately. A valid non-near result or a timeout at the next 20 ms frame restores full control. The program never skips a joystick/control frame to wait for Echo.

An attempt must complete both Echo edges before the next 20 ms control frame. An unfinished attempt becomes invalid, and later unmatched edges are ignored. This deliberately short window comfortably covers the 10 cm decision but not the HC-SR04's full advertised range. Use lesson 07 for full-range blocking measurements.

## Expect Chatter Near 10 cm

Small variations in Echo timing can put consecutive readings on opposite sides of 580 microseconds. Near 10 cm, `Slowdown` may alternate between `ON` and `OFF`, and applied PWM may alternate between half and full request. That is expected raw-threshold behavior in this lesson. The code intentionally adds no averaging, filtering, or hysteresis.

Do not use actuator motion to investigate threshold chatter. Keep the motor request at zero or external actuator power off, hold a target near 10 cm, and watch telemetry.

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

Trace original lead colors through extensions; connector orientation and extension colors can mislead.

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
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson09-ultrasonic-check
/tmp/lesson09-ultrasonic-check
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_distance_motor_policy.cpp -o /tmp/lesson09-policy-check
/tmp/lesson09-policy-check
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_control_mapping.cpp -o /tmp/lesson09-mapping-check
/tmp/lesson09-mapping-check
```

Expected host results:

```text
Ultrasonic checks passed: zero, 10 cm, 50 cm, and 4,000 mm conversions.
Distance policy checks passed: 579/580/581 us, invalid readings, and all 256 PWM requests.
Mapping checks passed: neutral, boundaries, endpoints, clamps, and all 1024 ADC values.
```

See the [host-check notes](test/README.md) for what each check covers.

## Troubleshooting and Stop Conditions

- **Nothing or garbled text in Monitor:** Keep actuator power off. Confirm lesson 09 is selected, Build and Upload succeed, Monitor is at 9,600 baud, and no other program owns the port.
- **Only `Echo: invalid/no echo`:** Full control is expected by policy, but the measurement is not trustworthy. Keep actuator power off, remove USB before tracing VCC → Arduino 5 V, Trig → D4, Echo → D2, and GND → shared ground. Then try a broad rigid target at 5–20 cm.
- **Invalid/no Echo makes the applied command rise:** This is the specified demonstration policy: invalid restores the full request. Release the joystick or remove external actuator power. Do not interpret the result as a clear path.
- **Valid near readings do not slow the motor:** Confirm Echo is below 0.580 ms and `Slowdown: ON (50%)`. Compare labeled requested and applied PWM, not apparent shaft speed. Keep the target stationary and broadside.
- **Slowdown switches rapidly near 10 cm:** Expected threshold chatter. Test with motor request zero or actuator power off; this lesson intentionally has no filter or hysteresis.
- **Applied PWM is one less than an exact half:** Integer division rounds odd requests down; for example, 255 becomes 127.
- **Released motor request is nonzero:** Keep actuator power off. Check joystick GND/+5V/VRx and ask the teacher to measure neutral before changing mapping constants.
- **Servo binds or buzzes, a clear high motor command does not turn the motor, or the Uno resets:** Turn off external power and unplug USB. Do not force or stall a shaft. Have the teacher inspect travel limits, shared ground, supply capacity, branch wiring, MOSFET stage, diode, and local decoupling.
- **Unexpected motion, unstable power, heat, unusual smell, smoke, or sparks:** Remove external power and USB immediately. Avoid hot parts and do not reconnect until the teacher resolves the cause.

This is the final lesson in the current PlatformIO Uno sequence. It demonstrates one raw sensor-informed policy on an immobile rig; it does not create an autonomous or safety-rated robot.
