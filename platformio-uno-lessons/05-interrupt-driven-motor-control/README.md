# Lesson 05: Interrupt-Driven Motor Control

This lesson lets the same RV-controlled motor check for a changed instruction more often. Lesson 04 checks about once each second; lesson 05 checks **ten times each second**, even though both show computer messages about once a second.

## Recommended Prior Learning

Complete [lesson 04: ADC PWM Motor Control](../04-adc-pwm-motor-control/) first. Reuse its teacher-approved **protected motor circuit**, Snap Circuits RV, B1 two-AA motor supply, and Arduino USB connection. The comparison changes the **uploaded program**, not the wiring.

## Core Mission: Can the Motor Notice an RV Change Sooner?

**Goal:** Compare one low-to-high RV movement in lessons 04 and 05, made just after a computer message. Watch whether the **motor command** can change before the next one-second message in lesson 05.

**Prepare and stay safe:** Keep **B1 off or its batteries removed and USB unplugged** before checking or changing a wire. Do not change any wire between the two tests. Use only a **bare, secured motor** with no wheel, propeller, gear, or other shaft attachment; keep hands, hair, clothing, and objects clear of the shaft. Arduino **5 V powers only the RV**; **B1 positive goes only to motor positive**, never to Arduino 5 V, VIN, A0, D3, or the RV. If the motor fails to turn when commanded high, a part or battery gets hot, there is an unusual smell, smoke or sparking, or motor or wiring moves unexpectedly, **switch B1 off and unplug USB immediately**. Tell the teacher; do not touch hot parts or restore power until the setup is checked.

1. **Teacher checkpoint, both sources off:** The teacher traces RV outer terminals to Arduino **5 V and GND**, center wiper to **A0**, D3 through the **220–330 Ω gate resistor** to the RFP30N06LE **Gate**, its **10 kΩ pulldown** to shared ground, motor negative to **Drain**, and **Source, B1 negative, and Arduino GND** to shared ground. Verify the **1N5817 banded end at motor/B1 positive** and unbanded end at motor/Drain negative. Confirm the separate positive supplies, secured bare motor, and clear shaft. Power on only after approval.
2. With **B1 still off**, connect Uno USB using a data cable. Open `platformio-uno-lessons/04-adc-pwm-motor-control` in VS Code, then choose **Build** and **Upload** under **Project Tasks > uno > General**; wait for `SUCCESS` after each. Open **Monitor** at **9,600 baud**. Set the RV near an ADC and PWM command of zero. Ask the teacher to recheck the clear shaft and enable B1.
3. Just after a new monitor line, slide the RV quickly from near zero to a **clearly higher setting** that makes the approved motor move. Hold it still. Notice when the motor's sound or motion changes and when the next line appears. Record your actual observation, then **switch B1 off and unplug USB**. If the motor does not turn at a high command, stop both power sources and ask the teacher to inspect it.

**Notice from lesson 04:** Did the motor seem to keep its old command until the next line? ______________________________

4. **Predict from lesson 04:** If the program checks the RV every **100 ms** instead of every **1,000 ms**, will the motor's command change sooner? Write a reason from the first trial. ______________________________
5. With **B1 off and USB unplugged**, leave every wire fixed. Close Monitor and ask the teacher to confirm the same approved circuit and clear shaft. Open `platformio-uno-lessons/05-interrupt-driven-motor-control`, choose **Build** and wait for `SUCCESS`. Reconnect USB, choose **Upload** and wait for `SUCCESS`, then reopen **Monitor** at the same **9,600 baud**. Return the RV near a zero command. After a final clear-shaft check, the teacher enables B1.
6. Just after a new line, make the **same** low-to-high RV movement. Hold the setting still and compare the motor's first change with the next line. Both programs print about once a second. **Switch B1 off first, close Monitor, then unplug USB.** Change a wire only after both sources are off.

| Paired trial | When did the motor's sound or motion change? | Did it change before the next message? |
| --- | --- | --- |
| Lesson 04: 1,000 ms checks |  |  |
| Lesson 05: 100 ms checks |  |  |

**Notice:** Which trial seemed quicker, or was it hard to tell? Use your observations, even if they differ from your prediction. A motor may need time to start moving or may coast after a command changes; these effects are different from how often the program checks the RV.

**Stop safely:** B1 is off and USB is unplugged. Leave the approved circuit fixed. For a fuller investigation with a student-written hypothesis and matching trials, use the [printable robot-attention worksheet](robot-attention-worksheet.pdf).

## Name the Idea: The Robot Keeps Checking

