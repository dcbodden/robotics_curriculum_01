## Context

See proposal.md for motivation and the delta spec for the behavior contract. Lesson 05 currently owns Timer1 compare A for 100 ms sampling and uses Timer2 hardware PWM on D3. The normal Arduino Servo library also owns Timer1 and its compare-A vector on Uno, so adding that library to the existing configuration would conflict.

This change warrants a design because timer ownership, pulse timing, and actuator power must be resolved before implementation.

## Goals / Non-Goals

**Goals:** Coordinate two commands in a short 20 ms control ISR, keep output pulse timing in hardware, and preserve straightforward student explanations.

**Non-Goals:** Servo library integration, reverse motor drive, joystick-button functions, loaded mechanisms, exact angle calibration, replacement of earlier lessons, or a new printable worksheet.

## Decisions

### Create a standalone lesson 06

Use `platformio-uno-lessons/06-joystick-motor-servo-control/` with its own `platformio.ini`, `src/main.cpp`, and README. Add a separate teacher guide for timer interpretation and power preparation. Editing lesson 05 would invalidate its established 100 ms B1/RV comparison; a new project preserves progression.

### Give Timer1 the servo frame and control interrupt

Use Timer1 Fast PWM with ICR1 as TOP, prescaler 8, and a 20 ms period on the 16 MHz Uno: 0.5 microsecond ticks and TOP 39999. OC1A on D9 generates servo pulses. Use Timer1 overflow interrupt once per frame to read both axes, calculate commands, update OCR1A and D3 PWM, and publish telemetry. Preserve Timer0 for Arduino timing and Timer2 for D3 PWM. Initialize output levels and a middle servo command before enabling the timer.

Timer1 buffers OCR1A; an ISR write can take effect in the following frame rather than in the pulse already underway. Document this bounded frame latency rather than promising physical response within 20 ms. Verify overflow phase and buffer transfer behavior against the ATmega328P datasheet during implementation and measure the result where hardware is available.

The Servo library schedules interrupts at pulse boundaries within a roughly 20 ms refresh frame; it does not provide the single periodic control ISR requested here. Direct hardware generation avoids timer ownership conflicts and pulse jitter from the two ADC reads. Software pulse waits would occupy the ISR unnecessarily. No Servo library dependency is needed.

### Map joystick power with a neutral deadband

Connect VRx to A0 and VRy to A1; power the joystick from Arduino 5 V. Leave SW unused. Use a documented nominal center of 512 and initial deadband of ±40 ADC counts. Values through the upper deadband edge command zero; larger values map monotonically from zero to 255 at 1023. Clamp results. Confirm the actual neutral reading with actuator power off; expose named center/deadband constants for teacher adjustment if the unit's neutral differs. Describe the active direction by increasing VRx readings because physical module orientation varies.

Direct full-axis mapping was rejected because joystick release would command approximately half power. Bidirectional operation requires a different motor driver and is outside this lesson.

Map A1 to an initial conservative servo pulse range of 1000–2000 microseconds with 1500 microseconds at center. Treat this as a command range, not a guaranteed 0–180 degree range. Teacher checks the unloaded unit for binding or sustained buzzing and narrows the named limits if needed. Avoid the Servo library's broader default pulse range without checking the exact kit unit.

### Keep the ISR short and reporting consistent

Read both ADC channels in the ISR with no serial output, delay, or servo pulse wait. Check channel-switch settling using the joystick's source impedance and AVR ADC guidance; use a discarded settling conversion if justified. Budget the complete ISR comfortably below 20 ms. Publish both ADC values, motor command, and servo pulse command together; copy them in a brief atomic block in loop() and restore interrupts before reporting at 9600 baud. The loop keeps a 1000 ms reporting delay.

### Use a separate regulated actuator supply

Use external regulated 5 V for servo and motor branches with shared ground and isolated positive supplies. Keep the protected motor stage, including RFP30N06LE, 220–330 ohm gate series resistor, 10 kilohm pulldown, and 1N5817 cathode toward motor positive. Servo red receives actuator positive, ground receives actuator negative, and signal receives D9; verify connector markings rather than relying solely on color. USB supplies the Uno and joystick.

Teacher preparation must establish both actuators' starting/stall current from the actual parts' documentation or appropriate supervised measurements, choose supply capacity with margin, and verify voltage stability under combined movement. Do not deliberately stall actuators in the student activity. A precise supply-current rating cannot be inferred from “SG90” alone. Document branch wiring and suitable local decoupling; capacitors do not substitute for adequate supply capacity. Do not assume the kit breadboard power module can support both actuators.

Voltage research: [TowerPro analog SG90](https://towerpro.com.tw/product/sg90-analog/) specifies 4.8 V, while the [digital SG90 manufacturer response](https://towerpro.com.tw/product/sg90-7/) permits 4.8–6 V. Choose regulated 5 V and verify the exact Elegoo unit; do not claim the digital model's 6 V allowance establishes the analog kit unit's maximum. [Arduino guidance](https://support.arduino.cc/hc/en-us/articles/360017053760-Troubleshoot-servo-motors) supports separate servo power and shared ground. [Arduino Servo source](https://github.com/arduino-libraries/Servo/blob/master/src/avr/Servo.cpp) documents its timer ownership and pulse scheduling.

## Risks / Trade-offs

- [Neutral varies or jitters] → Verify readings before actuator power; use an adjustable deadband and demonstrate release-to-zero.
- [Buffered servo update is mistaken for instant movement] → Explain command versus output-frame versus mechanical response and verify the waveform.
- [Combined actuator current causes voltage dips or resets] → Verify current capacity, branch routing, decoupling, and combined-operation stability.
- [Servo endpoints bind] → Start unloaded with bounded pulse widths and teacher checks; narrow limits before continued operation.
- [Two ADC channels contaminate readings or ISR work grows] → Verify settling and ISR duration; keep reporting outside the ISR.
- [Motor runs if powered with joystick held forward] → Start command at zero and require neutral verification before teacher enables actuator power; center-off is not a hardware interlock.

## Migration Plan

Add lesson 06 and links, build it independently, review mapping and timer calculations, and conduct document and available hardware verification. Preserve lessons 04/05 and the existing worksheet. Rollback removes the new lesson and discovery links; no earlier wiring or firmware migration is required.

## Open Questions

- The exact kit servo's approved supply range and conservative travel limits, actual joystick neutral, and combined actuator current need hardware-specific verification. These determine preparation values within the selected architecture; document any unperformed checks separately from build and document validation.
