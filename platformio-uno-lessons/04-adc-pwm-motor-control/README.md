# Lesson 04: ADC PWM Motor Control

This lesson uses the adjustable RV voltage from lesson 02 to control the PWM command sent to an externally powered DC motor.

## Recommended Prior Learning

Complete [Uno lesson 02](../02-adc-variable-delay/) to see the RV change an ADC reading, then [Uno lesson 03](../03-pwm-motor-ramp/) to see the approved D3 PWM motor circuit. Reuse lesson 03's teacher-approved **protected motor circuit**. Add only the lesson 02 RV input after removing all power.

## Core Mission: Turn the RV, Watch the Motor Command

**Goal:** See one RV movement change the printed **ADC reading**, the **PWM command**, and the secured motor's response. Notice that the program checks the RV only **once every 1,000 ms**, so a change just after a check can take about one second to affect the motor command.

**Prepare and stay safe:** Keep **USB unplugged and B1 switched off or its batteries removed** before adding, checking, or changing any wire. Use only a **bare, secured motor** with no wheel, propeller, gear, or other shaft attachment; keep hands, hair, clothing, and loose objects clear of the shaft. Never connect **B1 positive** to Arduino 5 V, VIN, A0, D3, or the RV. Arduino 5 V powers only the RV input; the motor uses B1 and shares ground with the Uno. If the commanded motor never turns, any part or battery gets hot, there is an unusual smell, smoke or sparking, or the motor or wiring moves unexpectedly, **switch off B1 and unplug USB immediately**. Tell the teacher; do not touch a hot part or restore power until the setup is checked. Never move wires while powered.

1. With **both power sources off**, keep the approved lesson 03 motor, MOSFET, two gate resistors, flywheel diode, D3 control, and shared-ground connections fixed. Connect the Snap Circuits RV's two outer terminals to **Arduino 5 V and GND** and its center **wiper to A0**, as in lesson 02. Its sliding direction may be reversed. See the [RV wiring table](#connect-the-rv-voltage-divider-to-a0) if needed.
2. **Teacher checkpoint, still unpowered:** The teacher traces the RV-to-A0 voltage divider; confirms no 5 V-to-GND short; verifies the RFP30N06LE **Gate, Drain, Source**, the **220–330 Ω series gate resistor** and **10 kΩ gate pulldown**, the shared ground, and **B1 positive only at motor positive**. Confirm the **1N5817 banded end at motor/B1 positive** and its unbanded end at motor/Drain negative. Check that the bare motor cannot roll or pull wires loose and the shaft area is clear. Restore power only after approval.
3. Open `platformio-uno-lessons/04-adc-pwm-motor-control` as the PlatformIO project in VS Code. Under **Project Tasks > uno > General**, choose **Build** and wait for `SUCCESS`. Keep B1 off, connect Uno USB using a **data** cable, choose **Upload**, and wait for `SUCCESS`. Then choose **Monitor** at **9,600 baud**. A new line appears about once a second, such as `ADC reading: 512 | PWM command: 127`.
4. With B1 **still off**, move the RV until both printed numbers are near zero. Check the motor's secure position and clear shaft again; ask the teacher for a final power check. After approval, the teacher enables B1 without moving any wire. Record the near-zero line and whether the motor is stopped.
5. Slide the RV toward a **much higher ADC reading**, without touching wires or the motor. Hold it still until at least two new lines appear. Record the latest ADC and PWM numbers and what the motor actually does. If it does not turn even at a high command, **switch off B1 and unplug USB** and ask the teacher to check the circuit before continuing.

| RV setting | ADC reading | PWM command | Motor observation |
| --- | ---: | ---: | --- |
| Near zero |  |  |  |
| Much higher |  |  |  |

**Notice:** Did the higher ADC number produce a higher PWM command? Did the motor start or change its sound or motion? The PWM number is a **command**, not an exact motor-speed reading.