In lesson 04, the program **reads A0, maps the ADC number to PWM, applies the D3 command, reports the numbers, then waits 1,000 ms**. If the RV moves just after the read, the previous motor power command stays in effect until the next check—possibly about one second. Lesson 05 still waits about one second **between reports**, but a timer calls a brief checking routine, called an **ISR**, every **100 ms**. That routine reads A0 and adjusts the D3 PWM command **ten times a second**, including during the main loop's wait. The next computer message can come later than the motor's command change. These ten **control updates** are different from the much faster on/off pulses that make the selected PWM output. The deeper [timer and shared-data reference](timer-teacher-reference.md) explains the real implementation for teachers and advanced readers; it is not needed for the student comparison.

| One-second timeline (● = RV read and D3 command update) | 0 ms | 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900 | 1,000 ms: next second |
| --- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Lesson 04 | ● |  |  |  |  |  |  |  |  |  | ● |
| Lesson 05 | ● | ● | ● | ● | ● | ● | ● | ● | ● | ● | ● |

The sketch shows **one check versus ten checks during the same second**. The final column starts the next second; real checks and reports are approximate. A robot should follow its plan **and** keep noticing changed controls or surroundings. A robot with suitable sensors might need frequent obstacle or temperature checks to avoid collisions or overheating; the RV here tests only a changing **control**, not those hazards.

## Printable Robot Attention Investigation

Compare lessons 04 and 05 using the same B1 battery, RV control, and protected motor circuit. The three-page activity guides a student-written hypothesis, matching trials, and a conclusion about the motor's response to 1000 ms versus 100 ms control checks.

- [Printable student worksheet (PDF)](robot-attention-worksheet.pdf)
- [Student worksheet source (HTML)](robot-attention-worksheet.html)
- [Teacher guide: preparation, experiment steps, and answer guidance](robot-attention-teacher-guide.md)

## Next Lesson: Two Joystick Controls

Continue with [lesson 06: joystick motor and servo control](../06-joystick-motor-servo-control/README.md) to control motor power and servo position with two joystick axes checked every 20 milliseconds. Lesson 06 uses a new regulated 5 V actuator supply and joystick circuit. Prepare that circuit with its [teacher guide](../06-joystick-motor-servo-control/teacher-guide.md).

## Deeper Circuit Reference

The component list, exact wiring tables, and full six-trial comparison below remain available for setup and further practice. The [timer teacher reference](timer-teacher-reference.md) holds register calculations, shared-data handling, and timer diagnosis.

### What You Need

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

### Identify the RFP30N06LE Pins

First confirm that the part marking says **RFP30N06LE** or **P30N06LE**. Other TO-220 parts can look identical while using different electrical connections.

Hold the MOSFET with its flat, printed face toward you and its three metal leads pointing down. In that reference view, the leads are numbered from left to right:

| Terminal | Pin | Function in these lessons |
| --- | --- | --- |
| Gate (G) | 1, left lead | The control input. Arduino D3 reaches this terminal through the gate resistor, and the pulldown resistor keeps it at ground when it is not being driven. |
| Drain (D) | 2, center lead | The switched-current terminal associated with the motor-negative side of the circuit. |
| Source (S) | 3, right lead | The return terminal associated with shared ground. |
| Drain flange/tab | Metal tab behind the plastic body | Electrically the same Drain node as pin 2; it is not an insulated mounting tab. |

