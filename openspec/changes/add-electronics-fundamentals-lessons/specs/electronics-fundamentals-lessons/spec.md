## Purpose

Defines standalone, fifth-grade-friendly electronics activities that teach safe multimeter use and manual conversion of a continuous voltage range into discrete digital levels.

## ADDED Requirements

### Requirement: Standalone electronics fundamentals collection
The curriculum SHALL provide `electronics-fundamentals-lessons/` as a collection of numbered, non-code lessons, beginning with one multimeter and Ohm's law lesson and one manual voltage-mapping lesson.

#### Scenario: Choose when to teach a fundamentals lesson
- **WHEN** a teacher plans instruction around the Arduino ADC variable-delay lesson
- **THEN** either fundamentals lesson can be taught before or alongside it without building or uploading firmware

### Requirement: Low-voltage multimeter activity
The multimeter lesson SHALL use a low-voltage Snap Circuits battery, switch, lamp, and fixed resistor to demonstrate an open and closed circuit and to measure voltage, resistance, and current.

#### Scenario: Observe current flow
- **WHEN** a student closes the switch in the correctly assembled circuit
- **THEN** the lamp provides a visible sign of current flow and the student can describe the circuit as a complete path

#### Scenario: Measure voltage
- **WHEN** a student measures voltage in the powered circuit
- **THEN** the meter is set to DC voltage with the black lead in COM and the red lead in the voltage jack, and the probes are placed in parallel across the two measurement points

#### Scenario: Measure resistance
- **WHEN** a student measures the fixed resistor
- **THEN** all power is disconnected, the resistor is isolated from other circuit paths, and the meter is set to resistance before the probes touch the resistor terminals

#### Scenario: Measure current
- **WHEN** a student measures current through the circuit
- **THEN** power is removed before the circuit is opened, the meter leads and range are configured for current, a teacher verifies the series connection, and power is restored only for the reading

### Requirement: Multimeter safety boundaries
The multimeter lesson SHALL explain that resistance is never measured on a powered circuit, current mode is never connected directly across a battery or component, circuit changes are made with power removed, and the activity is limited to the specified low-voltage circuit rather than household or wall power.

#### Scenario: Prepare for a different meter mode
- **WHEN** a student changes between voltage, resistance, and current measurements
- **THEN** the instructions require power removal where appropriate and a check of the dial setting, probe jacks, and probe placement before measurement

#### Scenario: Finish a current measurement
- **WHEN** the current reading is complete
- **THEN** power is removed and the red probe is returned from the current jack to the voltage/resistance jack

### Requirement: Fixed-resistor Ohm's law comparison
The multimeter lesson SHALL use the fixed resistor, rather than the heated lamp filament, for a beginner calculation comparing measured voltage, measured resistance, measured current, and the relationship $V = I \times R$.

#### Scenario: Compare calculation and measurement
- **WHEN** a student records the fixed resistor's resistance and voltage
- **THEN** the worksheet guides the student to calculate expected current with $I = V / R$ and compare it with a safely measured current using consistent units

#### Scenario: Discuss the lamp separately
- **WHEN** the lesson describes the lamp
- **THEN** it explains that the lamp makes current flow visible but its filament resistance changes as it heats, so it is not treated as the fixed value in the Ohm's law calculation

### Requirement: Eight-level manual voltage mapping
The mapping lesson SHALL treat a 50K Ohm RV Snap as a voltage divider from Arduino 5 V to GND and manually map its 0–5 V wiper range into eight equal 0.625 V ranges labeled decimal 0–7 and binary `000`–`111`.

#### Scenario: Look up a voltage below 5 V
- **WHEN** a student is given or measures a voltage from 0 V up to but not including 5 V
- **THEN** the student selects level $\lfloor V / 0.625 \rfloor$ and the matching 3-bit value from the lookup table

#### Scenario: Look up the maximum voltage
- **WHEN** a student is given or measures 5 V
- **THEN** the student selects decimal level 7 and binary value `111`

#### Scenario: Resolve a boundary value
- **WHEN** a voltage equals an internal boundary such as 0.625 V, 1.250 V, or 4.375 V
- **THEN** the lookup table places it in the range that starts at that boundary so the voltage maps to exactly one level

### Requirement: Safe RV Snap setup
The mapping lesson SHALL explain how to connect the RV Snap outer terminals to Arduino 5 V and GND and its center wiper to the voltage measurement point, with USB power disconnected during wiring and teacher verification before power is restored.

#### Scenario: Measure the RV Snap output
- **WHEN** the verified divider is powered and a student moves the RV Snap control
- **THEN** a DC voltage measurement from the wiper to GND varies within approximately 0–5 V without applying an external voltage to the Arduino connection

### Requirement: Beginner recording activities
Each lesson SHALL use fifth-grade-friendly numbered instructions and a recording table or prompts that let students compare predictions, measurements, calculations, and observations.

#### Scenario: Complete the multimeter record
- **WHEN** a student completes the measurement activity
- **THEN** the student records circuit state, voltage, fixed resistance, measured current, calculated current, and whether the lamp is lit

#### Scenario: Complete the mapping record
- **WHEN** a student tests the minimum, maximum, and at least six additional voltages covering all eight ranges
- **THEN** the student records each voltage, decimal level, and 3-bit value and can explain that one smoothly changing input was sorted into eight digital choices