6. **One quick comparison:** From the higher setting, predict whether sliding the RV quickly back near zero will change the motor command **right away** or at the **next one-second check**. Try only that slider movement. Watch the next printed line and listen or watch from a safe distance. The previous PWM command stays in effect during the wait; the time you notice may vary depending on when you moved the RV.

**Stop safely:** Switch off B1 first, close Monitor, then unplug USB. Change or remove wires only after **both** power sources are off.

## Name the Idea: Read, Map, Apply, Report, Wait

The focus instructions in [`src/main.cpp`](src/main.cpp) are `analogRead(ANALOG_INPUT_PIN)`, `map(...)`, `analogWrite(MOTOR_PWM_PIN, pwmCommand)`, the labeled `Serial` report, and `delay(CONTROL_UPDATE_DELAY_MS)`. The real code uses names for A0, D3, and the 1,000-millisecond wait. Each pass reads the RV once, turns its **0–1023 ADC number** into a **0–255 PWM command**, applies that command to D3, and reports both numbers. During the one-second wait, **D3 continues the selected PWM output** (switching for commands between fully off and fully on), but the program does not read the RV or choose a new command. If you move the RV just after the read, the motor may keep its previous command for about one second. Explain whether your quick comparison matched your prediction: ______________________________

## Optional Robot Attention Investigation

Compare lessons 04 and 05 using the same B1 battery, RV control, and protected motor circuit. The three-page activity guides a student-written hypothesis, matching trials, and a conclusion about the motor's response to 1000 ms versus 100 ms control checks.

- [Printable student worksheet (PDF)](../05-interrupt-driven-motor-control/robot-attention-worksheet.pdf)
- [Student worksheet source (HTML)](../05-interrupt-driven-motor-control/robot-attention-worksheet.html)
- [Teacher guide: preparation, experiment steps, and answer guidance](../05-interrupt-driven-motor-control/robot-attention-teacher-guide.md)

## Deeper Reference: Circuit and Control Timing

The sections below show component details, full wiring paths, an extended timing explanation, optional extra measurements, and troubleshooting. You can finish the Core Mission before reading them.

### Why the Control Update Waits

Each pass through the program reads A0, maps that ADC reading to a PWM value, sends the new value to D3, and prints the ADC and PWM values. It then calls `delay(1000)` before starting the next pass. The one-second pause spaces the serial-monitor lines far enough apart to make them easy to read.

The delay is **blocking**, which means the main program waits instead of taking another reading. If you move the RV just after A0 is sampled, the motor keeps the previous PWM command until the next pass through the program, so its response can lag by as much as about one second. This longer, intentionally sluggish response is easier to observe and gives you something clear to compare with the faster control updates in lesson 05.

The Arduino's PWM output keeps the already selected command during the delay. D3 switches for commands between fully off and fully on; only the reading and motor-command update wait.

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

The three-terminal RV works as a **voltage divider**: its two outer terminals connect across the Arduino's 5 V supply, and its center terminal, called the **wiper**, provides an adjustable voltage for A0.

Make these connections only while USB is disconnected and B1 is switched off or has its batteries removed:

| RV terminal | Arduino connection | Purpose |
| --- | --- | --- |
| One outer terminal | Arduino 5 V | Supplies the high side of the RV from the Arduino power domain. |
| Other outer terminal | Arduino GND/shared ground | Supplies the 0 V side of the RV. |
| Center wiper | Arduino A0 | Provides an adjustable input between approximately 0 V and 5 V. |

The two outer terminals may trade places; swapping them changes which adjustment direction raises the A0 reading. The center wiper must still connect to A0.

**Use Arduino 5 V only for the RV's high side. Never connect B1 positive to an RV terminal, A0, Arduino 5 V, or VIN.** B1 positive belongs only to the motor-positive power path. The two circuits share ground through B1 negative and Arduino GND, but their positive supplies remain separate.

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

### Optional Five-Position Recording

The ADC reading shows the RV wiper voltage as a number from 0 through 1023. The program maps that reading directly to a PWM command from 0 through 255, so a higher ADC reading should produce a higher PWM command. The motor may not begin turning at the lowest nonzero commands, and its speed may not increase evenly with the numbers.

