# Lesson 05: Interrupt-Driven Motor Control

This lesson uses timer interrupts to keep an RV-controlled motor command responsive while the main program waits between serial reports.

## How the Two Parts of the Program Share Work

The **foreground** is the normal work in `setup()` and `loop()`. In this lesson, `loop()` copies the latest ADC and PWM values, prints them, and then waits 1,000 milliseconds. That wait blocks the foreground, just as it did in lesson 04.

Timer1 provides a separate 100-millisecond alarm called an **interrupt**. When the alarm occurs, the Arduino briefly pauses the foreground and runs the interrupt service routine, or ISR. The ISR reads A0, maps the ADC reading to a PWM command, updates D3, and saves the latest values. It performs no serial printing. After the ISR finishes, the foreground continues from where it paused. This lets the motor command update about ten times during each one-second foreground wait.

The saved ADC and PWM variables are marked `volatile`. This tells the compiler that an interrupt can change them unexpectedly. `volatile` keeps the compiler from reusing an out-of-date value, but it does not guarantee that two related values will be copied together.

The Uno processes data 8 bits at a time, while the 0–1023 ADC reading needs 16 bits. An interrupt could otherwise change part of that value while `loop()` is copying it. The **atomic snapshot** briefly pauses interrupts, copies both the ADC and PWM values, and then restores interrupts. Slow serial printing happens only after interrupts are restored, so it cannot hold up the 100-millisecond controller.

Calling `analogRead()` inside an ISR is an intentional teaching trade-off. `analogRead()` waits synchronously for one conversion, which makes this ISR longer than an ISR should usually be. Here it runs at a bounded rate of only 10 times per second, does no serial work, and keeps the demonstration direct. More advanced systems normally keep ISRs shorter by starting the ADC separately or asking foreground code to do longer work.

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

## Connect the RV Voltage Divider to A0

Make these connections only while USB is disconnected and B1 is switched off or has its batteries removed:

| RV terminal | Arduino connection | Purpose |
| --- | --- | --- |
| One outer terminal | Arduino 5 V | Supplies the high side of the RV from the Arduino power domain. |
| Other outer terminal | Arduino GND/shared ground | Supplies the 0 V side of the RV. |
| Center wiper | Arduino A0 | Provides the adjustable input read by the timer interrupt. |

The two outer terminals may trade places; swapping them changes which adjustment direction raises the A0 reading. The center wiper must still connect to A0.

**Never connect B1 positive to the RV, A0, Arduino 5 V, or VIN.** The RV uses Arduino 5 V, while B1 positive belongs only to the motor-positive power path. The two power domains share only the documented ground connection.

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
4. Ask the teacher to trace every connection before power is applied. The teacher must verify the MOSFET terminals, gate resistors, RV-to-A0 voltage divider, shared ground, separate positive supplies, and the 1N5817 band toward B1/motor positive.
5. Connect USB and enable B1 only after the teacher approves the circuit. Remove both power sources again before moving a wire or component.

Immediately switch off B1 and disconnect USB if the motor does not turn when commanded to run, any component or battery becomes hot, there is an unusual smell, smoke or sparking appears, or the motor or wiring moves unexpectedly. Tell the teacher and do not touch a hot component or reconnect power until the circuit has been checked.

## Compare Lesson 04 and Lesson 05 Responsiveness

This is a fair comparison: keep the same RV, motor circuit, B1 supply, and USB-powered Arduino for both programs. Do not move any wires between tests. The RV must keep one outer terminal on Arduino 5 V, its other outer terminal on shared ground, and its center wiper on A0. The protected motor circuit must keep the same D3 gate path, separate B1 positive motor supply, shared ground, and flywheel diode used in lesson 04.

Both lessons print one serial report about every 1,000 milliseconds. The important observation is how soon the **motor** responds between those reports: lesson 04 updates its command only once per 1,000-millisecond loop, while lesson 05's timer updates it every 100 milliseconds.

