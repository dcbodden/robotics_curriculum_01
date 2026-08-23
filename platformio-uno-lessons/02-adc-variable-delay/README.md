# Lesson 02: ADC Variable Delay

This lesson uses a three-terminal potentiometer to send an adjustable voltage to analog pin A0. The Arduino reads that voltage and uses it to change the pin-8 blink delay.

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

Disconnect the USB cable from the Arduino Uno before adding or changing any wires. The board must have no USB power while you build the circuit.

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

## Build, Upload, and Observe

1. Open the `02-adc-variable-delay` folder in VS Code. Make sure the PlatformIO IDE extension is installed and the teacher-approved circuit is connected. If the PlatformIO icon does not appear in this multi-project workspace, choose **File > Add Folder to Workspace...**, add the `02-adc-variable-delay` folder, and wait for PlatformIO to activate.
2. Select the PlatformIO icon in the VS Code Activity Bar. Under **Project Tasks**, expand **uno**, expand **General**, and select **Build**. Wait for the terminal to report `SUCCESS`.
3. Connect the Arduino Uno with its USB cable. In the same **General** task list, select **Upload** and wait for the upload to finish successfully.
4. Select **Monitor** from the **General** task list. The project opens the serial monitor at 9,600 baud.
5. Slowly move the potentiometer to a new position. Watch the labeled `ADC reading` and `Delay` values in the serial monitor, and watch the LED blink.
6. Compare what you see: lower ADC readings should show shorter delays and faster blinking, while higher readings should show longer delays and slower blinking.

## Record Your Observations

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