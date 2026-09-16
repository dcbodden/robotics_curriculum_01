# Lesson 02: Manual Voltage Mapping

In this standalone, non-code lesson, you will measure changing voltage and use a lookup table to sort each reading into one of eight digital levels. Each decimal level from 0 through 7 has a matching 3-bit value.

## Learning Goals

By the end of the lesson, you will be able to:

- measure several points along a changing voltage range;
- use a lookup table to choose a decimal level;
- match each decimal level with a 3-bit value; and
- explain how many nearby voltages can share the same digital level.

## Core Mission: Measure One Voltage and Find Its Level

**Goal:** Safely measure one RV wiper voltage and use the [voltage-range lookup table](#voltage-range-lookup-table) to give it a decimal level and 3-bit label. This is a hands-on review after [Uno lesson 02: ADC Variable Delay](../../platformio-uno-lessons/02-adc-variable-delay/).

**Prepare and stay safe:** Use a 50K Ω RV Snap, an Arduino Uno, USB data cable, and digital multimeter. The Uno supplies 5 V and GND **through USB only**; do not connect B1, another battery, or external power. Do all wiring with USB disconnected. Never move a probe, wire, or component while USB is connected; move only the RV control. If parts become hot, a connection is loose, or the reading is unstable or outside 0–5 V, disconnect USB and ask the teacher to inspect the setup.

1. Based on the changing ADC value you observed in Uno lesson 02, write your prediction in the [recording activity](recording-activity.md#predict).
2. **Remove power and set the meter.** Disconnect USB from the Uno. Turn the meter off. Put black in **COM** and red in the **VΩ** voltage/resistance jack, never a current jack. Set the dial to **DC voltage** on a teacher-approved range that includes 5 V; do not use AC voltage, resistance, or current mode for this powered measurement.
3. **Wire with USB still disconnected.** Connect Arduino 5 V to one **outer** RV terminal and Arduino GND to the other **outer** RV terminal. Put the black probe on Arduino GND and the red probe on the RV **center wiper**. Keep probe tips and bare metal apart so 5 V cannot touch GND directly.
4. **Teacher checkpoint:** With USB disconnected, ask the teacher to identify the two outer terminals and center wiper, trace every connection, confirm no battery or external voltage is attached, check that black is in COM and red in VΩ, and verify the DC voltage range. Connect USB **only after teacher approval**.
5. Move only the RV control gently to one endpoint without forcing it. Wait for a steady meter reading. Record the actual voltage and decide whether it is the worksheet's **Minimum** or **Maximum** row. An endpoint does not have to read exactly 0 V or 5 V.
6. Find the row containing your measured voltage in the [lookup table](#voltage-range-lookup-table). Copy that row's decimal level and 3-bit value into the same worksheet row. An exact range boundary belongs to the row that **starts** at that voltage.
7. **Stop safely:** Disconnect USB **before** moving or removing any probe or wire. Turn the meter off. Leave the circuit unpowered for your next setup check.

**Notice:** What voltage did you measure, and which two labels did the lookup table give it?

Measured voltage: ______ V. Decimal level: ______. 3-bit value: ______.

**Stop with USB disconnected.** To complete the full lesson, continue with the [required eight-level survey](#measure-and-look-up-all-eight-levels), using the same teacher-approved wiring and repeating the power-off checkpoint before any connection change. The [electrical and digital words](#electrical-and-digital-words-you-will-use), [bucket explanation](#think-of-the-ranges-as-digital-buckets), [Uno ADC comparison](#how-this-model-connects-to-the-uno-adc), and [uncommon troubleshooting](#troubleshooting--disconnect-usb-before-making-changes) follow the immediate mission.

## Who Does What

- **Student actions:** Directions that say **you** are for the student. You may make predictions, wire only with USB disconnected, move the RV control after approval, read the meter, use the lookup table, and record observations. Stop whenever a step says **teacher checkpoint** or **ask the teacher**.
- **Teacher-only checks:** The teacher identifies the RV terminals, verifies all wiring and meter settings, confirms that no external power is connected, and decides when USB power may be connected. A student must not skip or perform a teacher-only check.

## Electrical and Digital Words You Will Use

- **Voltage**, measured in volts (`V`), is the electrical push that can make current flow. **Resistance**, measured in ohms (`Ω`), tells how strongly a part limits current. `50K Ω` means 50 kilo-ohms, or 50,000 ohms.
- The **RV Snap**, also called a **variable resistor** or **potentiometer**, is a resistor adjusted with a control. Its **terminals** are its metal connection points. The center terminal is the **wiper**, which moves electrically as the control moves.
- A **voltage divider** uses the RV's resistance to make an adjustable voltage between 0 V and 5 V at the wiper.
- **GND**, pronounced “ground,” is the circuit's 0 V reference point. **DC**, or direct current, flows in one main direction and is the type of voltage used in this lesson.
- A **short circuit** is an unintended path with too little resistance, such as connecting 5 V directly to GND. It can allow too much current and damage equipment.
- **Decimal** is the everyday number system using digits 0 through 9. **Binary** uses only 0 and 1. A **bit** is one binary digit.
- An **ADC**, or analog-to-digital converter, measures a smoothly changing voltage and gives it a numbered digital value.

## Think of the Ranges as Digital Buckets

The RV voltage can change smoothly through many different values. To make a simpler digital model, this lesson sorts those values into eight ranges. Imagine eight buckets lined up from the lowest voltage to the highest voltage: each measured voltage drops into the one bucket whose range contains it.

Every voltage in the same bucket receives the same **decimal level**, even when the measured voltages are a little different. The buckets are numbered 0 through 7 because digital systems commonly begin counting at 0.

Each decimal level also has a **3-bit value**. A **bit** is one binary digit, so it can be only `0` or `1`. Three bits can form eight different patterns—one for each bucket:

| Decimal level | 3-bit value |
| ---: | :---: |
| 0 | `000` |
| 1 | `001` |
| 2 | `010` |
| 3 | `011` |
| 4 | `100` |
| 5 | `101` |
| 6 | `110` |
| 7 | `111` |

The decimal level and 3-bit value are two labels for the same bucket. The lesson lookup table will tell you which bucket contains each measured voltage.

## Voltage-Range Lookup Table

Each bucket covers 0.625 V. Find the row containing the measured voltage, then copy that row's decimal level and 3-bit value.

| Voltage range | Decimal level | 3-bit value |
| --- | ---: | :---: |
| 0.000 V to less than 0.625 V | 0 | `000` |
| 0.625 V to less than 1.250 V | 1 | `001` |
| 1.250 V to less than 1.875 V | 2 | `010` |
| 1.875 V to less than 2.500 V | 3 | `011` |
| 2.500 V to less than 3.125 V | 4 | `100` |
| 3.125 V to less than 3.750 V | 5 | `101` |
| 3.750 V to less than 4.375 V | 6 | `110` |
| 4.375 V through 5.000 V | 7 | `111` |

An exact boundary goes in the row that **starts** with that voltage. For example, exactly 0.625 V belongs to level 1, exactly 1.250 V belongs to level 2, and exactly 4.375 V belongs to level 7. The maximum value, exactly 5.000 V, is also included in level 7.

## Wire the 50K Ohm RV Snap Voltage Divider

The RV Snap is a 50K ohm variable resistor with three terminals. Its two **outer terminals** connect across the Arduino's 5 V supply. Its **center terminal**, called the **wiper**, provides the changing voltage that you will measure.

Use only the Arduino's USB-powered 5 V supply for this activity. Do not connect B1, another battery, or any other external power supply.

1. Disconnect the Arduino's USB cable. The Arduino must have no power while you build or change the circuit.
2. Turn the multimeter off. Put the black lead in **COM** and the red lead in the voltage/resistance jack, usually marked **VΩ**. Do not put the red lead in a current jack.
3. Connect the RV Snap and meter as shown in this table:

   | From | To |
   | --- | --- |
   | Arduino 5 V | One outer RV terminal |
   | Arduino GND | The other outer RV terminal |
   | Multimeter black probe | Arduino GND, the 0 V reference point |
   | Multimeter red probe | RV center wiper, the voltage measurement point |

4. Set the meter to **DC voltage** on a range that can include 5 V. Do not use resistance, current, or AC voltage mode for these powered measurements.
5. Keep the probe tips and bare metal connections from touching one another. A direct connection from Arduino 5 V to GND would create a short circuit.
6. **Teacher checkpoint:** With USB still disconnected, ask the teacher to identify the two outer RV terminals and center wiper, trace every table connection, check the meter's probe jacks and DC voltage range, and confirm that no battery or external voltage is connected.
7. Connect the USB cable only after the teacher approves the complete setup. Do not move a probe, wire, or component while USB power is connected.

The meter now measures the RV wiper voltage relative to Arduino GND. Arduino A0 is not needed: the Uno provides only 5 V and GND, and this lesson requires no program, build, or upload. When you finish measuring, disconnect USB before removing probes or changing any connection.

## Measure and Look Up All Eight Levels

1. Complete the **Predict** section in [`recording-activity.md`](recording-activity.md) before taking a measurement.
2. With USB disconnected, complete **Wire the 50K Ohm RV Snap Voltage Divider** above. Stop at the teacher checkpoint and wait for approval before connecting USB.
3. After the teacher approves the setup, connect USB. Keep the meter probes and all wires in place while power is connected; move only the RV control.
4. Move the RV control gently to one endpoint without forcing it. Wait for the meter reading to become steady. If this is the lower endpoint voltage, record the actual reading in the worksheet's **Minimum** row. If it is the higher endpoint, use the **Maximum** row.
5. Find that measured voltage in the **Voltage-Range Lookup Table**. Copy the decimal level and 3-bit value from the same lookup-table row into the worksheet.
6. Move the RV control gently to its other endpoint. Wait for a steady reading, record the other **Minimum** or **Maximum** value, and look up its decimal level and 3-bit value.
7. Slowly move the RV control between the endpoints. Stop at **at least six additional positions**, wait for a steady voltage at each one, and record every measured voltage.
8. For each additional voltage, find the one lookup-table range that contains it and record that range's decimal level and 3-bit value. Remember that an exact boundary belongs to the range that starts at that value.
9. Check the decimal-level column. It must contain every level from 0 through 7 at least once. If a level is missing, use the lookup table to identify its voltage range, carefully adjust the RV until the meter shows a voltage in that range, and record an additional reading. Six additional readings is the minimum, not the limit.
10. When all eight levels are represented, disconnect USB before removing a probe or changing a connection. Then complete the worksheet's comparison questions.

## How This Model Connects to the Uno ADC

This eight-level activity is a small model of the Arduino Uno's **analog-to-digital converter**, or **ADC**. Both take a voltage that can change smoothly and represent it with one numbered digital choice.

- This manual model uses 3 bits, giving eight choices numbered 0 through 7.
- The Uno ADC uses 10 bits, giving 1,024 choices numbered 0 through 1023.

Having more choices lets the Uno tell apart smaller voltage changes. Imagine replacing eight large buckets with 1,024 much smaller buckets: nearby voltages that share one bucket in this activity might land in different Uno ADC buckets.

You do not need to calculate an Uno ADC value or write Arduino code in this lesson. Use the eight-row voltage lookup table to practice the same main idea by hand.

## Troubleshooting — Disconnect USB Before Making Changes

| Problem | Safe check |
| --- | --- |
| **The endpoint readings are not exactly 0 V and 5 V.** | This is normal because real parts and measurements are not perfect. Record the actual minimum and maximum; do not force the RV control. Tell the teacher if a reading is below 0 V or above 5 V. |
| **The voltage rises in the opposite adjustment direction from the one expected.** | This is normal and depends on which outer RV terminal connects to 5 V. You may use it as wired. If the teacher chooses to reverse the direction, disconnect USB before swapping the two outer-terminal connections. |
| **The voltage reading jumps or will not settle.** | Hold the RV control still and wait. If the reading remains unstable, disconnect USB and turn off the meter. Ask the teacher to check the wiper connection, Arduino GND connection, probe contact, and loose wires before restoring power. |
| **The meter shows an unexpected or missing reading.** | Disconnect USB and turn off the meter. Confirm that black is in **COM**, red is in **VΩ**, and the dial is on DC voltage with a range that includes 5 V—not resistance, current, or AC voltage. Ask the teacher to verify the setup before reconnecting USB. |
| **A displayed voltage is exactly on or very near a lookup-table boundary.** | The display may round a value. If it displays the exact boundary, use the higher range—the row that starts with that number. If it flickers across the boundary, hold the control still; after it settles, record the displayed value and use the matching row. |

This lesson uses the Arduino only as a source for the lesson's low voltage. It does not require programming, building firmware, or uploading firmware.
