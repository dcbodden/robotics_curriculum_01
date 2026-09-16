# Lesson 06: Joystick Motor and Servo Control

A joystick gives us two controls in one handle. One direction controls how much power a DC motor receives. The other controls the position of a servo. A **servo** is a motor that moves toward a chosen position rather than just spinning continuously. These two moving parts are **actuators**.

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

## Four Gated Core Missions: One Approved Circuit

**Goal:** First check both joystick inputs with actuator power **off**. Then observe the servo alone, the motor alone, and both controls together. The wiring stays **fixed** through all four missions: “servo alone” means you move only the servo joystick axis while motor power is commanded zero; the servo and motor share the same external supply when it is on.

**Prepare and stay safe:** Remove **USB and external actuator power before any wiring or mechanical change**. The Uno's USB powers its electronics; **Arduino 5 V goes only to joystick +5V**. Teacher-approved **external regulated +5 V** goes to **servo red and motor positive only**, through separate teacher-approved branches. Keep these positive supplies separate. Arduino GND, joystick GND, servo **brown**, external negative, and MOSFET Source share ground; **brown never goes to the motor's switched Drain**. Servo **orange** is the D9 signal. Trace original servo colors through any extensions; their colors or connector orientation may differ. Keep the **bare motor secured with no wheel, propeller, gear, or other shaft attachment** and the **unloaded servo secured** with a clear motion area. Keep hands, hair, clothing, and objects away from both. Never hold or block either shaft. If a motor fails to turn with a clear high command, the servo binds or persistently buzzes, a part gets hot, power becomes unstable, smoke/sparks or unusual smells appear, or motion/wiring behaves unexpectedly, **turn off external actuator power and unplug USB immediately**. Tell the teacher; do not touch hot parts or reconnect until the setup is checked. A zero motor command is not an emergency power switch, and a spinning motor can coast.

### Core Mission 1: Check Inputs Before Actuator Power

**Observable result:** With the external supply **off**, see the released joystick report **Motor PWM command: 0** and see A0 and A1 readings change as the stick moves. Neither actuator can move during this input check.

