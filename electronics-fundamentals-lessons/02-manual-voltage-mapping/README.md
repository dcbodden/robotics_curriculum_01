# Lesson 02: Manual Voltage Mapping

In this standalone, non-code lesson, you will measure changing voltage and use a lookup table to sort each reading into one of eight digital levels. Each decimal level from 0 through 7 has a matching 3-bit value.

## Learning Goals

By the end of the lesson, you will be able to:

- measure several points along a changing voltage range;
- use a lookup table to choose a decimal level;
- match each decimal level with a 3-bit value; and
- explain how many nearby voltages can share the same digital level.

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
