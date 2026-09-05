## Context

Lessons 01 and 02 are independent Arduino Uno PlatformIO projects that introduce D8 transistor switching, blocking delays, A0 conversion, direct numeric mapping, and readable serial output. The motor extension must remain recognizable as a continuation while moving the output to a PWM-capable pin and adding an independently powered inductive load. See `proposal.md` for motivation and `specs/arduino-uno-pwm-motor-lessons/spec.md` for observable requirements.

The Arduino is USB-powered. A Snap Circuits B1 holder with two AA batteries powers a 3–6 V yellow gearbox motor that draws approximately 250 mA without load. The motor, RFP30N06LE, 1N5817, and resistors are assembled on a breadboard and share only ground with the Arduino power domain.

## Goals / Non-Goals

**Goals:**

- Keep each lesson independently buildable while making the three projects visibly cumulative.
- Introduce duty cycle before adding live ADC control, then contrast a blocking controller with interrupt-driven updates.
- Use separate Uno timers for PWM, the 100 ms scheduler, and Arduino foreground timing.
- Make the power path, diode polarity, gate biasing, and separate supplies explicit enough for teacher verification.
- Preserve the lesson-02 RV voltage-divider mental model and serial labels.

**Non-Goals:**

- Drive the motor from an Arduino power pin or connect B1 positive to Arduino 5 V or VIN.
- Reverse the motor, brake it actively, close a speed-feedback loop, or claim that duty cycle and shaft speed are linear.
- Characterize stall current, attach a wheel or propeller, or operate the motor under a mechanical load.
- Add an external timer library, MOSFET gate-driver IC, current sensor, encoder, or oscilloscope requirement.
- Teach raw timer-register details as a student learning objective; the register setup is supporting code for the concurrency demonstration.

## Decisions

### Use three numbered standalone projects

Create `03-pwm-motor-ramp`, `04-adc-pwm-motor-control`, and `05-interrupt-driven-motor-control` beside lessons 01 and 02. Each copies the small Uno PlatformIO configuration and contains its own sketch and README. Lesson 04 begins from lesson 03, and lesson 05 begins from lesson 04, so students can compare adjacent sketches without cross-project build coupling.

Alternative considered: place three variants in one project. That would reduce configuration duplication but obscure which program is being built and weaken the repository's established standalone-lesson convention.

### Use D3 PWM with an RFP30N06LE low-side switch

Move the control output from non-PWM D8 to PWM-capable D3. D3 uses the Uno's Timer2 PWM hardware, leaving Timer1 available to schedule lesson 05. The RFP30N06LE source connects to shared ground, its drain connects to motor negative, and its gate connects to D3 through 220–330 ohms. A 10 kilohm gate-to-ground resistor keeps the MOSFET off while the Uno resets or D3 is high impedance.

Alternative considered: retain D8 and generate PWM in software. Hardware PWM is simpler, continues without foreground bit-banging, and creates the timer-separation lesson needed for the final variant.

### Keep the motor supply separate and clamp flyback locally

B1 positive feeds motor positive, and motor negative returns through the MOSFET. B1 negative joins the MOSFET source and Arduino GND, but B1 positive never joins Arduino 5 V or VIN. A 1N5817 is placed directly across the motor, with its banded cathode at motor/B1 positive and anode at motor/MOSFET-drain negative. Its 1 A average rating, fast Schottky behavior, and low-voltage freewheeling use are a better match than the available signal or conventional rectifier diodes.

Alternative considered: use an available 1N400x or 1N540x rectifier. The 1N5817 is preferred for repeated PWM freewheeling; the 1N4148 is excluded because its current rating is unsuitable for this motor path.

### Make lesson 03 an intentionally simple triangular command ramp

Lesson 03 will step `analogWrite` from 0 to 255 and back with a short fixed delay selected to produce an approximately eight-second cycle. The README will call this a smooth command ramp, not guaranteed smooth shaft speed, and will ask students to note the motor's starting threshold and coast-down behavior.

Alternative considered: calculate duty cycle continuously from `millis()`. That gives more exact timing but introduces non-blocking time arithmetic before the curriculum is ready to contrast blocking foreground work with interrupts.

