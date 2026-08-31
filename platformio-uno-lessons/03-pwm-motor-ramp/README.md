# Lesson 03: PWM Motor Ramp

This lesson introduces pulse-width modulation (PWM) by changing the command sent to an externally powered DC motor.

## How PWM Controls the Motor

Pulse-width modulation, or **PWM**, switches the MOSFET on and off very quickly. The **duty cycle** is the percentage of each switching cycle spent on. A 0% duty cycle is always off, a 50% duty cycle is on half the time, and a 100% duty cycle is always on.

The Arduino represents that range with PWM commands from 0 through 255:

| PWM command | Approximate duty cycle | Command meaning |
| --- | --- | --- |
| 0 | 0% | Fully off |
| 128 | 50% | On about half the time |
| 255 | 100% | Fully on |

This program changes the PWM command by one step at a time, so the **command** forms a smooth ramp. The motor's physical speed may not form the same smooth line:

- The motor may remain still through the lowest commands because it needs enough force to overcome friction. The command where it begins turning is its **starting threshold**.
- When the command decreases, the spinning motor may **coast** instead of stopping immediately because its moving parts have momentum.
- Twice the PWM command does not necessarily produce twice the motor speed. Friction, the motor's electrical behavior, its mechanical load, and battery voltage can make the response **non-linear**.

Treat the PWM value as a motor command, not as a direct speed measurement.

## What You Need

- 1 Arduino Uno;
- 1 USB data cable for the Uno;
- 1 solderless breadboard;
- 1 yellow 3–6 V gearbox motor, such as the motor from an Elegoo Arduino car kit;
- 1 RFP30N06LE N-channel power MOSFET;
- 1 1N5817 Schottky diode;
- 1 Snap Circuits B1 battery holder with 2 AA batteries;
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

## Build, Upload, and Run the Ramp

1. Keep B1 switched off. After the teacher approves the unpowered circuit, connect the Uno to the computer with a USB data cable.
2. Open the `platformio-uno-lessons/03-pwm-motor-ramp` folder in VS Code. The folder you open must contain this lesson's `platformio.ini` file.
3. Select the PlatformIO alien-head icon in the Activity Bar. Under **Project Tasks**, expand **uno**, then expand **General**.
4. Select **Build** and wait for the terminal to end with `SUCCESS` and no error message.
5. Select **Upload** and wait for the upload terminal to end with `SUCCESS`. The Uno restarts and begins producing the PWM ramp, but the motor remains unpowered while B1 is off.
6. Confirm again that the bare motor is secured and its shaft is clear. Ask the teacher for a final wiring check, then switch on B1 without touching or moving the circuit.
7. Watch for at least three complete cycles. Each cycle should take about eight seconds: roughly four seconds ramping upward and four seconds ramping downward.
8. When the activity is complete, switch off B1 first and then disconnect USB. Do not change the circuit until both power sources are removed.

## Observe and Record the Motor Response

The program changes its command smoothly and repeats the same pattern every cycle. Watch the motor itself to find out whether its motion follows that command in the same way.

| Cycle | Approximate full-cycle time | Did the motor wait before starting? | Did it coast while slowing? | Other changes in sound or motion |
| --- | --- | --- | --- | --- |
| 1 |  |  |  |  |
| 2 |  |  |  |  |
| 3 |  |  |  |  |

After recording three cycles, answer these questions:

1. Did the motor begin moving as soon as the upward ramp began, or was there a starting threshold?
2. Did it stop as soon as the downward ramp reached its lowest commands, or did it coast?
3. Did the speed appear to change evenly throughout the ramp? Describe any range where the response changed more or less than expected.
4. How was the motor's physical response different from the program's smooth 0-to-255-to-0 command?

## Troubleshooting

- **Build does not end with `SUCCESS`:** Make sure VS Code opened the `03-pwm-motor-ramp` folder that contains this lesson's `platformio.ini`. Run **Build** again and read the first error in the terminal.
- **Upload cannot find the Uno:** Use a known USB data cable, try a direct computer USB port, close other programs that may be using the Uno's port, and try **Upload** again.
- **The motor never turns:** Immediately switch off B1 and disconnect USB. Ask the teacher to check the B1 batteries, shared ground, D3 gate path, MOSFET pin functions, motor-current path, and diode polarity before reconnecting power.
- **The motor runs at one speed instead of ramping:** Remove both power sources. Check that Arduino D3 reaches MOSFET Gate pin 1 through the 220–330 ohm resistor and that the 10 kilohm pulldown connects the Gate to shared ground rather than to the Drain.
- **The motor turns only near the top of the ramp:** A starting threshold is expected. Record it as an observation. Stop and ask the teacher to check the circuit if the motor never turns during the cycle.
- **The motor or Uno stutters or resets:** Remove both power sources. Ask the teacher to check for loose motor-current wiring, a missing shared ground, weak B1 batteries, or a 1N5817 that is not connected directly across the motor with the documented polarity.
- **A cycle is not close to eight seconds:** Confirm that `RAMP_STEP_DELAY_MS` is `16` and that both loops in `src/main.cpp` still cover every PWM value from 0 through 255.
