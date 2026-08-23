## Context

Lesson 01 is a standalone Arduino Uno PlatformIO project whose sketch drives digital pin 8 HIGH and LOW for fixed 2,000 ms intervals. Lesson 02 must preserve that recognizable output behavior while adding one new idea: converting an adjustable voltage on A0 into a number that changes the timing. The target audience is fifth-grade students, and the project must remain independently buildable with the repository's existing Arduino Uno PlatformIO configuration. See `proposal.md` for motivation and `specs/arduino-uno-adc-lessons/spec.md` for observable requirements.

## Goals / Non-Goals

**Goals:**

- Keep the sketch close enough to lesson 01 that students can identify what changed.
- Make the ADC input, mapped delay, serial output, and LED blink rate visibly correlate.
- Use named constants and short comments to make the numeric ranges understandable.
- Keep A0 within the Uno's supported input range through a simple potentiometer voltage divider powered by the Uno.
- Make the README usable as both a wiring guide and a short experiment worksheet.

**Non-Goals:**

- Introduce PWM, interrupts, non-blocking `millis()` timing, ADC register configuration, or alternate voltage references.
- Measure voltages above the Arduino 5 V rail or accept an independently powered analog source.
- Replace or redesign lesson 01's external transistor/LED arrangement.
- Promise that clockwise potentiometer rotation always increases the reading; that direction depends on which outer terminal is connected to 5 V.
- Add copied third-party circuit artwork; any illustration must be original and repository-owned.

## Decisions

### Use A0 with the default Uno analog reference

The potentiometer's outer terminals will connect to Arduino 5 V and GND, and its center wiper will connect to A0. This keeps the measured voltage within the default reference range and makes the endpoint readings approximately 0 and 1023 without additional parts.

Alternative considered: use a photoresistor divider. A potentiometer is preferred because it provides deliberate, repeatable adjustment across most of the ADC range and makes the relationship easier to demonstrate.

### Map the full ADC range directly to 100–2,000 ms

The sketch will use a direct, increasing mapping: reading 0 corresponds to 100 ms and reading 1023 corresponds to 2,000 ms. The range preserves the first lesson's two-second maximum while adding an obviously faster minimum that is still visually distinguishable.

Alternative considered: invert the mapping so a higher reading blinks faster. The direct mapping is easier to explain numerically: a bigger input number produces a bigger delay number.

### Sample before each output interval

Before driving pin 8 HIGH, the sketch will read A0, compute and report the delay, then wait for that delay. It will repeat the same sequence before the LOW interval. This allows a changed control position to affect the next state rather than waiting for an entire old-speed cycle.

Alternative considered: read once at the beginning of each HIGH/LOW cycle and reuse one delay for both states. That creates a perfectly symmetric cycle but makes the control less responsive at the slowest setting. Per-state sampling remains simple and satisfies the lesson's focus.

### Retain blocking delay and pin 8

Lesson 02 will intentionally keep `digitalWrite` and `delay` from lesson 01. Only the source of the delay value changes. This provides a clear before-and-after comparison and avoids introducing non-blocking state management before students understand analog conversion.

Alternative considered: replace `delay` with `millis()`. That would be more scalable for later projects but would add several variables and control-flow concepts unrelated to the ADC objective.

### Report readable labeled values at 9,600 baud

Each sample will produce one concise serial line containing labeled ADC and delay values. The existing PlatformIO projects already use a 9,600 baud monitor setting, and labeled output is easier for beginners to interpret than unlabeled columns.

Alternative considered: comma-separated numeric output for the Serial Plotter. A compact teaching-oriented line is preferred for this lesson; plotting can be introduced separately.

### Keep the lesson independently buildable but explicitly cumulative

Lesson 02 will have its own `platformio.ini`, sketch, and README, while the documentation will identify lesson 01 concepts as prerequisites. Copying the small project configuration avoids cross-project build coupling.

Alternative considered: share source or configuration files with lesson 01. That reduces duplication but makes it harder for a student to open and build one lesson directory by itself.

## Risks / Trade-offs

- [A floating A0 connection produces unstable readings] → Require the potentiometer wiper connection and explain that an unconnected analog pin does not have a reliable value.
- [The two intervals can use slightly different delays after the knob moves] → Describe this as intentional responsiveness and print the value used for each interval.
- [ADC endpoint readings may not be exactly 0 or 1023] → Use “approximately” in student material and focus observations on trends rather than exact endpoint guarantees.
- [The potentiometer's apparent direction can be reversed] → Describe low-reading/faster and high-reading/slower behavior instead of promising a clockwise direction, and explain how swapping the outer terminals reverses direction.
- [A 100 ms minimum produces frequent serial messages] → Emit one line per state transition; approximately ten lines per second at the fastest setting remains manageable at 9,600 baud.
- [Incorrect potentiometer wiring could short 5 V to GND] → Require wiring with USB power disconnected and adult or teacher verification before power is restored.