### Make lesson 04's 500 ms blocking behavior explicit

Each lesson-04 loop iteration reads A0, maps 0–1023 directly to 0–255, updates D3, prints labeled ADC and PWM values, then calls `delay(500)`. The delay is retained to make serial output readable and to give students a perceptible control-latency limitation to compare with lesson 05.

Alternative considered: separate fast control and slow reporting using `millis()`. That solves the responsiveness problem before students have observed it and does not set up the intended interrupt demonstration.

### Use Timer1 CTC interrupts for 100 ms lesson-05 updates

Configure Timer1 in clear-timer-on-compare mode for a 100 ms compare interrupt while leaving Timer2 to generate D3 PWM and Timer0 to support Arduino `delay()` and `millis()`. At 16 MHz with a 64:1 prescaler, a compare value of 24,999 produces 25,000 timer ticks per 100 ms interval.

The interrupt handler reads A0, derives an 8-bit PWM value, updates D3, and stores the latest ADC and PWM values in `volatile` shared variables. It performs no serial I/O. A synchronous `analogRead()` makes the handler longer than an ideal production ISR, but at ten calls per second it keeps the demonstration direct and the interrupt occupancy small. The README will name this as a teaching trade-off rather than a general recommendation.

Alternative considered: have the timer ISR set a flag and service ADC in the foreground. A blocking 500 ms delay would then prevent 100 ms control updates, defeating the observable comparison. A fully interrupt-driven ADC state machine would shorten the timer ISR but add register-level concepts outside this lesson's scope.

### Snapshot shared values before serial reporting

The foreground loop will briefly disable interrupts while copying the 16-bit ADC value and 8-bit PWM value into local variables, then restore interrupts before printing. This prevents a torn 16-bit read on the 8-bit ATmega328P and keeps the slow serial operations outside the ISR. It then blocks for 500 ms, during which Timer1 updates continue.

Alternative considered: print the shared variables directly. That leaves the ADC report vulnerable to inconsistent byte reads and misses an important correctness rule for interrupt-shared state.

### Reuse RV and student-document conventions

Lessons 04 and 05 retain the three-terminal RV connection from lesson 02: Arduino 5 V and GND on the outer terminals and A0 on the wiper. Each README will use a shared circuit table, power-off and teacher checkpoints, build/upload/monitor steps, observation prompts, and troubleshooting. Lesson 03 omits RV because its duty cycle is programmatic.

## Risks / Trade-offs

- [The motor draws a startup or stalled current above its 250 mA no-load value] → Keep the demonstration brief and unloaded, use a protected teacher-checked circuit, select the 1N5817 rather than a signal diode, and stop immediately for hot components, odor, battery heating, or failure to turn.
- [The motor may not turn at low PWM values] → Treat the starting threshold as an observation and avoid promising proportional speed across the full 0–255 range.
- [Motor noise can disturb ADC readings or reset the Uno] → Keep motor-current wiring short, place the flywheel diode directly across the motor, separate B1 positive from Arduino power, and keep the common-ground connection secure.
- [Incorrect diode polarity creates a short when the MOSFET turns on] → Show the cathode band in both the wiring table and circuit diagram and require a teacher checkpoint before B1 is enabled.
- [Incorrect TO-220 orientation can swap gate, drain, and source] → Identify pins from the exact RFP30N06LE datasheet and require tracing by function rather than relying only on a drawing's orientation.
- [Calling `analogRead()` inside an ISR models a deliberate teaching compromise] → Document the bounded 10 Hz use, forbid serial output in the ISR, and explain that more advanced systems can trigger ADC conversion asynchronously.
- [Timer register changes can silently break PWM or Arduino timing] → Use Timer1 only for scheduling, D3/Timer2 only for PWM, and leave Timer0 configuration untouched; verify motor updates continue during `delay(500)`.
- [Breadboard or jumper errors can expose moving parts or short AA cells] → Require power-off wiring, secure the motor with no attachment, add teacher verification, and provide immediate shutdown guidance.
