## Context

The repository currently has one reusable Arduino Uno PlatformIO template whose sketch blinks `LED_BUILTIN`. The new curriculum needs a separate home for independently opened lesson projects. The first lesson controls an NPN transistor and LED that are already assembled as a Snap Circuits project, so the Arduino-side interface consists only of a common ground and a GPIO control signal.

The lesson is intended for a fifth-grade audience and pairs visible LED behavior with digital multimeter measurements. See `proposal.md` for motivation and the delta specs for observable requirements.

## Goals / Non-Goals

**Goals:**

- Keep the reusable Uno template separate from student lesson projects.
- Make the first lesson independently buildable and uploadable in PlatformIO.
- Make the GPIO state changes slow enough for a beginner to measure reliably.
- Explain the two cross-system connections and multimeter procedure in simple language.
- Include cautious, explicit steps for USB connection, build, upload, and basic troubleshooting.

**Non-Goals:**

- Rebuild or document the internal Snap Circuits transistor-and-LED assembly.
- Power a motor, relay, solenoid, or other inductive load.
- Introduce pulse-width modulation, non-blocking timing, serial monitoring, or external power supplies.
- Convert the reusable template into a multi-project workspace.

## Decisions

### Use a separate `platformio-uno-lessons` collection

Each lesson will be a nested standalone PlatformIO project under a numbered directory, beginning with `01-gpio-transistor-switch`. This preserves `platformio-uno-template` as a clean source template and prevents multiple Arduino `setup()` and `loop()` functions from sharing a build.

Alternative considered: place several sketches inside the template. This would blur the template's role and make independent PlatformIO builds less obvious to beginners.

### Use digital pin 8 and blocking two-second delays

The sketch will give the external control point a descriptive constant mapped to digital pin 8, configure it as `OUTPUT`, and alternate `HIGH` and `LOW` with `delay(2000)` after each transition. Pin 8 avoids coupling the exercise to the built-in LED on pin 13. Blocking delays are appropriate because this introductory lesson performs no concurrent work.

Alternative considered: use `LED_BUILTIN` or a `millis()` state machine. The former obscures the external GPIO connection, while the latter introduces timing concepts that distract from the voltage/state relationship.

### Treat Snap Circuits as a preassembled external system

The wiring section will identify only two required jumpers: Arduino GND to the Snap Circuits ground node, and Arduino D8 to the transistor base control node already provided by the assembly. It will tell the student to disconnect power while wiring and to have an adult or teacher verify the two points before reconnecting USB.

Alternative considered: include a discrete-transistor wiring diagram and component values. That would misrepresent the real activity and could conflict with the existing Snap Circuits design.

### Center the observation on voltage and LED state

The guide will have the student measure D8 relative to the shared ground. It will describe HIGH as near 5 V and LOW as near 0 V, while asking the student to record whether the LED is on or off in each interval. The language will avoid promising exact measurements because loading, meter behavior, and board tolerances vary.

### Keep the upload guide visual-language friendly but text-native

The README will use short numbered steps, button names plus recognizable PlatformIO icons, expected messages, and a compact troubleshooting section. Text remains useful in source control and can later be paired with screenshots without changing the workflow.

## Risks / Trade-offs

- [The Snap Circuits base node may not be labeled consistently across projects] → Describe its function and require teacher verification against the specific assembly.
- [The LED's on/off polarity may differ from the expected HIGH/on relationship] → Ask students to record the observed correlation rather than hard-code an unsupported assertion about the assembly.
- [A charge-only USB cable can power the board but cannot upload] → Call out use of a USB data cable in the main procedure and troubleshooting section.
- [Linux serial-port permissions may block upload] → Include a teacher-facing troubleshooting note without burdening the main student path with system administration.
- [Two-second blocking delays do not scale to interactive projects] → State that the approach is intentional for the first lesson; later lessons can introduce non-blocking timing.
