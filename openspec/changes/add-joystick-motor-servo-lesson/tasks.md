## 1. Standalone project and control firmware

- [x] 1.1 Create lesson 06's standalone Uno PlatformIO configuration and source, with joystick A0/A1 inputs, D3 motor output, D9 servo output, zero initial motor command, and middle initial servo command.
- [x] 1.2 Implement bounded center-off motor mapping with named neutral/deadband constants and bounded servo pulse mapping. Verify center, deadband boundaries, inactive travel, active endpoint, and servo endpoints with meaningful mapping checks.
- [x] 1.3 Verify Timer1 frame, overflow phase, and buffered compare behavior against the ATmega328P documentation; configure hardware servo pulses on a 20 ms frame and update both ADC-derived commands in the once-per-frame ISR, preserving Timer0 and Timer2. Check ADC channel settling and ISR duration; exclude waits and serial output from the ISR.
- [x] 1.4 Publish and atomically snapshot both ADC readings and commands for roughly one-second 9600-baud reporting, retaining the foreground delay and restoring interrupts before serial output.

## 2. Student and teacher materials

- [x] 2.1 Write lesson 06's fifth-grade README with prerequisites, joystick/servo wiring and polarity, protected motor stage, shared ground, separate positive supplies, USB power, and regulated external 5 V actuator power. Include combined-current selection and teacher verification rather than assuming kit power-module capacity.
- [x] 2.2 Add build/upload and power checkpoints, neutral/deadband verification with actuator power off, conservative servo limits, secured motor/servo precautions, and troubleshooting for binding, buzzing, motor starting thresholds, heat, and unstable power.
- [x] 2.3 Add an observation activity for each axis separately and both together, with records and reflection on 50 checks per second, servo frame timing, motor PWM, serial snapshots, coasting, and physical response.
- [x] 2.4 Create the teacher guide with supply-current and voltage checks for the actual hardware, local decoupling and branch-routing guidance, neutral and travel adjustments, source links for SG90 voltage and timer behavior, and the distinction between this hardware timer approach and Arduino Servo library interrupts.
- [x] 2.5 Link lesson 06 from lesson 05 and the lesson index; verify links and preserve earlier firmware, wiring, and the robot-attention worksheet.

## 3. Verification and completion

- [x] 3.1 Build lesson 06 for Uno and review timer arithmetic, resource ownership, startup commands, mapping checks, ISR timing budget, and consistent telemetry against the requirements.
- [x] 3.2 Where hardware is available, verify servo frame/pulse bounds and command-to-frame timing, continuing D3 PWM, independent ADC channels, neutral release, bounded servo motion, and response between serial messages. Record any unperformed waveform or physical checks explicitly.
- [x] 3.3 Where the actual actuators and supply are available, verify documented supply capacity and combined-operation voltage stability without deliberate student stalls; record actual findings and distinguish unperformed power checks from firmware/document verification.
- [x] 3.4 Review student and teacher explanations and all local links, run strict OpenSpec validation and whitespace checks, and summarize completed checks and remaining physical verification without inventing observations.
