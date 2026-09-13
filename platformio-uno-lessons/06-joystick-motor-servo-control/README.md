# Lesson 06: Joystick Motor and Servo Control

A joystick gives us two controls in one handle. One direction controls how much power a DC motor receives. The other controls the position of a servo. A **servo** is a motor that moves toward a chosen position rather than just spinning continuously.

Our program checks both joystick directions every **20 milliseconds**, or **50 times per second**. Like lesson 05, it keeps checking while the main loop waits between computer messages. The motor control is **center-off**: releasing the joystick commands zero motor power. Pushing in one direction increases power; pushing the opposite way keeps power off. The servo control moves between two position limits, with a middle command when centered.

## Before You Begin

Complete [lesson 05](../05-interrupt-driven-motor-control/README.md) with your teacher. You should recognize analog inputs, motor power commands, and timer reminders. Your teacher handles power-supply selection and verifies the circuit before power is applied.

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
