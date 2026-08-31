# Lesson 05: Interrupt-Driven Motor Control

This lesson uses timer interrupts to keep an RV-controlled motor command responsive while the main program waits between serial reports.

## What You Need

- 1 Arduino Uno;
- 1 USB data cable for the Uno;
- 1 solderless breadboard;
- 1 yellow 3–6 V gearbox motor, such as the motor from an Elegoo Arduino car kit;
- 1 RFP30N06LE N-channel power MOSFET;
- 1 1N5817 Schottky diode;
- 1 Snap Circuits B1 battery holder with 2 AA batteries;
- 1 three-terminal Snap Circuits RV variable-resistor snap;
- 1 resistor from 220 to 330 ohms for the MOSFET gate;
- 1 10 kilohm resistor for the gate pulldown; and
- teacher-approved jumper wires and Snap Circuits-to-breadboard connections.

## Identify the RFP30N06LE Pins

First confirm that the part marking says **RFP30N06LE** or **P30N06LE**. Other TO-220 parts can look identical while using different electrical connections.

Hold the MOSFET with its flat, printed face toward you and its three metal leads pointing down. In that reference view, the leads are numbered from left to right:

| Terminal | Pin | Function in these lessons |
| --- | --- | --- |
| Gate (G) | 1, left lead | The control input. Arduino D3 reaches this terminal through the gate resistor, and the pulldown resistor keeps it at ground when it is not being driven. |
| Drain (D) | 2, center lead | The switched-current terminal associated with the motor-negative side of the circuit. |
| Source (S) | 3, right lead | The return terminal associated with shared ground. |
| Drain flange/tab | Metal tab behind the plastic body | Electrically the same Drain node as pin 2; it is not an insulated mounting tab. |

Use the terminal names and functions when tracing the circuit. Do not rely only on “left,” “middle,” and “right,” because turning the part around reverses its apparent order. The [RFP30N06LE manufacturer datasheet](https://www.sparkfun.com/datasheets/Components/General/RFP30N06LE.pdf) shows the TO-220AB Gate, Drain, Source, and Drain-flange assignments.

## Connect the Low-Side Motor and Gate Paths

The B1 batteries power the motor. The Arduino sends only the control signal and shares the circuit's ground reference.

| From | To | Purpose |
| --- | --- | --- |
| B1 positive (`+`) | Motor positive terminal | Starts the motor-power path using the separate two-AA supply. |
| Motor negative terminal | MOSFET Drain, pin 2 | Sends motor current to the MOSFET's switched terminal. |
| MOSFET Source, pin 3 | Shared-ground breadboard rail | Returns switched motor current toward B1 negative. |
| B1 negative (`-`) | Shared-ground breadboard rail | Completes the B1 motor-current loop. |
| Arduino GND | Shared-ground breadboard rail | Gives D3 and the MOSFET Gate the same 0 V reference. |
| Arduino D3 | 220–330 ohm resistor, then MOSFET Gate, pin 1 | Carries the PWM control signal while the series resistor limits each gate charge/discharge pulse. |
| MOSFET Gate, pin 1 | 10 kilohm resistor, then shared ground | Pulls the Gate LOW when D3 is disconnected or not actively driving it. |

When the MOSFET is on, the main motor-current path is:

```text
B1 positive -> motor -> MOSFET Drain -> MOSFET Source -> B1 negative
```

The Arduino remains USB-powered. **Never connect B1 positive to Arduino 5 V, VIN, A0, D3, or any other Arduino pin.** B1 negative joins Arduino GND only at the documented shared-ground node.

## Add the 1N5817 Flywheel Diode

Place the 1N5817 directly across the motor terminals, as close to the motor connections as the breadboard permits.

| Diode end | Connect to |
| --- | --- |
| Cathode (banded end) | Motor positive, which is the same node as B1 positive |
| Anode (unbanded end) | Motor negative, which is the same node as MOSFET Drain pin 2 |

During normal motor power, this orientation keeps the diode from conducting. When PWM turns the MOSFET off, the motor's stored magnetic energy can keep current circulating through the diode instead of producing a damaging voltage spike.

Do not reverse the diode. A reversed flywheel diode would conduct across B1 when the MOSFET turns on, creating a short circuit.

## Complete the Safety Check Before Powering the Circuit

1. Disconnect the USB cable and switch off or remove the batteries from B1. Build or change the circuit only while **both** power sources are disconnected.
2. Use only the bare motor. Do not attach a wheel, propeller, gear, or anything else to its shaft.
3. Secure the motor so it cannot roll, jump, or pull wires loose. Keep hands, hair, clothing, and other objects away from the shaft.
4. Ask the teacher to trace every connection before power is applied. The teacher must verify the MOSFET terminals, gate resistors, shared ground, separate positive supplies, and the 1N5817 band toward B1/motor positive.
5. Connect USB and enable B1 only after the teacher approves the circuit. Remove both power sources again before moving a wire or component.

Immediately switch off B1 and disconnect USB if the motor does not turn when commanded to run, any component or battery becomes hot, there is an unusual smell, smoke or sparking appears, or the motor or wiring moves unexpectedly. Tell the teacher and do not touch a hot component or reconnect power until the circuit has been checked.
