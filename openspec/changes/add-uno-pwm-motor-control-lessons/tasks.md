## 1. Standalone Lesson Structure

- [ ] 1.1 Create `03-pwm-motor-ramp`, `04-adc-pwm-motor-control`, and `05-interrupt-driven-motor-control` as independent Arduino Uno PlatformIO projects using the established `atmelavr`, `uno`, and `arduino` configuration.
- [ ] 1.2 Add the existing PlatformIO IDE extension recommendation and serial monitor configuration needed by each lesson.
- [ ] 1.3 Update `platformio-uno-lessons/README.md` with the three new lessons, their prerequisites, and the progression from PWM through blocking ADC control to interrupt-driven control.

## 2. Protected Motor Circuit Instructions

- [ ] 2.1 Add a bill of materials for the yellow 3–6 V gearbox motor, RFP30N06LE, 1N5817, two-AA B1 supply, breadboard, RV snap for lessons 04–05, 220–330 ohm gate resistor, and 10 kilohm gate pulldown resistor.
- [ ] 2.2 Document the RFP30N06LE gate, drain, source, and tab identification from its datasheet and provide a functional wiring table that does not rely only on physical orientation.
- [ ] 2.3 Document the low-side power path, D3 gate connection, gate pulldown, shared-ground connection, and strict isolation of B1 positive from Arduino 5 V and VIN.
- [ ] 2.4 Document the 1N5817 directly across the motor with its banded cathode toward B1/motor positive and its anode toward the MOSFET drain/motor negative.
- [ ] 2.5 Add power-off wiring, teacher verification, secured bare-motor operation, immediate-shutdown symptoms, and no-wheel-or-propeller safety instructions to all three lesson procedures.

## 3. Lesson 03 Programmatic PWM Ramp

- [ ] 3.1 Implement the lesson-03 sketch on D3 with beginner-readable constants and a repeated approximately four-second 0-to-255 ramp followed by an approximately four-second 255-to-0 ramp.
- [ ] 3.2 Explain PWM duty cycle and why a smoothly changing command can produce a starting threshold, coasting, or non-linear motor speed.
- [ ] 3.3 Add build, upload, observation, recording, and troubleshooting steps for the eight-second ramp demonstration.

## 4. Lesson 04 Blocking ADC Motor Control

- [ ] 4.1 Implement the lesson-04 sketch to read A0, map 0–1023 directly to D3 PWM 0–255, print labeled ADC and PWM values, and delay 500 milliseconds between updates.
- [ ] 4.2 Document the RV outer-terminal connections to Arduino 5 V and GND and the center-wiper connection to A0 without connecting B1 positive to the ADC circuit.
- [ ] 4.3 Explain that the 500-millisecond delay makes serial output readable while also making motor-control response intentionally sluggish.
- [ ] 4.4 Add build, upload, serial-monitor, multi-position observation, recording, and troubleshooting steps that relate RV position, ADC value, PWM value, and motor behavior.

## 5. Lesson 05 Interrupt-Driven Motor Control

- [ ] 5.1 Configure Timer1 in compare mode for a 100-millisecond interrupt without changing Timer0 or D3's Timer2 PWM configuration.
- [ ] 5.2 Implement the interrupt handler to read A0, derive and apply the 0–255 D3 PWM command, update `volatile` shared values, and perform no serial output.
- [ ] 5.3 Implement a consistent foreground snapshot of the shared ADC and PWM values, report it over serial, and block for 500 milliseconds between reports.
- [ ] 5.4 Explain foreground work, interrupt work, `volatile` data, atomic snapshots, and the deliberate trade-off of using synchronous `analogRead()` in this bounded 10 Hz demonstration.
- [ ] 5.5 Add an observation activity that compares lesson 04 and lesson 05 responsiveness while retaining the same RV, motor circuit, and 500-millisecond serial-report interval.
- [ ] 5.6 Add timer-specific troubleshooting for a stationary motor, unchanging ADC values, missing serial output, and evidence that Timer1 updates continue during foreground delays.

## 6. Verification

- [ ] 6.1 Build each of lessons 03, 04, and 05 independently with PlatformIO and confirm that no other lesson source is compiled.
- [ ] 6.2 Review all three procedures against the protected power-stage specification, including MOSFET function, diode polarity, separate positive supplies, shared ground, and secured-motor cautions.
- [ ] 6.3 Review lesson timing and mappings against the delta spec: approximately eight seconds for lesson 03, 500-millisecond control updates for lesson 04, and 100-millisecond interrupt updates with 500-millisecond reports for lesson 05.
- [ ] 6.4 Verify lesson 05 leaves Timer0 and Timer2 roles intact, keeps serial operations outside the ISR, and copies the 16-bit ADC value atomically.
- [ ] 6.5 Run strict OpenSpec validation for `add-uno-pwm-motor-control-lessons` and resolve any reported issues.