1. Switch off B1 and keep the circuit unchanged. Open `platformio-uno-lessons/04-adc-pwm-motor-control` in VS Code, select **Build** under PlatformIO's **Project Tasks > uno > General**, wait for `SUCCESS`, and then select **Upload**.
2. Open the serial monitor at 9,600 baud. Adjust the RV until the reported ADC and PWM values are near zero.
3. Confirm that the bare motor is still secured and the wiring still has teacher approval, then switch on B1.
4. Just after a new serial line appears, move the RV quickly from near minimum to near maximum. Watch the motor without touching it and record whether its response seems immediate or delayed. Repeat the test three times, then repeat three times from near maximum to near minimum.
5. Switch off B1 without changing the circuit. Open `platformio-uno-lessons/05-interrupt-driven-motor-control` in VS Code, select **Build** and wait for `SUCCESS`, select **Upload** and wait for `SUCCESS`, and then reopen the serial monitor at the same 9,600-baud setting.
6. Repeat the same three low-to-high and three high-to-low RV movements. Make each movement just after a serial line so the trials are as similar as possible.
7. Switch off B1 when the comparison is complete, close the serial monitor, and disconnect USB. Remove both power sources before changing any wire.

| RV movement | Lesson 04 motor response | Lesson 05 motor response | Which response seemed faster? |
| --- | --- | --- | --- |
| Low to high, trial 1 |  |  |  |
| Low to high, trial 2 |  |  |  |
| Low to high, trial 3 |  |  |  |
| High to low, trial 1 |  |  |  |
| High to low, trial 2 |  |  |  |
| High to low, trial 3 |  |  |  |

After completing the table, answer these questions:

1. Did the serial lines appear at about the same one-second pace in both lessons?
2. Which lesson usually made the motor respond sooner after you moved the RV?
3. Why can the lesson 05 motor respond before its next ADC and PWM values appear in the serial monitor?
4. What hardware and timing did you keep the same to make this a fair test, and what one thing changed?

## Timer-Specific Troubleshooting

- **No serial lines appear:** Confirm that the monitor is set to 9,600 baud, close and reopen it, and press the Uno reset button once. Lesson 05 reports about once per second. Serial calls belong in `loop()`, not inside the ISR.
- **The ADC value never changes:** Switch off B1 and disconnect USB before touching any wire. Ask the teacher to verify that the RV center wiper connects to A0 and that its outer terminals connect to Arduino 5 V and shared ground. B1 positive must not connect to the RV or A0.
- **The ADC changes but the PWM value does not:** Confirm that the Timer1 ISR maps the 0–1023 ADC range to 0–255 and stores both `latestAdcReading` and `latestPwmCommand` after each reading.
- **The motor stays still even when the reported PWM value is high:** Immediately switch off B1 and disconnect USB. Ask the teacher to check the B1 batteries, shared ground, D3 gate path, MOSFET pins, motor-current path, and 1N5817 polarity before reconnecting power.
- **The motor changes only when a new serial line appears:** Move the RV immediately after a report. In lesson 05, the motor should react within about 100 milliseconds—well before the next one-second report. If it waits for the next report, confirm that `OCR1A` is `24999`, Timer1 CTC and the 64:1 prescaler are enabled, the compare-A interrupt is enabled, and `analogRead()` plus `analogWrite()` remain inside `ISR(TIMER1_COMPA_vect)`.
- **The motor responds during the wait but the displayed values have not changed yet:** That is expected evidence that Timer1 is working. The ISR updates the motor every 100 milliseconds, while `loop()` shows only one snapshot every 1,000 milliseconds.
- **The motor or Uno stutters or resets:** Remove both power sources. Ask the teacher to check for loose motor-current wiring, a missing shared ground, weak B1 batteries, or a 1N5817 that is not directly across the motor with the documented polarity.
- **Any part becomes hot, smells unusual, sparks, smokes, or moves unexpectedly:** Switch off B1 and disconnect USB immediately. Tell the teacher, do not touch hot parts, and do not restore power until the circuit has been checked.