If you stopped after the Core Mission, use the **same approved wiring** without moving any lead. Reconnect USB with B1 off, confirm the motor is still secured and the shaft is clear, then ask the teacher to enable B1 before trying these extra positions.

1. Begin with the RV adjusted near an ADC reading of 0. Keep your hands away from the secured motor and its shaft.
2. Move the RV to each position in the table below. At every position, hold the control still until at least two new serial lines appear, then record the newest ADC and PWM values and observe the motor.
3. Starting near zero again, increase the RV slowly. Record the first position where the motor begins turning as the starting threshold.
4. Move the RV quickly between two far-apart positions. Notice that the serial report and motor command change only on the next one-second update; the response can therefore feel sluggish.
5. When the activity is complete, switch off B1 first, close the serial monitor, and then disconnect USB. Do not change the circuit until both power sources are removed.

| RV position | ADC reading (0–1023) | PWM command (0–255) | Motor behavior: stopped, starting, or turning | Sound or motion observation |
| --- | ---: | ---: | --- | --- |
| Near ADC minimum |  |  |  |  |
| Low |  |  |  |  |
| Middle |  |  |  |  |
| High |  |  |  |  |
| Near ADC maximum |  |  |  |  |
| Starting threshold |  |  |  |  |

After recording the readings, answer these questions:

1. As the ADC reading increased, what happened to the PWM command?
2. A PWM command is roughly one quarter of its ADC reading. Do your recorded pairs follow that relationship?
3. At what ADC reading and PWM command did the motor first begin turning?
4. Did larger PWM commands always produce an equally large change in the motor's sound or motion? Explain what you observed.
5. When you moved the RV quickly, where did you notice the one-second delay: in the reported numbers, the motor response, or both?

### Troubleshooting

- **Build does not end with `SUCCESS`:** Make sure VS Code opened the `04-adc-pwm-motor-control` folder that contains this lesson's `platformio.ini`. Run **Build** again and read the first error in the terminal.
- **Upload cannot find the Uno:** Use a known USB data cable, try a direct computer USB port, close any serial monitor or other program using the Uno's port, and try **Upload** again.
- **The serial monitor is blank or unreadable:** Confirm the monitor speed is 9,600 baud, close and reopen **Monitor**, and press the Uno reset button once. A labeled line should appear about every 1,000 milliseconds.
- **The ADC reading does not change with the RV:** Switch off B1 and disconnect USB before changing any wire. Ask the teacher to check that the RV's center wiper connects to A0 and its outer terminals connect to Arduino 5 V and shared ground. B1 positive must not connect to the RV or A0.
- **The PWM command does not follow the ADC reading:** Confirm that `src/main.cpp` maps the 0–1023 ADC range directly to the 0–255 PWM range. The PWM value should be approximately one quarter of the ADC value, with 0 mapping to 0 and 1023 mapping to 255.
- **The numbers change but the motor never turns:** Immediately switch off B1 and disconnect USB. Ask the teacher to check the B1 batteries, shared ground, D3 gate path, MOSFET pin functions, motor-current path, and diode polarity before reconnecting power.
- **The numbers change but the motor stays at one speed:** Remove both power sources. Check that Arduino D3 reaches MOSFET Gate pin 1 through the 220–330 ohm resistor and that the 10 kilohm pulldown connects the Gate to shared ground.
- **The motor stays stopped at low nonzero PWM values:** This can be its normal starting threshold. Record the value where it begins turning. Stop and ask the teacher to check the circuit if it never turns at a high PWM command.
- **The motor or Uno stutters or resets:** Remove both power sources. Ask the teacher to check for loose motor-current wiring, a missing shared ground, weak B1 batteries, or a 1N5817 that is not connected directly across the motor with the documented polarity.
- **Any part becomes hot, smells unusual, sparks, smokes, or moves unexpectedly:** Switch off B1 and disconnect USB immediately. Tell the teacher, do not touch hot parts, and do not restore power until the circuit has been checked.
