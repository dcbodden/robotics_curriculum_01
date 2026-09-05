## Why

Students need a first, observable Arduino GPIO lesson that connects software output states to real voltage measurements and a physical LED. A standalone lesson also establishes a repeatable layout for the progressive Arduino Uno projects that will follow.

## What Changes

- Add a `platformio-uno-lessons/` area for small, progressive, independently buildable Arduino Uno projects.
- Add the first lesson, `01-gpio-transistor-switch`, with a PlatformIO project that alternates a GPIO output between HIGH and LOW every two seconds.
- Document how to connect Arduino ground and the selected GPIO pin to an existing Snap Circuits NPN-transistor-and-LED assembly.
- Explain how to compare the LED state with GPIO voltage measurements from a digital multimeter.
- Add fifth-grade-friendly instructions for connecting the Arduino Uno by USB and building and uploading the lesson with the PlatformIO extension in VS Code.
- Preserve `platformio-uno-template/` as the reusable starting template rather than turning it into a collection of nested projects.

## Capabilities

### New Capabilities

- `arduino-uno-lessons`: Defines independently buildable Arduino Uno lessons, beginning with a GPIO-controlled Snap Circuits transistor/LED activity and beginner-friendly upload instructions.

### Modified Capabilities

- `repository-structure`: Adds `platformio-uno-lessons/` as the home for standalone Arduino Uno curriculum projects while retaining `platformio-uno-template/` as a reusable template.

## Impact

- Adds a new top-level curriculum directory containing one PlatformIO Arduino Uno project and its student documentation.
- Updates the documented repository layout and the Uno template documentation.
- Uses the existing PlatformIO Arduino framework and introduces no runtime dependencies beyond the configured `atmelavr` platform.
- Assumes the NPN transistor, LED, and their supporting components already exist in the Snap Circuits assembly; the Arduino supplies a shared ground and GPIO control signal only.
