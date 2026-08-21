## Why

Students need a second Arduino Uno lesson that connects a continuously varying voltage to a digital value they can observe and use. Extending the first lesson's blinking output with an analog-controlled delay provides a concrete introduction to the Uno's analog-to-digital converter without introducing unrelated control-flow concepts.

## What Changes

- Add `platformio-uno-lessons/02-adc-variable-delay/` beside the existing GPIO lesson as an independently buildable Arduino Uno PlatformIO project.
- Read an adjustable voltage on analog input A0 and use the Uno's 10-bit ADC result to control the HIGH and LOW delay applied to digital pin 8.
- Map the expected ADC range of 0–1023 to a beginner-observable delay range of 100–2,000 milliseconds, with lower readings producing faster blinking and higher readings producing slower blinking.
- Print the raw ADC reading and calculated delay to the serial monitor so students can compare the input, timing value, and visible LED behavior.
- Document a safe three-terminal potentiometer voltage-divider connection between 5 V, A0, and GND while retaining the first lesson's pin-8 transistor/LED output connection.
- Explain ADC resolution, the relationship between input voltage and readings, and how to build, upload, observe, and experiment with the lesson in fifth-grade-friendly language.

## Capabilities

### New Capabilities

- `arduino-uno-adc-lessons`: Defines the standalone ADC-controlled timing lesson, its observable program behavior, safe analog-input wiring, and beginner learning activity.

### Modified Capabilities

None.

## Impact

- Adds one numbered PlatformIO project under `platformio-uno-lessons/` with its own configuration, sketch, and student documentation.
- Reuses Arduino digital pin 8 and the external transistor/LED output arrangement from lesson 01, and adds analog pin A0 plus Arduino 5 V for a potentiometer input.
- Uses only the Arduino framework's built-in `analogRead`, `map`, GPIO, timing, and serial APIs; no new libraries or runtime dependencies are required.
- Establishes lesson 02 as dependent on concepts from lesson 01 while remaining independently buildable and uploadable.