1. With **USB unplugged and external power off**, the teacher prepares the [full wiring tables](#wire-with-all-power-removed). Check the joystick labels: **GND → shared ground, +5V → Arduino 5 V, VRx → A0, VRy → A1, SW unconnected**. Check servo **orange → D9, red → external +5 V, brown → shared ground**. Keep external positive away from Arduino 5 V/VIN and joystick. Check the protected motor path: **D3 → 220–330 Ω resistor → MOSFET Gate**, **Gate → 10 kΩ pulldown → ground**, **motor negative → Drain**, **Source → shared ground**, and **1N5817 banded end → motor positive**, unbanded end → motor negative. The teacher confirms MOSFET terminal identities, shared ground, supply polarity and capacity, no shorts or split-rail mistakes, and both secured clear motion areas. Do not restore any power until approved.
2. Keep **actuator power off**. Connect Uno USB with a data cable. Open `platformio-uno-lessons/06-joystick-motor-servo-control` as the PlatformIO project in VS Code; opening a `main.cpp` tab alone does not select it. Under **Project Tasks > uno > General**, choose **Build** and wait for `SUCCESS`. Close any open Monitor, choose **Upload** and wait for `SUCCESS`. Keep actuator power off throughout uploads.
3. Open **Monitor** at **9,600 baud**. Release the joystick several times. Check that the labeled **Motor PWM command stays 0**. A center line may show `Motor ADC: 506 | Motor PWM command: 0 | Servo ADC: 512 | Servo pulse command (us): 1500`; your ADC numbers may differ. A number near 506 is an **ADC reading**, not motor PWM. If release gives a nonzero motor command, **keep actuator power off** and ask the teacher to check neutral and adjust the mapping before rebuilding, uploading, and repeating this check.
4. With actuator power still off, move **VRx's motor axis** gently in both directions and watch **Motor ADC** and its PWM command. Only the direction that raises A0 above the center-off zone should increase motor power; the opposite direction and release should command 0. Move **VRy's servo axis** gently and watch **Servo ADC** and its position command change. Return the stick to center before the next mission.

**Notice:** Released motor PWM: ______. One changed A0 reading: ______. One changed A1 reading: ______.

**Gate to Mission 2:** External actuator power remains **off**. The teacher confirms repeated motor-zero release readings, changing A0/A1 inputs, approved supply and connector polarity, secure actuators, and clear motion areas. If any check fails, stop here. If ending the lesson now, close Monitor and unplug USB; change wiring only with both sources off.

### Core Mission 2: Move Only the Servo Axis

**Observable result:** The secured unloaded servo moves toward a new position while the motor axis stays released and **Motor PWM command remains 0**.

1. Keep the **same approved circuit fixed**. Release the joystick; the teacher checks a fresh zero motor command and confirms the servo is unloaded, the motor bare and secured, both motion areas clear, and the external supply approved. The teacher then enables actuator power **without moving wires**. The servo may move to its middle position immediately; it may still draw power while holding position.
2. Keep the **motor axis released**. Move the servo axis a little to one side, then a little to the other. Start near center; approach only the teacher-approved travel limits if motion remains free. Watch the servo move and read a new **Servo ADC** and **Servo pulse command** line. Check that **Motor PWM command stays 0**. Do not push or force the servo shaft.

**Notice:** Servo movement I saw: ____________________. Motor PWM stayed at: ______.

**Gate to Mission 3:** Release the stick. If the servo binds, persistently buzzes, or moves unexpectedly, remove **actuator power and USB** and ask the teacher to inspect and adjust travel before proceeding. Otherwise keep the fixed circuit and approved supply as they are; move only joystick axes in the next mission. If stopping here, turn off actuator power first, close Monitor, then unplug USB.

### Core Mission 3: Test Motor Center-Off

**Observable result:** With the servo axis near center, motor release and inactive travel command **0**, while the active motor direction can command power and turn the secured motor.

1. Keep the **servo axis near center** and clear of its motion area. With the same approved circuit, move the motor axis partway in the direction that **raises Motor ADC**. Hold until two messages appear; record the newest **Motor PWM command** and observe sound or motion from a safe distance. Try a clearly higher approved command if needed. If the motor never turns at a clear high command, remove **actuator power and USB** and ask the teacher to inspect it; do not leave it powered and stalled.
2. Release the motor axis. Check that **Motor PWM command returns to 0**; the shaft may coast briefly. Then move the motor axis into the **inactive direction** and check that command remains 0. Keep the servo axis near center throughout; the servo may still use power to hold its middle position.

**Notice:** Active-direction command: ______. Released/inactive commands: ______ / ______. Motor motion or coasting I saw: ____________________.

**Gate to Mission 4:** Return the stick to center. Continue only if the motor is secured, its shaft area remains clear, center-off commands are 0, and both actuators behave normally. The wiring and external supply stay fixed. If stopping here, turn off actuator power first, close Monitor, then unplug USB.

### Core Mission 4: Change Both Controls Together

**Observable result:** A moderate diagonal stick movement changes the motor command and servo position together, possibly before the next one-second computer message.

1. With the same approved circuit and clear motion areas, move diagonally toward a **moderate active motor command** and an approved servo position. Do not push either axis beyond its teacher-approved range. Hold for two messages; record the motor and servo commands and what each actuator actually did. Return to center.
2. **Predict from the earlier missions:** If you move just after a computer message, will either actuator begin responding before the next message? Try one approved diagonal movement just after a line, watch from a safe distance, then release. “Hard to tell” is a valid observation; physical motion can lag behind a command.

**Notice:** Motor changed: ____________________. Servo changed: ____________________. Before next message? ____________________.

**Stop safely:** Turn off **external actuator power first**, close Monitor, then unplug USB. Remove both power sources before any wiring or mechanical change. Leave the approved circuit unchanged until the teacher says it is safe to disassemble.

## Name the Idea: Two Inputs, Two Commands

The focus code is the pair of decisions in [`include/control_mapping.h`](include/control_mapping.h): `motorPwmFromAdc(...)` turns **A0** into a center-off motor power command; `servoPulseFromAdc(...)` turns **A1** into a bounded servo position command. The program checks both axes every **20 ms**, or **50 times per second**, even while its main loop waits between roughly **one-second Serial messages**. That control-check rhythm differs from the servo's repeated **20 ms position-signal frames** and the motor's faster PWM on/off pulses. A message shows one snapshot; it does not show every check. A power or position **command** is an instruction, not an exact measure of speed or angle. The [teacher guide](teacher-guide.md#interpret-the-four-timing-rhythms) explains timer configuration and pulse buffering; those details are not needed for these missions.

## Circuit and Optional Activity Reference

The sections below retain exact wiring tables, an extended recording activity, and troubleshooting. Teachers can use the [preparation guide](teacher-guide.md) for supply sizing, calibration, timing implementation, and detailed diagnosis.

### Two Power Paths

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

### Wire with All Power Removed

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

### Optional Extended Recording

After completing all four missions, copy or print this table if you want more writing space. If power was removed at the end, use the **same approved wiring**; reconnect USB with external power off, verify released motor command 0 and clear motion areas, and ask the teacher before enabling external power again. Move only the joystick. Record motion and sound in your own words, such as “motor did not start,” “motor coasted,” or “servo moved then settled.” Leave a result uncertain if you could not judge it. Do not replace an observation with what you expected to happen. Finish by turning off external power first, closing Monitor, and unplugging USB.

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

### Reflect on Your Evidence

1. Which axis controlled motor power, and which controlled servo position? What records support your answer?
2. What happened when you released the motor axis or moved it in the inactive direction? Compare the command with the motion you observed.
3. Could you change one control while keeping the other nearly steady? What did you notice when moving diagonally?
4. There are 1000 ms in a second. Calculate **1000 ÷ 20 = ____ checks per second**. Why could movement begin before the next computer message?
5. Which timing tries showed a response before the next message? If the result was unclear, explain what made it hard to judge.
6. Did a larger motor command always mean an equally larger speed change? What evidence showed a starting threshold, coasting, or time needed for servo motion?
7. Imagine the motor moves a robot and the servo points its steering. Why should the robot keep checking new control instructions while following its plan? What additional sensor would it need to notice an obstacle? Our joystick circuit does not detect obstacles or guarantee an immediate stop.

### Student Troubleshooting and Stop Conditions

- **Build or upload fails:** Keep actuator power off. Confirm the selected project is lesson 06, close Monitor if it is using the port, check the USB data cable and Uno port, and ask the teacher to read the first error.
- **Monitor is blank or garbled:** Check **9,600 baud**, close and reopen Monitor, and confirm lesson 06 uploaded. It should label Motor ADC, Motor PWM command, Servo ADC, and Servo pulse command.
- **A number near 506 appears at center:** Read its label. That can be a normal **ADC reading**, not the motor PWM command. Released motor PWM should read **0** before actuator power is enabled.
- **Motor command stays zero in both stick directions:** Keep actuator power off and ask the teacher to inspect A0 and joystick connections. Only the direction that raises A0 above the center-off zone should increase the command.
- **Servo binds or keeps buzzing, a clear high motor command does not turn the motor, or readings/power become unstable:** Remove **external power and USB**. Ask the teacher to inspect the circuit using [detailed diagnosis](teacher-guide.md#detailed-diagnosis). Do not force a shaft or leave a nonmoving commanded motor powered.
- **Heat, unusual smell, smoke, sparks, or unexpected movement:** Remove **external power and USB immediately**. Tell the teacher, avoid hot parts, and do not reconnect until the setup has been checked.
