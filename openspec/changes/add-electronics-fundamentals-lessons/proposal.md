## Why

Students encounter voltage, resistance, current, and digital value ranges in the Arduino lessons before they have a standalone activity for measuring those quantities or practicing how a continuous range becomes a small set of digital levels. Two optional electronics fundamentals lessons will let teachers introduce those ideas before or alongside the Arduino ADC lesson without requiring code or an Arduino project.

## What Changes

- Add an `electronics-fundamentals-lessons/` collection for numbered, standalone, non-code activities.
- Add a multimeter lesson that safely measures voltage, resistance, and current in a low-voltage Snap Circuits battery, switch, lamp, and fixed-resistor circuit.
- Use the lamp to make current flow visible, but use the fixed resistor for Ohm's law calculations because a lamp filament's resistance changes as it heats.
- Add a manual lookup-table lesson that divides the 0–5 V range of a 50K Ohm RV Snap voltage divider into eight output levels.
- Map the eight voltage ranges to decimal levels 0 through 7 and matching 3-bit values `000` through `111`.
- Explain how teachers may use either lesson before or alongside the Arduino ADC variable-delay lesson.

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
- May add cross-references from related curriculum documentation without making either fundamentals lesson a mandatory build dependency.