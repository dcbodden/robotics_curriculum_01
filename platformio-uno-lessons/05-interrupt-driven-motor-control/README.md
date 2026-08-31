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
