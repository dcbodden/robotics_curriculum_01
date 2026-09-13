# Lesson 06: Joystick Motor and Servo Control

A joystick gives us two controls in one handle. One direction controls how much power a DC motor receives. The other controls the position of a servo. A **servo** is a motor that moves toward a chosen position rather than just spinning continuously.

Our program checks both joystick directions every **20 milliseconds**, or **50 times per second**. Like lesson 05, it keeps checking while the main loop waits between computer messages. The motor control is **center-off**: releasing the joystick commands zero motor power. Pushing in one direction increases power; pushing the opposite way keeps power off. The servo control moves between two position limits, with a middle command when centered.

## Before You Begin

Complete [lesson 05](../05-interrupt-driven-motor-control/README.md) with your teacher. You should recognize analog inputs, motor power commands, and timer reminders. Your teacher handles power-supply selection and verifies the circuit before power is applied.

Teachers: use the [preparation and timing guide](teacher-guide.md) for actual-hardware power checks, orange/red/brown servo connections, and neutral/travel adjustments.

This lesson uses the kit joystick instead of the RV snap, and an external regulated 5 V supply instead of B1. Keep B1 and RV out of this circuit.

## What You Need

- Arduino Uno and a USB data cable;
- Elegoo joystick module with GND, +5V, VRx, VRy, and SW labels;
- SG90 **position servo** with orange, red, and brown wires;
- The documented 3–6 V yellow gearbox DC motor;
- RFP30N06LE MOSFET, 220–330 ohm gate resistor, 10 kilohm pulldown resistor, and 1N5817 flywheel diode;
- Breadboard, suitable jumper wires, and secure supports for the motor and unloaded servo;
- Teacher-approved **regulated 5 V DC actuator supply**, connector, and wiring rated for the combined motor and servo current.

## Two Power Paths

An **actuator** makes something move. Here, the servo and DC motor are our actuators. USB powers the Uno, and the Uno's 5 V pin powers the joystick. The separate actuator supply powers both motors.

| Power connection | Where it goes |
| --- | --- |
| USB cable | Uno USB socket |
| Arduino 5 V | Joystick +5V only |
| External regulated +5 V | Servo red wire and DC motor positive terminal |
| External negative (0 V) | Shared ground |
| Arduino GND | Shared ground |

**Keep the two positive supplies separate, even though both are called 5 V.** Do not connect external +5 V to Arduino 5 V, VIN, an input/output pin, or the joystick. Ground is their shared reference: Arduino GND, external negative, joystick GND, servo brown, and MOSFET Source all join it.

Use separate, suitably rated branches from the actuator supply for the servo and motor. The teacher must check breadboard rail breaks and wiring capacity; a rail's colored stripe does not guarantee its voltage or continuity.

### Teacher Supply Check

Before the student connects power, verify that the actual servo permits regulated 5 V and the DC motor is the documented 3–6 V model. Choose a supply whose continuous current capacity exceeds the **combined starting/stall currents with margin**, using the actual parts' specifications or appropriate supervised verification. Idle current alone is insufficient. Students must not block either shaft to measure stall current.

Check the supply's output voltage, polarity, connector, and lead ratings, and verify voltage stability when both actuators move. Do not assume the kit's breadboard power module or the Uno's 5 V pin can supply both actuators. Local capacitors may help with brief voltage dips, but cannot replace an adequate supply. The teacher chooses them and checks their polarity and voltage rating.

## Wire with All Power Removed

Disconnect USB and disconnect or switch off the external actuator supply before making or changing any connection. Secure the bare DC motor without a wheel or propeller. Secure the unloaded servo and keep its motion area clear. Keep fingers, hair, clothing, and objects away from moving parts.

### Connect the Joystick

Find the labels on the module; do not guess pin order from how it faces you.

| Joystick label | Connect to | What it does |
| --- | --- | --- |
| GND | Shared ground | Provides the 0 V reference |
| +5V | Arduino 5 V | Powers the two analog controls |
| VRx | Arduino A0 | Controls motor power |
| VRy | Arduino A1 | Controls servo position |
| SW | Leave disconnected | Joystick button is unused |

The motor's active direction is the direction that raises the A0 reading. Module orientation can change whether that feels like forward, backward, left, or right.

### Connect the Servo: Orange, Red, Brown

