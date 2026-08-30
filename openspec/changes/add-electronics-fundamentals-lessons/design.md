## Context

The Arduino Uno lessons already ask fifth-grade students to recognize voltage levels and ADC values. The repository does not yet provide standalone, non-code lessons for using a digital multimeter or for manually sorting a voltage into digital ranges. The multimeter lesson will establish foundational electrical concepts as the first curriculum lesson. After students encounter ADC behavior in the Arduino ADC variable-delay lesson, the manual voltage-mapping activity will revisit the concept to strengthen retention and comprehension. Neither fundamentals lesson is a firmware dependency.

Current measurement presents the largest safety and teaching risk. A meter in current mode has a low-resistance path and must be inserted in series; placing it across a battery can short the source or blow the meter fuse. The supplied `electronics-fundamentals-lessons/01-multimeter-ohms-law/sample_circuit/base_layout.jpg` shows the intended B1 battery, 100 Ohm R1 fixed resistor, and S2 press-switch loop. See `proposal.md` for scope and the delta specs for observable requirements.

## Goals / Non-Goals

**Goals:**

- Establish a numbered home for standalone electronics fundamentals activities.
- Teach the distinct meter setup and probe placement for voltage, resistance, and current.
- Use a low-voltage Snap Circuits circuit and require teacher checks at risky transitions.
- Give students a defensible Ohm's law comparison using a known fixed resistor.
- Let students manually map 0–5 V into eight complete, non-overlapping ranges.
- Connect decimal levels 0–7 to their 3-bit forms without requiring programming.

**Non-Goals:**

- Measure household outlets, mains-powered devices, high voltage, or high current.
- Add a lamp or another load to the B1/R1/S2 sample circuit, or require exact agreement between calculated and measured current.
- Teach internal multimeter circuitry, meter fuse replacement, significant-figure rules, or uncertainty analysis.
- Build, upload, or modify Arduino firmware.
- Expand the mapping activity to the Uno's full 1,024 ADC values; the eight-level table is a smaller manual model of the same idea.

## Decisions

### Add a separate numbered fundamentals collection

Create `electronics-fundamentals-lessons/` with a collection README and two lesson directories: `01-multimeter-ohms-law/` and `02-manual-voltage-mapping/`. Each lesson will contain its own student instructions and recording activity. This parallels the numbered Uno lesson convention while keeping non-code materials out of PlatformIO project directories.

Alternative considered: place the activities under `electronics-reference/`. A lesson collection is preferred because these materials have ordered procedures, teacher checks, and student observations rather than reference-only content.

### Use the fixed resistor as both the load and calculation component

The first lesson will follow the sample photo and assemble a teacher-verified series circuit using the low-voltage Snap Circuits B1 battery, 100 Ohm R1 fixed resistor, and momentary S2 press switch. Pressing S2 will complete the series path when the multimeter is properly connected to measure current.

The student sequence is deliberate: measure R1 with power removed and the resistor isolated, measure the voltage provided by B1, calculate and record predicted current in milliamps with $I = V / R$, and only then reconfigure the meter in series to measure current. The worksheet will compare the measured result with the prediction using approximate agreement and consistent units rather than promise identical values. The teacher guide must verify the actual B1 battery voltage, R1 resistance and power rating, expected circuit current, and a safe meter range before students begin.

Alternative considered: add a lamp as a visible current indicator. The lamp is unnecessary because the meter provides the lesson's observations, and its changing filament resistance would make the beginner circuit and Ohm's law comparison less direct.

### Make current measurement a teacher-controlled transition

Student-facing instructions will use separate checkpoints for each meter mode. Current-mode setup cannot begin until the student has recorded a predicted current in milliamps. The prediction helps the teacher confirm that the selected current input and range can accept the expected value, but it does not replace starting on the highest suitable safe range or checking the exact meter's limits. Current measurement requires power off, moving the red lead to the correct current jack, opening the circuit, placing the meter in series, and receiving teacher verification before restoring power. After the reading, power is removed before the red lead returns to the voltage/resistance jack.

Voltage measurement can be doen directly on the B1 connections. Resistance remains an across-the-part measurement where the circuit is open and no power is flowing. A prominent warning will prohibit current-mode measurements across the battery, and emphasize the importance of ensuring adequate load (resistance) in the circuit to avoid overcurrent situations.

Alternative considered: omit direct current measurement and calculate it only. Direct measurement is retained because it is part of the requested lesson, but the teacher checkpoint limits the highest-risk configuration error.

### Use eight exact, endpoint-aware voltage ranges

The second lesson will power the 50K Ohm RV Snap as a voltage divider from Arduino 5 V to GND and measure its center wiper relative to GND. The Arduino is only a regulated 5 V source in this activity; no sketch is built or uploaded. Students will use this lookup table:

| Voltage range | Decimal level | 3-bit value |
| --- | ---: | :---: |
| $0.000 \le V < 0.625$ V | 0 | `000` |
| $0.625 \le V < 1.250$ V | 1 | `001` |
| $1.250 \le V < 1.875$ V | 2 | `010` |
| $1.875 \le V < 2.500$ V | 3 | `011` |
| $2.500 \le V < 3.125$ V | 4 | `100` |
| $3.125 \le V < 3.750$ V | 5 | `101` |
| $3.750 \le V < 4.375$ V | 6 | `110` |
| $4.375 \le V \le 5.000$ V | 7 | `111` |

Internal boundaries belong to the range that begins at that value, and 5.000 V is included in the final range. Student instructions will emphasize table lookup and comparison; the floor formula is teacher-facing support, not required fifth-grade arithmetic.

Alternative considered: use levels 1–8. Levels 0–7 align directly with the eight possible 3-bit values and prepare students for zero-based ADC readings.

### Require all eight mapping outcomes

The mapping worksheet will include or elicit at least one voltage from each range, including the minimum and maximum. Students will record voltage, decimal level, and binary value, then explain in their own words how many nearby voltages can share one output level. This makes the activity about quantization rather than only turning a control and copying numbers.

### Use a foundation, application, reinforcement sequence and repository-owned artwork

The collection README and related Arduino lesson documentation will present a recommended teaching order: begin the curriculum with the multimeter and Ohm's law lesson, teach the Arduino ADC variable-delay lesson before manual voltage mapping, and use the mapping lesson afterward to reinforce the ADC concept. This teaching sequence does not create a firmware dependency: the fundamentals activities remain non-code lessons and do not require students to build or upload firmware. Any diagrams or photos added during implementation must be original repository-owned material or use assets with clearly documented permission.

## Risks / Trade-offs

- [A meter in current mode is placed across the battery] → Use power-off steps, a prominent prohibition, a teacher check of the series connection, and immediate return of the red lead after measurement.
- [A resistance reading includes parallel circuit paths] → Disconnect the battery and isolate at least one resistor terminal before measuring.
- [The B1 voltage and 100 Ohm R1 combination exceeds a resistor or meter rating] → Require the teacher guide to verify the actual battery voltage, expected current, resistor power, and meter range before publishing the final circuit.
- [Calculated and measured current differ] → Use measured B1 voltage and R1 resistance, check units, discuss meter and component tolerance, and ask for approximate comparison rather than exact equality.
- [RV Snap endpoints do not reach exactly 0 V or 5 V] → Describe measured endpoints as approximate while keeping the nominal lookup table at 0–5 V.
- [A rounded meter display appears on a range boundary] → Define each internal boundary as belonging to the higher range and include boundary examples.
- [Students confuse eight levels with the Uno's 1,024 ADC values] → Call the table a smaller model and explicitly compare 3-bit eight-level resolution with the Uno's 10-bit resolution.
