# Lesson 02: ADC Variable Delay

This lesson uses a three-terminal potentiometer to send an adjustable voltage to analog pin A0. The Arduino reads that voltage and uses it to change the pin-8 blink delay.

## Recommended Prior Learning

Complete [lesson 01: GPIO Transistor Switch](../01-gpio-transistor-switch/) first. Reuse the teacher-approved Snap Circuits transistor-and-LED assembly from that lesson, including its protected D8 control connection, shared Arduino ground, and approved assembly power source. Keep the assembly intact. The earlier [Multimeter and Ohm's Law](../../electronics-fundamentals-lessons/01-multimeter-ohms-law/) and [Coding Fundamentals](../../coding-fundamentals-lessons/) lessons prepare you for measurement, Build, Upload, and blinking output.

## Core Mission: Turn the RV, Compare Two Blinks

**Goal:** Watch a change in the Snap Circuits RV position change the `ADC reading`, the printed `Delay`, and the speed of the lesson 01 LED blink.

**Prepare and stay safe:** Start with the approved lesson 01 assembly. Keep its **own approved power source off** and **unplug Arduino USB** before adding or checking RV wires. Do not rebuild the LED/transistor assembly or move its protected D8 control lead or shared-ground lead. Never connect Arduino 5 V to the assembly's power circuit. Ask a teacher to check the RV, the approved assembly, and both power sources before restoring power. If a part gets hot, a wire comes loose, or the LED behaves unexpectedly, turn off assembly power, unplug USB, and ask the teacher to inspect the setup. Never move wires while powered.

1. With **USB unplugged and assembly power off**, connect the RV's two outer terminals to **Arduino 5 V and GND**. Connect its center sliding terminal, called the **wiper**, to **A0**. The outer terminals may trade places; that changes which sliding direction raises the reading. Leave lesson 01's **D8-to-protected-transistor-control** and **Arduino-GND-to-assembly-ground** connections as approved. Use the [wiring table and photos](#wire-the-potentiometer) if you need to find the RV terminals.
2. **Teacher checkpoint, still unpowered:** The teacher traces 5 V → RV outer terminal, GND → other RV outer terminal, and wiper → A0; checks that 5 V and GND are not shorted, no external assembly power reaches A0 or Arduino 5 V, and the lesson 01 control and ground connections are still protected and correct. Restore power only after approval.
3. Open `platformio-uno-lessons/02-adc-variable-delay` as the PlatformIO project in VS Code. Under **Project Tasks > uno > General**, choose **Build** and wait for `SUCCESS`. Close any open Serial Monitor. With the approved circuit fixed and assembly power still off, reconnect USB, choose **Upload**, and wait for `SUCCESS`. Then turn on the assembly's approved power. Do not change wiring during or after upload.
4. Choose **Monitor** under the same project tasks. It runs at **9,600 baud**. Leave the RV at one position. Read a line like `ADC reading: 512 | Delay: 1050 ms` and watch several LED blinks. Record the actual ADC number, delay, and whether the blink looks faster or slower in the table below.
5. **Compare one safe change:** Slide the RV to a clearly different position **without touching any wires**. Wait for a new monitor line and several blinks. Record the new numbers and blink speed. If the readings stay nearly the same, ask the teacher to help you choose two positions farther apart; do not rewire while powered.

| RV position | ADC reading | Delay (ms) | Blink speed I saw |
| --- | --- | --- | --- |
| First position |  |  |  |
| Second position |  |  |  |

**Notice:** Which position had the **lower** ADC reading? Did it also have the **shorter** printed delay and **faster** blink? Record what you saw, even if the RV moves in the opposite direction from what you expected.

**Stop safely:** Turn off the assembly's approved power, stop Monitor, then unplug USB. Change or remove wires only after **both** power sources are off. Leave the approved assembly intact.

## Name the Idea: One Reading Changes One Wait

The RV changes the voltage at A0. The Arduino turns that voltage into an **ADC number**. The program uses the number to choose how long to wait while D8 is HIGH or LOW. A shorter wait usually makes the LED blink faster; a longer wait makes it blink slower. The focus lines in [`src/main.cpp`](src/main.cpp) are `analogRead(ANALOG_INPUT_PIN)`, `map(...)`, `digitalWrite(...)`, and `delay(stateDelayMs)`. The program reads A0 again before the next HIGH or LOW wait. You can follow that chain without learning how the ADC hardware works inside.

**Predict and try:** From your two rows, predict what will happen if you move the RV toward the position with the **higher ADC reading**. With the same teacher-approved wiring fixed, reconnect USB and then the assembly's approved power if they are off. Try only that RV movement, observe the printed delay and blink, and stop safely again. Do not change any wire while powered. Was your prediction supported by what you saw?

## Optional Reinforcement and Deeper Reference

After this lesson, [Manual Voltage Mapping](../../electronics-fundamentals-lessons/02-manual-voltage-mapping/) lets you sort measured voltages into eight digital levels. The sections below explain the ADC in more detail, show the approved circuit, and offer help when the first comparison does not work.

## How the Arduino Turns Voltage Into a Number

Voltage can slide smoothly from low to high, like moving a dimmer switch. The Arduino's **analog-to-digital converter**, or **ADC**, measures the voltage on A0 and chooses a whole-number value for it.

The Uno has a 10-bit ADC, so it can choose from 1,024 values. Those values are numbered **0 through 1023** because counting starts at 0. A voltage near 0 V gives a reading near 0, a voltage near 5 V gives a reading near 1023, and voltages in between give readings in between. This lets the program work with a changing voltage as a digital number.

## How the Reading Changes the Blink Speed

A low ADC reading makes a shorter delay, so the LED blinks faster. A high ADC reading makes a longer delay, so the LED blinks slower. Watch how the numbers change as you move the slider or turn the knob on your variable resistor; depending on which outer potentiometer terminal connects to 5 V, either direction could make the reading go up.

## Keep the Lesson 01 Output Circuit

Leave the transistor and LED assembly from lesson 01 connected just as it was. Do not rebuild it or move its parts. Keep these two lesson 01 connections:

- Arduino GND connects to the existing assembly's ground so both circuits share the same ground.
- Arduino digital pin 8 connects to the existing NPN transistor base control point.

Lesson 02 adds the potentiometer connections below; it does not replace the pin-8 output circuit.

## Wire the Potentiometer

Disconnect the USB cable and turn off the assembly's approved power before adding or changing any wires. Both sources must be off while you build or inspect the circuit.

A potentiometer has two outer terminals and one center terminal called the **wiper**. Connect it as a voltage divider:

| Potentiometer terminal | Arduino Uno connection |
| --- | --- |
| One outer terminal | 5 V |
| Other outer terminal | GND |
| Center wiper | A0 |

The two outer terminals may trade places. Swapping them changes which turning direction raises the A0 voltage, but the voltage divider still works the same way.

## Circuit Reference Photos

These original, repository-owned photos show one completed Snap Circuits experiment connected to the Arduino Uno. Use the wiring table above as the connection guide, and ask a teacher to check your circuit before applying power.

![Full Snap Circuits experiment connected to an Arduino Uno](external_circuit_images/full_circuit.jpeg)

The full circuit shows the variable resistor near the bottom and the transistor-controlled LED assembly near the top.

![Arduino power and analog headers with jumpers connected to 5 V, GND, and A0](external_circuit_images/variable_resistor_pin_connections.jpeg)

The variable resistor uses Arduino 5 V and GND across its outer terminals. Its center wiper connects to analog input A0.

![Arduino digital header with jumpers connected to pin 8 and GND](external_circuit_images/transistor_pin_connections.jpeg)

The lesson 01 output assembly keeps its control connection to digital pin 8 and its shared-ground connection to Arduino GND.

## Safety Check

Ask a teacher to verify every connection before reconnecting the USB cable. Check that no wire or metal part directly joins Arduino 5 V to GND, because that would create a short circuit and could damage the board.

Keep the voltage on A0 between 0 V and 5 V. Use only the Arduino's 5 V and GND pins to power this potentiometer; do not connect a separate battery, power supply, or other externally powered analog signal to A0.

Make sure the center wiper is securely connected to A0. If A0 is left unconnected, or **floating**, it can pick up electrical noise and produce unstable readings that jump around even when no one turns the knob.

## Optional Three-Position Recording

Test three different potentiometer positions. For each position, copy the ADC reading and delay from the serial monitor, then describe the visible blink speed as fast, medium, or slow.

| Potentiometer position | ADC reading (0-1023) | Delay (milliseconds) | Visible blink speed |
| --- | --- | --- | --- |
| Position 1 |  |  |  |
| Position 2 |  |  |  |
| Position 3 |  |  |  |

Compare your three rows. Which position had the lowest ADC reading and fastest blink? Which position had the highest ADC reading and slowest blink?

## Troubleshooting

- **ADC readings jump around:** Disconnect USB power, then check that the center wiper connects firmly to A0 and the two outer terminals connect to 5 V and GND. A loose or floating A0 input gives unstable readings.
- **The adjustment direction seems reversed:** This is normal. The direction depends on which outer terminal connects to 5 V. You may use it as wired or, with USB power disconnected, swap the two outer-terminal wires.
- **The serial monitor shows nothing:** Make sure the sketch uploaded successfully, open PlatformIO **Monitor**, and check that it is using 9,600 baud.
- **The blink timing does not change:** Watch the ADC and delay values while adjusting the potentiometer. If the values do not change, recheck the A0 wiper, 5 V, and GND connections. If the values change but the LED does not, recheck lesson 01's pin-8 and shared-ground connections.
