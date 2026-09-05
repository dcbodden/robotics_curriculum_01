## 1. Lesson Collection Structure

- [x] 1.1 Create `electronics-fundamentals-lessons/` with a collection README that explains the numbered, standalone, non-code lesson convention.
- [x] 1.2 Create `01-multimeter-ohms-law/` and `02-manual-voltage-mapping/` with independent student instructions and recording activities.
- [x] 1.3 Document the recommended curriculum sequence: teach the multimeter and Ohm's law lesson first, teach the Arduino ADC variable-delay lesson before manual voltage mapping, and use the mapping lesson afterward for reinforcement and comprehension; neither fundamentals lesson requires building or uploading firmware.
- [x] 1.4 Update repository structure documentation to list the new fundamentals lesson collection.

## 2. Multimeter and Ohm's Law Lesson

- [x] 2.1 Identify the B1 battery, 100 Ohm R1 fixed resistor, and S2 press switch shown in `sample_circuit/base_layout.jpg`, then add a teacher preparation check for actual battery voltage, resistor value and power rating, expected current, and meter range.
- [x] 2.2 Add fifth-grade-friendly assembly and observation steps showing how S2 opens and closes the B1/R1 series path and how teacher-approved meter readings provide evidence of current flow.
- [x] 2.3 Add an unpowered resistance procedure that disconnects the battery, isolates R1 from alternate paths, checks the meter mode and probe jacks, and records the resistance before powered measurements begin.
- [x] 2.4 Add a DC voltage procedure that follows the resistance measurement, uses the black lead in COM and the red lead in the voltage jack, places the probes in parallel directly across B1's positive and negative terminals while the intentional S2-to-B1 gap remains open, and records B1's voltage.
- [x] 2.5 Explain $V = I \times R$ and guide students to use B1's measured voltage and R1's measured resistance with consistent units to calculate and record predicted current in milliamps before any current-mode rewiring.
- [x] 2.6 Add a teacher-verified current procedure that begins only after the prediction is recorded, powers down before rewiring, starts on the highest suitable safe current range that includes the predicted value, inserts the meter in series, prohibits connecting current mode across the battery, records measured current in milliamps, returns the red lead after power is removed, and compares the measurement approximately with the prediction.
- [x] 2.7 Explain that R1 is both the circuit load that limits current and the approximately fixed resistance used in the Ohm's law calculation.
- [x] 2.8 Add a recording table ordered by switch or circuit state, fixed resistance, B1 battery voltage, predicted current in milliamps, measured current in milliamps, and an approximate comparison of the two current values.
- [x] 2.9 Add prominent low-voltage-only cautions and concise troubleshooting for meter mode, probe ports, negative voltage readings, overload displays, open circuits, and blown meter fuses.

## 3. Manual Voltage-Mapping Lesson

- [x] 3.1 Document safe 50K Ohm RV Snap voltage-divider wiring from Arduino 5 V to GND with the center wiper as the measurement point, including USB-disconnected wiring and teacher verification.
- [x] 3.2 Explain in fifth-grade-friendly language how eight ranges act like digital buckets and how decimal levels 0–7 match 3-bit values `000`–`111`.
- [x] 3.3 Add the complete lookup table using eight 0.625 V ranges from 0–5 V, assigning internal boundaries to the higher range and including 5.000 V in level 7.
- [x] 3.4 Add numbered measurement and lookup steps that require the minimum, maximum, and at least six additional readings so every output level is represented.
- [x] 3.5 Add a recording table for measured voltage, decimal level, and 3-bit value plus prompts comparing nearby voltages that share a level.
- [x] 3.6 Relate the eight-level manual model to the Uno ADC's 1,024 values without requiring the floor formula or Arduino code from students.
- [x] 3.7 Add concise troubleshooting for approximate endpoints, reversed adjustment direction, unstable readings, incorrect meter setup, and boundary rounding.

## 4. Curriculum Integration and Assets

- [ ] 4.1 Add links from the root and collection documentation to both fundamentals lessons and identify their suggested relationship to the ADC variable-delay lesson.
- [ ] 4.2 Add optional preparation or reinforcement links from the ADC variable-delay lesson without making the fundamentals activities build prerequisites.
- [ ] 4.3 Ensure instructions consistently separate student actions from teacher-only checks and define unfamiliar electrical terms for a fifth-grade audience.
- [ ] 4.4 Document repository ownership or permission for `sample_circuit/base_layout.jpg` and every other circuit illustration or photo included in the lessons.

## 5. Verification

- [ ] 5.1 Assemble and teacher-test the B1/R1/S2 multimeter circuit shown in `sample_circuit/base_layout.jpg`, confirming battery voltage, resistor value and power rating, expected voltage and current ranges, switch behavior, and a safe meter configuration before finalizing numeric examples.
- [ ] 5.2 Verify the resistance → voltage → predicted-current → teacher-approved series-current sequence and review every mode transition for power state, probe jack, dial setting, series or parallel placement, teacher checkpoint, and low-voltage-only language.
- [ ] 5.3 Verify programmatically or by exhaustive review that the lookup table covers every value from 0 through 5 V exactly once and maps all internal boundaries and 5.000 V as specified.
- [ ] 5.4 Review both student activities against every electronics fundamentals delta-spec scenario, including all recording fields and all eight mapping outcomes.
- [ ] 5.5 Run strict OpenSpec validation for `add-electronics-fundamentals-lessons` and resolve all reported issues.