Use the terminal names and functions when tracing the circuit. Do not rely only on “left,” “middle,” and “right,” because turning the part around reverses its apparent order. The [RFP30N06LE manufacturer datasheet](https://www.sparkfun.com/datasheets/Components/General/RFP30N06LE.pdf) shows the TO-220AB Gate, Drain, Source, and Drain-flange assignments.

### Connect the Low-Side Motor and Gate Paths

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

### Connect the RV Voltage Divider to A0

Make these connections only while USB is disconnected and B1 is switched off or has its batteries removed:

| RV terminal | Arduino connection | Purpose |
| --- | --- | --- |
| One outer terminal | Arduino 5 V | Supplies the high side of the RV from the Arduino power domain. |
| Other outer terminal | Arduino GND/shared ground | Supplies the 0 V side of the RV. |
| Center wiper | Arduino A0 | Provides the adjustable input read by the timer interrupt. |

The two outer terminals may trade places; swapping them changes which adjustment direction raises the A0 reading. The center wiper must still connect to A0.

**Never connect B1 positive to the RV, A0, Arduino 5 V, or VIN.** The RV uses Arduino 5 V, while B1 positive belongs only to the motor-positive power path. The two power domains share only the documented ground connection.

### Add the 1N5817 Flywheel Diode

Place the 1N5817 directly across the motor terminals, as close to the motor connections as the breadboard permits.

| Diode end | Connect to |
| --- | --- |
| Cathode (banded end) | Motor positive, which is the same node as B1 positive |
| Anode (unbanded end) | Motor negative, which is the same node as MOSFET Drain pin 2 |

During normal motor power, this orientation keeps the diode from conducting. When PWM turns the MOSFET off, the motor's stored magnetic energy can keep current circulating through the diode instead of producing a damaging voltage spike.

Do not reverse the diode. A reversed flywheel diode would conduct across B1 when the MOSFET turns on, creating a short circuit.

### Complete the Safety Check Before Powering the Circuit

1. Disconnect the USB cable and switch off or remove the batteries from B1. Build or change the circuit only while **both** power sources are disconnected.
2. Use only the bare motor. Do not attach a wheel, propeller, gear, or anything else to its shaft.
3. Secure the motor so it cannot roll, jump, or pull wires loose. Keep hands, hair, clothing, and other objects away from the shaft.
4. Ask the teacher to trace every connection before power is applied. The teacher must verify the MOSFET terminals, gate resistors, RV-to-A0 voltage divider, shared ground, separate positive supplies, and the 1N5817 band toward B1/motor positive.
5. Connect USB and enable B1 only after the teacher approves the circuit. Remove both power sources again before moving a wire or component.

Immediately switch off B1 and disconnect USB if the motor does not turn when commanded to run, any component or battery becomes hot, there is an unusual smell, smoke or sparking appears, or the motor or wiring moves unexpectedly. Tell the teacher and do not touch a hot component or reconnect power until the circuit has been checked.

### Optional Extended Lesson 04/05 Comparison

This is a fair comparison: keep the same RV, motor circuit, B1 supply, and USB-powered Arduino for both programs. Do not move any wires between tests. The RV must keep one outer terminal on Arduino 5 V, its other outer terminal on shared ground, and its center wiper on A0. The protected motor circuit must keep the same D3 gate path, separate B1 positive motor supply, shared ground, and flywheel diode used in lesson 04.

Both lessons print one serial report about every 1,000 milliseconds. The important observation is how soon the **motor** responds between those reports: lesson 04 updates its command only once per 1,000-millisecond loop, while lesson 05's timer updates it every 100 milliseconds.

1. With B1 off and the same approved wiring fixed, reconnect USB if it is unplugged. Close any open Monitor. Open `platformio-uno-lessons/04-adc-pwm-motor-control` in VS Code, select **Build** under PlatformIO's **Project Tasks > uno > General**, wait for `SUCCESS`, and then select **Upload**.
2. Open the serial monitor at 9,600 baud. Adjust the RV until the reported ADC and PWM values are near zero.
3. Confirm that the bare motor is still secured and the wiring still has teacher approval, then switch on B1.
4. Just after a new serial line appears, move the RV quickly from near minimum to near maximum. Watch the motor without touching it and record whether its response seems immediate or delayed. Repeat the test three times, then repeat three times from near maximum to near minimum.
5. Switch off B1 without changing the circuit and close Monitor. Open `platformio-uno-lessons/05-interrupt-driven-motor-control` in VS Code, select **Build** and wait for `SUCCESS`, select **Upload** and wait for `SUCCESS`, and then reopen the serial monitor at the same 9,600-baud setting. With the motor still secured, ask the teacher to approve B1 power again before the next trials.
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

### Student Troubleshooting and Stop Conditions

- **No serial lines appear:** Confirm that Monitor is set to 9,600 baud, close and reopen it, and press the Uno reset button once. Lesson 05 reports about once per second. Ask the teacher to inspect the uploaded project if it stays blank.
- **The ADC value never changes:** Switch off B1 and disconnect USB before touching any wire. Ask the teacher to verify that the RV center wiper connects to A0 and that its outer terminals connect to Arduino 5 V and shared ground. B1 positive must not connect to the RV or A0.
- **The ADC changes but the PWM value does not:** Switch off B1 and unplug USB. Ask the teacher to check the lesson 05 program and the [timer reference](timer-teacher-reference.md#teacher-timer-diagnosis).
- **The motor stays still even when the reported PWM value is high:** Immediately switch off B1 and disconnect USB. Ask the teacher to check the B1 batteries, shared ground, D3 gate path, MOSFET pins, motor-current path, and 1N5817 polarity before reconnecting power.
- **The motor changes only when a new serial line appears:** Try a clearly separated RV movement just after a report, from a safe distance. Physical motion can lag because the motor needs time to start or coast. If the pattern persists, switch off B1 and unplug USB; ask the teacher to use the [timer diagnosis](timer-teacher-reference.md#teacher-timer-diagnosis).
- **The motor responds during the wait but the displayed values have not changed yet:** That is expected evidence that Timer1 is working. The ISR updates the motor every 100 milliseconds, while `loop()` shows only one snapshot every 1,000 milliseconds.
- **The motor or Uno stutters or resets:** Remove both power sources. Ask the teacher to check for loose motor-current wiring, a missing shared ground, weak B1 batteries, or a 1N5817 that is not directly across the motor with the documented polarity.
- **Any part becomes hot, smells unusual, sparks, smokes, or moves unexpectedly:** Switch off B1 and disconnect USB immediately. Tell the teacher, do not touch hot parts, and do not restore power until the circuit has been checked.
