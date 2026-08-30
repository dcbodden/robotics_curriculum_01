## Why

Students need hands-on experience with voltage, resistance, and current before beginning the Arduino sequence, then benefit from revisiting analog-to-digital conversion after seeing it in code. A multimeter and Ohm's law lesson taught first will establish the electrical foundation, while a manual voltage-mapping lesson taught after the Arduino ADC variable-delay lesson will reinforce how a continuous voltage becomes a digital value. Neither fundamentals lesson requires students to build or upload firmware.

## What Changes

- Add an `electronics-fundamentals-lessons/` collection for numbered, standalone, non-code activities.
- Add a multimeter lesson that safely measures voltage, resistance, and current in a low-voltage Snap Circuits circuit using the B1 battery, 100 Ohm R1 fixed resistor, and S2 press switch shown in `sample_circuit/base_layout.jpg`.
- Use R1 as both the circuit load and the fixed resistance for Ohm's law calculations; the circuit does not include a lamp.
- Add a manual lookup-table lesson that divides the 0–5 V range of a 50K Ohm RV Snap voltage divider into eight output levels.
- Map the eight voltage ranges to decimal levels 0 through 7 and matching 3-bit values `000` through `111`.
- Establish the recommended curriculum sequence: multimeter and Ohm's law first, Arduino ADC variable-delay before manual voltage mapping, and manual voltage mapping afterward for reinforcement and comprehension.

## Capabilities

### New Capabilities

- `electronics-fundamentals-lessons`: Defines standalone, fifth-grade-friendly measurement and manual value-mapping activities, including electrical safety and observable learning outcomes.

### Modified Capabilities

- `repository-structure`: Adds `electronics-fundamentals-lessons/` as the home for numbered, non-code electronics activities.

## Impact

- Adds a new top-level curriculum directory with two lesson directories and collection documentation.
- Adds student worksheets or tables for recording measurements, checking Ohm's law, and mapping measured voltages to eight digital levels.
- Uses low-voltage Snap Circuits parts and a digital multimeter; it adds no software dependencies or firmware.
- Requires explicit meter-mode, probe-port, powered-circuit, and series-versus-parallel safety instructions, with teacher verification before current measurement.
- Adds cross-references that communicate the recommended lesson sequence without making either fundamentals lesson a firmware build dependency.
