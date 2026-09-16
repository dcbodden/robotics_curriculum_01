# Lesson 06 Teacher Guide: Two Controls, Frequent Checks

Use the [student lesson](README.md) to investigate independent motor-power and servo-position controls. The goal is that a robot must keep checking its controls and surroundings while carrying out instructions. This circuit reads a joystick; detecting obstacles would require another sensor.

Tell the student: “Every 20 milliseconds, the Arduino checks both controls and changes both commands. It keeps doing this while waiting to send the next computer message.” Reserve the timer details below for teacher preparation.

## Verify the Actual Hardware and Supply

Use regulated external **5 V DC** for the documented 3–6 V motor and servo, after confirming that the actual kit servo permits this voltage. USB powers the Uno; Arduino 5 V powers only the joystick. External positive must remain separate from Arduino 5 V and VIN. Keep B1 out of lesson 06.

[TowerPro's analog SG90 page](https://towerpro.com.tw/product/sg90-analog/) lists 4.8 V operation and external power. Its [digital SG90 page](https://towerpro.com.tw/product/sg90-7/) includes a manufacturer response allowing 4.8–6 V for that model. This does not establish a 6 V maximum for an Elegoo kit unit. Record the exact servo's approved range from supplier documentation before powering it. Resolve missing specifications with the supplier rather than increasing voltage to cure erratic movement.

Obtain motor starting/stall current and servo peak/stall current at the intended voltage from documentation for the actual parts or appropriate supervised engineering measurements. Torque is not current, and unloaded running current cannot establish stall demand. Do not deliberately stall either actuator during the student activity.

Choose a regulated supply covering their combined peak demand with suitable margin. Check transient capability, current-limit behavior, and wire, connector, distribution, MOSFET, and diode ratings. Do not infer a current rating from “SG90” or assume a kit breadboard power module can support both actuators. Resolve missing peak-current information before the activity.

Record evidence for this setup. Mark unperformed measurements **not measured**.

| Preparation item | Value and source/method |
| --- | --- |
| Servo identification and approved voltage range | |
| Motor identification and approved voltage range | |
| Motor starting/stall current at 5 V | |
| Servo peak/stall current at 5 V | |
| Combined peak demand and chosen supply margin | |
| Supply model, regulated voltage, rated current, and current limit | |
| Branch wire/connector ratings and distribution arrangement | |
| Capacitor values, voltage ratings, polarity, and locations | |
| Voltage at supply, servo connector, and motor branch: idle | |
| Voltage at those points: motor starting while servo moves | |
| Minimum captured voltage, instrument, and observed stability | |
| Teacher/date and unresolved checks | |

Measure polarity and voltage before connecting actuators. During approved unloaded movement, check voltage at the load connectors, including simultaneous motor start and servo movement. Compare with the actual parts' approved ranges and watch for resets, chatter, heat, or current limiting. A multimeter may miss short dips; use a scope or suitable capture instrument when transient stability remains uncertain. Working motion does not establish combined peak-current capacity.

## Route Power and Add Local Decoupling

Remove USB and actuator power before wiring changes. Establish a suitably rated actuator-power distribution point with separate positive and return branches for the servo and motor stage. Keep branches short. Motor return runs through MOSFET Source to actuator negative; servo brown returns directly to actuator negative. Avoid carrying actuator current through the Uno ground jumper or joystick return wiring. Connect Arduino GND to the common ground reference so D3 and D9 have a defined reference. Verify continuity across split breadboard rails.

Place suitable local decoupling across external positive and ground near the servo connector and motor-stage supply connection. Select ceramic bypass and bulk capacitance using supply/actuator guidance and measured behavior. Keep leads short; verify electrolytic polarity and voltage ratings above the maximum supply voltage, including transients. Capacitors help with disturbances but do not replace adequate supply capacity or wiring. [Arduino's servo guidance](https://docs.arduino.cc/learn/electronics/servo-motors/) discusses power demand and capacitor support.

Retain the protected motor stage in the [student wiring instructions](README.md#connect-the-protected-dc-motor-stage): D3 through 220–330 ohms to RFP30N06LE Gate, 10 kilohms from Gate to ground, Source to ground, and Drain to motor negative. The 1N5817 banded cathode goes to motor positive; its unbanded anode goes to Drain. Local capacitors do not replace this diode.

## Trace Orange, Red, and Brown

| Actual servo lead | Connection |
| --- | --- |
| Orange | Arduino D9 signal |
| Red | External regulated +5 V, separate from Arduino 5 V/VIN |
| Brown | External negative/shared ground, never MOSFET Drain |

Trace original servo leads through extensions. Extension colors and connector left/right orientation cannot replace terminal identification. Confirm the actual unit's connector documentation. [Arduino's connection guide](https://docs.arduino.cc/learn/electronics/servo-motors/) explains signal, power, and ground functions.

A missing Arduino-to-actuator ground can produce a stopped motor and erratic servo even when positive and signal wires look correct. With all power removed, check continuity among actuator negative, servo brown, MOSFET Source, Arduino GND, and joystick GND. [Arduino troubleshooting guidance](https://support.arduino.cc/hc/en-us/articles/360017053760-Troubleshoot-servo-motors) recommends external servo power with shared ground.

## Check Neutral and Adjust Travel

Secure a bare motor without a wheel or propeller and an unloaded servo with a clear motion area. Keep actuator power off during uploads and initial serial checks. Select lesson 06's project and a 9600-baud monitor.

The [mapping constants](include/control_mapping.h) start with `MOTOR_NEUTRAL_ADC = 512` and `MOTOR_DEADBAND_ADC = 40`. Release the joystick repeatedly and record A0. The nominal neutral band is 472–552; all readings through 552, including inactive travel, command zero. Increasing A0 beyond that edge increases power toward 255; very small increases can still round to zero. Module orientation determines the physical active direction.

If needed, set `MOTOR_NEUTRAL_ADC` to the measured center and choose `MOTOR_DEADBAND_ADC` to contain repeated release variation. Keep the band inside 0–1023 with its upper edge below 1023. Rebuild/upload with actuator power off, then verify zero command on repeated release and inactive travel before enabling power. **506 can be an ADC reading; motor PWM is 0–255.** Center-off is software behavior, not a power interlock; a motor can coast after a zero command.

Servo constants start at `SERVO_MIN_PULSE_US = 1000`, `SERVO_CENTER_PULSE_US = 1500`, and `SERVO_MAX_PULSE_US = 2000`. `SERVO_CENTER_ADC = 512` maps to the middle pulse; adjust this ADC constant if measured A1 neutral requires it. These are position commands, not guaranteed angles or universally safe endpoints.

Begin with small unloaded movements around center. For binding or sustained buzzing, remove both supplies and narrow the affected pulse limit toward center before rebuilding/uploading. Maintain minimum < center < maximum and keep maximum well below the 20,000-microsecond frame. If center itself is unsuitable, resolve calibration or mechanical condition before continuing. Never force the horn or repeatedly drive into an end stop to seek an advertised angle. Remove all power before mechanical changes.

## Interpret the Four Timing Rhythms

| Work | Timing and meaning |
| --- | --- |
| Control ISR | Both axes read and both commands updated once per nominal 20 ms frame: 50 checks/second |
| Servo D9 signal | Timer1 hardware repeats a position pulse every 20 ms; pulse length represents position |
| Motor D3 signal | Timer2 hardware runs separate, faster motor PWM; the command sets duty cycle |
| Serial report | Foreground copies a consistent snapshot, prints it, and waits about 1 second |

Timer1 uses Fast PWM mode 14, prescaler 8, and ICR1 = 39999 on the 16 MHz Uno. Ticks are 0.5 microseconds, giving `(39999 + 1) × 0.5 microseconds = 20 milliseconds`. OC1A drives D9 directly. The overflow ISR discards one settling conversion per ADC channel, retains another reading, calculates commands, updates D3 and OCR1A, and publishes telemetry. It does no serial printing, delay, or software servo-pulse wait. ADC reads themselves wait for conversion, making this a bounded teaching trade-off rather than a general recommendation for lengthy ISR work.

OCR1A is buffered: an ISR command can take effect in the following frame rather than changing the pulse already underway. Sampling, frame transfer, and mechanical travel contribute to response. Do not promise that the servo reaches its new position within 20 ms. Timer0 supports Arduino timing; Timer2 continues motor PWM. The short atomic snapshot restores interrupts before printing, so the foreground wait does not stop checks.

The [ATmega328P datasheet](https://ww1.microchip.com/downloads/en/devicedoc/atmel-7810-automotive-microcontrollers-atmega328p_datasheet.pdf), sections 15.9.3 and 15.11, documents Fast PWM, timer interrupts, and buffered compare updates. Sections 23.4 and 23.6.1 cover ADC timing/input guidance. Calculations explain intended operation; measured waveforms are separate evidence.

### How This Differs from the Servo Library

On Uno, the [Arduino Servo implementation](https://github.com/arduino-libraries/Servo/blob/master/src/avr/Servo.cpp) owns Timer1 and schedules compare interrupts at pulse boundaries, switching servo pins in software. Its [header](https://github.com/arduino-libraries/Servo/blob/master/src/Servo.h) defines a nominal 20,000-microsecond refresh interval. A refresh frame is not one periodic control ISR every 20 ms.

Lesson 06 gives hardware the pulse edges and uses one overflow ISR per frame for both control commands. Thus the ADC reads do not determine servo pulse edges. Adding `Servo.attach()` would reconfigure the same timer and conflict. Do not use `analogWrite()` on Timer1 pins D9/D10 alongside this configuration. Lesson 05's separate project retains its 100 ms Timer1 controller.

## Guide the Investigation

Follow the student's [four gated Core Missions](README.md#four-gated-core-missions-one-approved-circuit): power-off inputs, servo axis only, motor center-off, then both together and movement between reports. The approved circuit stays fixed; “servo only” and “motor only” describe joystick movement, not separate supply or circuit rebuilds. Enable actuator power only after neutral/wiring checks. Stop and remove both supplies for binding, sustained buzzing, unexpected heat, resets, or a commanded motor that fails to turn.

Look for evidence connecting A0 to motor command, A1 to servo command, zero on motor release/inactive travel, and independent diagonal control. Starting thresholds, coasting, servo travel time, and serial snapshots affect observations. Accept “hard to tell” with an explanation rather than requiring a visible timing difference.

Ask why a moving robot should check again before its next computer message. An answer should connect frequent checks with quicker steering or stopping decisions. This joystick circuit does not guarantee collision avoidance. Record physical, waveform, and power measurements separately from successful builds and qualitative motion; leave unperformed checks explicit.

## Detailed Diagnosis

Remove **external actuator power and USB** before checking any wiring, connector, component, or mechanical setup. Keep actuator power off during any rebuild or upload. Record the observed symptom rather than assuming a firmware error.

- **Released motor command is nonzero:** Record repeated A0 neutral readings with actuator power off. Check joystick GND, +5V, and VRx → A0. Adjust `MOTOR_NEUTRAL_ADC` or `MOTOR_DEADBAND_ADC` only after confirming the real neutral variation, then rebuild/upload with external power off and repeat the release check before enabling actuators.
- **Servo chatters while motor does not move:** First check continuity from Arduino GND to external negative, servo brown, joystick GND, and MOSFET Source. Check split breadboard rails and loose returns. Trace servo orange → D9 and red → external +5 V; verify actual connector polarity and supply behavior. A missing shared ground can produce both symptoms even when positive and signal wires appear correct.
- **Motor ADC never changes or command stays zero in both directions:** Check joystick orientation and VRx → A0 while all sources are off. After approved USB-only input checks, move in the direction that raises A0 past the neutral band. Very small changes may round to zero.
- **Motor command is high but motor never turns:** Remove both sources promptly. Inspect the regulated motor-positive branch, D3 → series gate resistor → RFP30N06LE Gate, gate pulldown, Drain/Source wiring, shared ground, motor connections, and 1N5817 polarity. Do not leave a powered motor stalled while diagnosing it.
- **Very low nonzero command does not start the motor:** A starting threshold can be normal. After verifying power and protection, compare a clearly larger command without touching or blocking the shaft. Stop if it still fails at a high command.
- **Servo binds or persistently buzzes:** Remove both sources. Check the servo is unloaded and secured, its travel area clear, connector colors correct, and supply stable. Narrow `SERVO_MIN_PULSE_US` or `SERVO_MAX_PULSE_US` toward center if needed; rebuild/upload with actuator power off and recheck small movements. Do not widen travel or force the shaft to seek an advertised angle.
- **Readings jump, servo chatters, or Uno resets during motion:** Remove both sources. Check joystick connection and common ground; verify external supply capacity, connector polarity, branch ratings, voltage at both loads, and transient stability. Review decoupling separately; capacitors cannot compensate for inadequate supply capacity or wiring.
- **Hot parts, unusual smell, smoke, sparks, or unexpected motion:** Remove external power and USB immediately, avoid hot parts, inspect the setup, and do not resume until the cause is resolved.