**Trace each wire from the servo cable into its connector.** If you use jumper extensions, their colors may differ: label their other ends before connecting them. Looking at the opposite side of a connector reverses its apparent order, so follow the wire colors and verified terminal functions rather than “left” and “right.”

| Servo wire | Function | Connect to |
| --- | --- | --- |
| **Orange** | **Signal: chosen position** | **Arduino D9** |
| **Red** | **Power: positive** | **External regulated +5 V** |
| **Brown** | **Ground: negative/0 V** | **External supply negative/shared ground** |

Remember: **orange tells, red powers, brown returns**. Orange carries the position instructions; it does not power the servo. Brown must share a ground reference with Arduino GND so the servo can understand D9's signal. Red must receive external +5 V, not Arduino D9 or the Uno's 5 V pin.

Have the teacher verify the actual servo's connector and documentation before power. Do not reverse red and brown or connect orange to the supply rail. These color assignments match [Arduino's servo wiring guidance](https://docs.arduino.cc/learn/electronics/servo-motors/); confirm them for the unit in your kit.

### Connect the Protected DC Motor Stage

Use lesson 04's [MOSFET identification instructions](../04-adc-pwm-motor-control/README.md#identify-the-rfp30n06le-pins) to confirm Gate, Drain, and Source. The connections here use the external supply in place of B1.

| From | To |
| --- | --- |
| External +5 V | DC motor positive |
| DC motor negative | MOSFET Drain, pin 2 |
| MOSFET Source, pin 3 | Shared ground/external negative |
| Arduino D3 | 220–330 ohm series resistor, then MOSFET Gate, pin 1 |
| MOSFET Gate, pin 1 | 10 kilohm pulldown resistor, then shared ground |
| 1N5817 cathode, **banded end** | Motor positive/external +5 V |
| 1N5817 anode, **unbanded end** | Motor negative/MOSFET Drain |

Place the diode close to the motor connections. It gives stored motor current a path when the switch turns off. Reversing it can short the supply when the MOSFET turns on. The servo has its own internal control circuit: its brown wire connects directly to shared ground, **not** to the DC motor's switched Drain terminal.

## Teacher Wiring Checkpoint

With all power removed, trace the joystick labels, servo orange/red/brown connections, MOSFET terminals, both gate resistors, diode band, and all ground connections. Confirm that external +5 V reaches only the actuator power branches and remains separate from Arduino 5 V and VIN. Check for loose wires and accidental shorts, and confirm the secured motor and servo have clear motion areas.

Keep actuator power off during uploads. Before enabling it, the teacher must verify a released joystick reports zero motor command. The servo may move when power is enabled. Remove both power sources before touching wiring again. If parts become hot, smell unusual, smoke, spark, or behave unexpectedly, remove actuator power and USB and tell the teacher; do not reconnect until the setup has been checked.

## Build, Upload, and Check the Controls

1. Keep the external actuator supply off. After the teacher approves the unpowered wiring, connect USB to the Uno.
2. Open **06-joystick-motor-servo-control** in VS Code. Choose the folder containing this lesson's `platformio.ini`. Opening its `main.cpp` tab alone does not select the project for upload.
3. In PlatformIO **Project Tasks**, choose this project's **uno → General → Build**. Wait for `SUCCESS`.
4. Close any monitor using the Uno's port, choose **Upload** for the same project, and wait for `SUCCESS`. Keep actuator power off throughout.
5. Open **Monitor** at **9600 baud**. Expect labeled lines about once per second. With the joystick released, a line could look like this (your ADC numbers may differ):

   ```text
   Motor ADC: 506 | Motor PWM command: 0 | Servo ADC: 512 | Servo pulse command (us): 1500
   ```

6. Leave actuator power off. Release the joystick several times and check that **Motor PWM command stays 0**. Gently move near center and release again. The **deadband** is a small zone around center that ignores tiny changes. Its default center is 512 with a ±40-count zone (472–552). All motor readings through 552 command zero, including travel in the inactive direction. The active direction raises the reading above 552 and increases power toward 255. Very small increases can still round to zero.
7. If releasing the stick gives a nonzero motor command, keep actuator power off. Have the teacher check wiring and the repeated neutral readings, then adjust `MOTOR_NEUTRAL_ADC` or `MOTOR_DEADBAND_ADC` in [control_mapping.h](include/control_mapping.h) if needed. Rebuild, upload, and repeat the neutral check after any change.
8. With actuator power still off, move the servo axis and check that **Servo ADC** and its pulse command change. The initial command limits are **1000–2000 microseconds**, with **1500 microseconds** at the nominal center. These are position instructions, not a promise of exact angles or a full 180-degree sweep.

### Enable Actuator Power with the Teacher

Release the joystick. Have the teacher confirm zero motor command, the supply check, secured actuators, and clear shaft/motion areas. Use a secured unloaded servo; do not attach a mechanism for this first check. Enable the external supply only after approval, without moving any wires. The servo may move toward its middle position immediately.

Keep the motor axis released while the teacher checks servo travel with small movements first. Extend toward the chosen limits only if motion is free. **Stop if the servo binds or persistently buzzes.** Remove actuator power and USB; the teacher checks the setup and narrows `SERVO_MIN_PULSE_US` or `SERVO_MAX_PULSE_US` around `SERVO_CENTER_PULSE_US` if necessary. Rebuild and upload with actuator power off, then repeat the checks. Do not push the shaft by hand or widen the range to force more travel.

Keep hands, hair, clothing, and objects clear of both actuators. Never hold or block the DC motor shaft. A zero command removes motor power; a spinning motor can still **coast**. Center-off is a software command, not a physical emergency switch: the teacher must be able to remove actuator power promptly.

When finished, switch off actuator power first, close the monitor, and disconnect USB. Remove both power sources before changing wiring or components.

## Observe One Control, Then Both

Complete the upload, neutral, and teacher power checks above first. Use the same approved circuit and servo limits throughout. Keep both actuators secured and their movement areas clear. Move only the joystick; never touch or block either shaft. Stop and remove both supplies if either actuator binds, fails to move when clearly commanded to run, or behaves unexpectedly.

1. **Start at center.** Release the joystick and record the four reported numbers. Listen and watch: is the motor stopped or still coasting? Where does the servo settle?
2. **Motor axis only.** Keep the servo axis near center. Move the motor axis partway in its active direction, hold until at least two messages appear, and record the newest numbers and what the motor does. Repeat farther in that direction. Notice whether the servo command stays near its middle value; small sideways hand movements can change it too.
3. **Release, then try the inactive direction.** Release and observe when the motor's sound or movement starts changing. Record whether it coasts after the command becomes zero. Then move the motor axis in the opposite direction while keeping the servo axis near center. Hold and record: does the motor command remain zero?
4. **Servo axis only.** Release the motor axis so its command stays zero. Move the servo axis toward one teacher-approved limit, hold, and record. Repeat toward the other limit, then release. Watch the servo position and motor command. Do not force extra travel.
5. **Both axes together.** Move diagonally to give a moderate active motor command and a changed servo position. Hold and record. Repeat with a different diagonal position inside the approved range. Notice whether you can change both controls together.
6. **Look between messages.** Just after a computer message, move from a centered position to one of your approved diagonal positions. Observe whether either actuator begins responding before the next message. Repeat twice, returning to center between tries. Describe what you actually notice; no stopwatch is needed, and “hard to tell” is useful evidence.
7. **Finish.** Release the stick, switch off actuator power, close the monitor, and disconnect USB. Compare your records and answer the reflection questions below.

### Record Commands and Motion

Copy or print this table if you want more writing space. Use the newest serial line after holding each position. Record motion and sound in your own words, such as “motor did not start,” “motor coasted,” or “servo moved then settled.” Leave a result uncertain if you could not judge it. Do not replace an observation with what you expected to happen.

| Joystick position | Motor ADC | Motor PWM command | Servo ADC | Servo pulse command (µs) | What I heard or saw |
| --- | --- | --- | --- | --- | --- |
| Released at center | | | | | |
| Motor axis: partway active | | | | | |
| Motor axis: farther active | | | | | |
| Release after motor movement | | | | | |
| Motor axis: inactive direction | | | | | |
| Servo axis: toward first limit | | | | | |
| Servo axis: toward other limit | | | | | |
| Both axes: first diagonal | | | | | |
| Both axes: different diagonal | | | | | |

For each timing try, describe the response **between** messages rather than just copying the next line:

| Try | Motor began responding before next message? | Servo began responding before next message? | Evidence from sound or motion |
| --- | --- | --- | --- |
| 1 | Yes / No / Hard to tell | Yes / No / Hard to tell | |
| 2 | Yes / No / Hard to tell | Yes / No / Hard to tell | |
| 3 | Yes / No / Hard to tell | Yes / No / Hard to tell | |

### Four Different Rhythms

The computer messages show **snapshots**: one saved set of readings and commands, not every check the robot makes.

| What happens | Its rhythm in lesson 06 |
| --- | --- |
| Check both joystick axes and update both commands | Every 20 ms: **50 checks per second** |
| Send the servo's position pulse | A frame repeats every 20 ms; the pulse length selects position |
| Switch motor power on and off with PWM | A separate, faster hardware rhythm; the command selects how much of each cycle is on |
| Print a serial snapshot | About once per second, while control checks continue between messages |

The two “50 times per second” rhythms have different jobs: one checks the controls; the other repeats the servo signal. A new servo command takes effect in the following frame. Hardware continues both output signals while the main loop waits. Fifty updates do not mean fifty different positions or speeds: a still joystick can produce the same command repeatedly.

A power or position **command** is an instruction, not a measurement of actual speed or angle. A motor may need a larger command to start, may coast after zero power, and may change speed unevenly. A servo takes time to move. Frequent checks help the program use fresh instructions sooner; they do not make either actuator move or stop instantly.

At joystick center, the DC motor receives zero power command and the servo receives its middle position command. The servo can still use power to hold that position even when it looks stopped.

### Reflect on Your Evidence

1. Which axis controlled motor power, and which controlled servo position? What records support your answer?
2. What happened when you released the motor axis or moved it in the inactive direction? Compare the command with the motion you observed.
3. Could you change one control while keeping the other nearly steady? What did you notice when moving diagonally?
4. There are 1000 ms in a second. Calculate **1000 ÷ 20 = ____ checks per second**. Why could movement begin before the next computer message?
5. Which timing tries showed a response before the next message? If the result was unclear, explain what made it hard to judge.
6. Did a larger motor command always mean an equally larger speed change? What evidence showed a starting threshold, coasting, or time needed for servo motion?
7. Imagine the motor moves a robot and the servo points its steering. Why should the robot keep checking new control instructions while following its plan? What additional sensor would it need to notice an obstacle? Our joystick circuit does not detect obstacles or guarantee an immediate stop.

## Troubleshooting

- **Build or upload fails:** Confirm the selected project is lesson 06 and read the first error. For a port error, close the monitor, check the USB data cable and selected Uno port, and try again with actuator power off.
- **Monitor is blank, garbled, or uses different labels:** Check 9600 baud and explicitly upload lesson 06. Close and reopen its monitor. This program reports motor ADC, motor PWM command, servo ADC, and servo pulse command in microseconds.
- **A number near 506 appears at center:** Read its label. That is a normal possible **ADC reading**, not a motor PWM command. ADC readings range from 0 to 1023; motor PWM commands range from 0 to 255. Center should command zero motor power.
- **Servo chatters and the DC motor does not move:** Remove actuator power and USB. Check the shared-ground connection first: **Arduino GND must join external negative, servo brown, joystick GND, and MOSFET Source**. Check rail breaks and loose connections. Also verify orange → D9, red → external +5 V, and supply capacity/polarity.
- **Motor command remains zero:** With actuator power off and USB connected after wiring approval, try both directions of the motor axis. Only the direction raising A0 above the deadband increases power. If A0 never changes, remove both supplies and check VRx → A0 and joystick power/ground.
- **Motor has a high command but never turns:** Remove both supplies. The teacher checks external motor power, D3's gate-resistor path, MOSFET terminals, pulldown, and diode polarity. Do not leave a motor powered while it fails to turn.
- **Motor stays stopped at very low nonzero commands:** A small power command may be below its **starting threshold**. After the approved checks, observe a clearly larger command without touching the shaft. Stop and ask the teacher if it still fails to turn.
- **Servo binds or keeps buzzing:** Remove both supplies. Check that motion is unobstructed, the servo is unloaded, and its connections are correct. Have the teacher review and narrow the pulse limits before another trial.
- **Readings jump with the stick held still, or the Uno resets:** Remove both supplies. Check joystick connections, shared ground, supply voltage stability during movement, current capacity, and branch wiring. The teacher reviews local decoupling; do not hide a power problem by changing the mapping.
- **Parts get hot, smell unusual, smoke, or spark:** Remove actuator power and USB immediately. Tell the teacher, avoid hot parts, and do not reconnect until the circuit has been checked.
