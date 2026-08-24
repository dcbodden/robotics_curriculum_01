## 1. Lesson Collection Structure

- [x] 1.1 Create `electronics-fundamentals-lessons/` with a collection README that explains the numbered, standalone, non-code lesson convention.
- [x] 1.2 Create `01-multimeter-ohms-law/` and `02-manual-voltage-mapping/` with independent student instructions and recording activities.
- [x] 1.3 Document the recommended curriculum sequence: teach the multimeter and Ohm's law lesson first, teach the Arduino ADC variable-delay lesson before manual voltage mapping, and use the mapping lesson afterward for reinforcement and comprehension; neither fundamentals lesson requires building or uploading firmware.
- [x] 1.4 Update repository structure documentation to list the new fundamentals lesson collection.

## 2. Multimeter and Ohm's Law Lesson

- [ ] 2.1 Identify the exact low-voltage Snap Circuits battery, switch, lamp, and fixed-resistor parts, then add a teacher preparation check for resistor value and power rating, expected current, lamp visibility, and meter range.
- [ ] 2.2 Add fifth-grade-friendly assembly and observation steps showing how the switch opens and closes the series path and how the lamp makes current flow visible.
- [ ] 2.3 Add a DC voltage procedure with the black lead in COM, the red lead in the voltage jack, and the probes connected in parallel across named measurement points.
- [ ] 2.4 Add an unpowered resistance procedure that disconnects the battery, isolates the fixed resistor from alternate paths, checks the meter mode and probe jacks, and measures across the resistor.
- [ ] 2.5 Add a teacher-verified current procedure that powers down before rewiring, starts on a safe current range, inserts the meter in series, prohibits connecting current mode across the battery, and returns the red lead after power is removed.
- [ ] 2.6 Explain $V = I \times R$ and guide students to calculate $I = V / R$ with consistent units using the fixed resistor's measured voltage and resistance.
- [ ] 2.7 Explain why the lamp is an observation aid rather than the fixed resistance used in the calculation, including how filament resistance changes as it heats.
- [ ] 2.8 Add a recording table for circuit state, lamp state, voltage, resistance, measured current, calculated current, and an approximate comparison of the two current values.
- [ ] 2.9 Add prominent low-voltage-only cautions and concise troubleshooting for meter mode, probe ports, negative voltage readings, overload displays, open circuits, and blown meter fuses.

## 3. Manual Voltage-Mapping Lesson

- [ ] 3.1 Document safe 50K Ohm RV Snap voltage-divider wiring from Arduino 5 V to GND with the center wiper as the measurement point, including USB-disconnected wiring and teacher verification.
- [ ] 3.2 Explain in fifth-grade-friendly language how eight ranges act like digital buckets and how decimal levels 0–7 match 3-bit values `000`–`111`.
- [ ] 3.3 Add the complete lookup table using eight 0.625 V ranges from 0–5 V, assigning internal boundaries to the higher range and including 5.000 V in level 7.
- [ ] 3.4 Add numbered measurement and lookup steps that require the minimum, maximum, and at least six additional readings so every output level is represented.
- [ ] 3.5 Add a recording table for measured voltage, decimal level, and 3-bit value plus prompts comparing nearby voltages that share a level.
- [ ] 3.6 Relate the eight-level manual model to the Uno ADC's 1,024 values without requiring the floor formula or Arduino code from students.
- [ ] 3.7 Add concise troubleshooting for approximate endpoints, reversed adjustment direction, unstable readings, incorrect meter setup, and boundary rounding.

## 4. Curriculum Integration and Assets

- [ ] 4.1 Add links from the root and collection documentation to both fundamentals lessons and identify their suggested relationship to the ADC variable-delay lesson.
- [ ] 4.2 Add optional preparation or reinforcement links from the ADC variable-delay lesson without making the fundamentals activities build prerequisites.
- [ ] 4.3 Ensure instructions consistently separate student actions from teacher-only checks and define unfamiliar electrical terms for a fifth-grade audience.
- [ ] 4.4 If circuit illustrations or photos are included, use original repository-owned assets or document permission for every external asset.

## 5. Verification

- [ ] 5.1 Assemble and teacher-test the specified multimeter circuit, confirming part ratings, visible lamp behavior, expected voltage and current ranges, and a safe meter configuration before finalizing numeric examples.
- [ ] 5.2 Review every mode transition in the multimeter procedure for power state, probe jack, dial setting, series or parallel placement, teacher checkpoint, and low-voltage-only language.
- [ ] 5.3 Verify programmatically or by exhaustive review that the lookup table covers every value from 0 through 5 V exactly once and maps all internal boundaries and 5.000 V as specified.
- [ ] 5.4 Review both student activities against every electronics fundamentals delta-spec scenario, including all recording fields and all eight mapping outcomes.
- [ ] 5.5 Run strict OpenSpec validation for `add-electronics-fundamentals-lessons` and resolve all reported issues.
