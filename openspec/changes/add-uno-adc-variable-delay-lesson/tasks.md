## 1. Lesson Project Structure

- [x] 1.1 Create `platformio-uno-lessons/02-adc-variable-delay/` as an independent Arduino Uno PlatformIO project using the repository's `atmelavr`, `uno`, and `arduino` configuration.
- [x] 1.2 Configure the lesson's serial monitor for 9,600 baud and include the same editor recommendations used by the existing Uno lesson projects.
- [x] 1.3 Update the lessons collection documentation to list lesson 02 after `01-gpio-transistor-switch` and state its prerequisite concepts.

## 2. ADC-Controlled Program

- [x] 2.1 Add a beginner-readable sketch that configures digital pin 8 as the transistor control output, initializes serial communication at 9,600 baud, and names A0 as the analog input.
- [x] 2.2 Read A0 before each HIGH and LOW interval and map the 0–1023 ADC range to an increasing 100–2,000 ms delay range.
- [x] 2.3 Print a labeled ADC reading and mapped delay for every sampled interval, then apply that delay to the corresponding pin-8 state.
- [x] 2.4 Keep comments and control flow visibly related to lesson 01 while explaining only the newly introduced ADC and mapping concepts.

## 3. Wiring and Safety Instructions

- [x] 3.1 Document the three-terminal potentiometer voltage divider: outer terminals to Arduino 5 V and GND, center wiper to A0, with USB power disconnected during wiring.
- [x] 3.2 Document reuse of lesson 01's shared-ground and digital-pin-8 transistor/LED output connections without redesigning the external assembly.
- [x] 3.3 Add cautions about teacher verification, keeping A0 within 0–5 V, avoiding external analog power, preventing 5 V-to-GND shorts, and the unstable behavior of a floating A0 input.
- [x] 3.4 If a wiring illustration is included, create original repository-owned artwork rather than copying third-party component or circuit artwork.

## 4. Student ADC Activity

- [x] 4.1 Explain in fifth-grade-friendly language how a varying voltage becomes one of 1,024 digital values from 0 through 1023.
- [x] 4.2 Explain that low readings produce shorter delays and faster blinking while high readings produce longer delays and slower blinking, without assuming a fixed knob direction.
- [x] 4.3 Add numbered build, upload, serial-monitor, and observation steps using the PlatformIO extension in VS Code.
- [x] 4.4 Add an observation table or prompts for at least three potentiometer positions comparing ADC reading, delay in milliseconds, and visible blink speed.
- [x] 4.5 Add concise troubleshooting for a floating or miswired A0 input, reversed adjustment direction, missing serial output, and unchanged blink timing.

## 5. Verification

- [x] 5.1 Build `02-adc-variable-delay` with PlatformIO and confirm it targets an Arduino Uno without compiling another lesson.
- [x] 5.2 Review or test the endpoint mapping so ADC readings 0 and 1023 produce delays of 100 ms and 2,000 ms respectively.
- [x] 5.3 Verify the sketch samples before each output interval and emits matching labeled ADC and delay values at 9,600 baud.
- [x] 5.4 Review the student instructions against every ADC lesson scenario, including independent build behavior, safe wiring, three-position observation, and lesson 01 continuity.
- [x] 5.5 Run strict OpenSpec validation for `add-uno-adc-variable-delay-lesson` and resolve all reported issues.
