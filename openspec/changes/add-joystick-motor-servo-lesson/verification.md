## Task 1.3: timer and ADC review

- Reviewed [ATmega328P datasheet](https://ww1.microchip.com/downloads/en/devicedoc/atmel-7810-automotive-microcontrollers-atmega328p_datasheet.pdf), sections 15.9.3, 15.11, 23.4, and 23.6.1: fixed ICR1 TOP permits OC1A servo output; Fast PWM sets TOV1 at TOP and transfers the buffered compare at the frame boundary. Interrupt entry and ADC work occur after that transfer, so the new command controls the following frame. The initial OCR1A value is loaded in stopped normal mode before enabling PWM.
- Mode 14, prescaler 8, and TOP 39999 give 16 MHz / (8 × 40000) = 50 Hz. Compare commands of 2000–4000 ticks represent nominal 1000–2000 microsecond pulses. Timer0 remains unchanged; D3 uses Arduino Timer2 PWM, with steady low/high for zero/full power.
- ADC source-impedance guidance is approximately 10 kilohms or less. Discarding a conversion after every axis switch gives additional settling time; hardware verification of the actual joystick and wiring remains necessary. Arduino's local AVR core sets the ADC prescaler to 128 (125 kHz); normal conversions take 13 ADC clocks, the first takes 25. Four conversions cost approximately 416 microseconds normally, with startup/synchronization allowance below 550 microseconds.
- Reviewed the built AVR disassembly: four analogRead calls, at most two bounded 32-bit division routines, no serial calls, and no delay or servo pulse wait. A conservative 2 ms ISR budget allows substantial margin for ADC polling, arithmetic, entry/exit, and output writes within the 20 ms period. This is a software timing estimate, not a measured duration. ADC polling is the intended conversion wait; hardware maintains both output signals while it runs.
- Uno build passed. Actual ISR duration, servo waveform/startup, ADC channel independence, and motor/servo motion have not been measured; physical and waveform checks remain under task 3.2. Foreground snapshot reporting is verified below under task 1.4.

## Task 1.4: consistent serial snapshots

- ISR publishes both ADC readings and both commands. Foreground copies the complete set within one ATOMIC_RESTORESTATE block, then prints labeled decimal readings, the motor PWM command, and the servo pulse command in microseconds at 9600 baud. The 1000 ms foreground delay follows reporting; control interrupts remain enabled during printing and the delay.
- Reviewed generated AVR disassembly: seven byte loads for the four values occur between cli and restoration of SREG. No print calls occur in that block; serial calls follow status restoration. The copy itself occupies approximately one microsecond at 16 MHz, excluding interrupt entry if one was already pending. ISR publication cannot interleave with this snapshot.
- Uno build passed (3150 bytes flash, 275 bytes static RAM). Serial timing and live data on an attached board have not been physically verified.

## Task 3.1: final Uno build and firmware review

Completed on 2026-09-13 against the current lesson 06 firmware. No firmware changes were needed.

### Executed checks

- `/home/dbodden/.platformio/penv/bin/pio run -d platformio-uno-lessons/06-joystick-motor-servo-control` passed for Uno/ATmega328P, 16 MHz, Arduino AVR framework 5.2.0 and AVR GCC 7.3.0. The configured project has no library dependencies. Image usage: 3150/32256 bytes flash and 275/2048 bytes static RAM; static RAM usage does not include runtime stack.
- From the lesson directory, `g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_control_mapping.cpp -o /tmp/lesson06-mapping-check`, followed by `/tmp/lesson06-mapping-check`, passed. Checks cover neutral, both deadband edges, inactive travel, integer quantization at 553/554, active midpoint/endpoint, servo center/endpoints, out-of-range clamping, and bounded monotonic behavior across all 1024 ADC inputs.
- Inspected the built ELF using the installed `avr-objdump -d -C`, including Timer1 overflow vector `__vector_13`, arithmetic helpers, timer initialization, and foreground snapshot/printing. Also inspected the installed Arduino core's `wiring.c`, `wiring_analog.c`, and standard Uno pin-to-timer table.

### Review against requirements

| Area | Result |
| --- | --- |
| Timer arithmetic | Timer1 mode 14, divide by 8, TOP 39999: 40000 ticks at 0.5 microseconds = 20 ms, or 50 frames/checks per second. OCR1A 2000–4000 ticks gives nominal 1000–2000 microsecond pulses. Constants fit AVR 16-bit unsigned storage; the 16 MHz and maximum-pulse/frame assertions pass. |
| Resource ownership | D9 is OC1A/Timer1 hardware output, with only the overflow control interrupt enabled. D3 is OC2B/Timer2; the installed core initializes phase-correct PWM with divide by 64, nominally 16 MHz / (64 × 510), about 490 Hz. Zero/full motor commands use steady low/high. Timer0 configuration is untouched. No Servo library or competing Timer1 output calls are present. |
| Startup | D3 is made low/output with zero command before enabling control. D9 is prepared low/output; stopped normal-mode Timer1 receives TOP and the initial 1500-microsecond compare before PWM starts. Counter starts at TOP, pending flags are cleared, and saved SREG is restored. Initial neutral telemetry is a placeholder until sampling; a held active joystick can command power on the first check, so the documented power-off neutral preparation remains necessary. |
| Mapping | A0 uses the confirmed center-off behavior: all readings through 552 command zero and active travel reaches 255. A1 is bounded 1000–2000 microseconds, with exactly 1500 at 512. Widened multiplication avoids AVR 16-bit signed overflow. Channels and commands are independent, with the joystick switch unused. |
| ISR work and budget | Disassembly confirms four ADC calls, at most two bounded signed 32-bit divisions (the lower servo half becomes a nine-step shift), motor output, buffered OCR1A output, and seven telemetry-byte stores. No print/delay/pulse-wait calls are reachable from this control ISR. ADC completion is polled without requiring interrupts. |
| Frame latency | Hardware maintains the pulse edges while the ISR runs. The new buffered OCR1A compare applies to the following frame; this is a command update, not a promise of completed mechanical movement within 20 ms. |
| Consistent telemetry | Both readings and commands are published in the same non-nested ISR. Generated foreground code loads all seven bytes between `cli` and SREG restoration, then prints the four labeled fields at configured 9600 baud. The copy plus disable/restore is approximately 1 microsecond. Printing and the 1000 ms foreground delay occur with interrupts restored. Reports are roughly one second apart, not an exact one-second sampling stream. |

The installed core sets the ADC clock to 125 kHz. Four normal 13-clock conversions total about 416 microseconds; first-conversion and clock-synchronization allowances remain below the previously reserved 550 microseconds of ADC time. The division helper has a fixed 33-count loop; two divisions and the other bounded arithmetic/output/entry/exit work fit comfortably in the remaining 1450 microseconds of the conservative 2 ms software budget. This budget is 10% of a 20 ms frame, not a measured ISR duration or a formal cycle-accurate worst-case proof. No added unbounded software loops were found beyond polling the normally completing ADC hardware.

Task 3.1 establishes build, mapping, and source/generated-code review results. The user's earlier report that motor and servo work after restoring the missing ground is qualitative motion evidence. It does not establish waveform bounds, measured ISR duration, command-to-frame latency, or supply capacity/stability. Those checks remain under tasks 3.2 and 3.3; no upload or live measurement was performed for this task.

## Task 3.2: hardware observations and unperformed measurements

On 2026-09-13, PlatformIO device enumeration outside the sandbox identified an Arduino Uno USB device at `/dev/ttyACM0` (VID:PID 2341:0043). No oscilloscope or logic-analyzer captures are available in the task evidence. Actuator power state and the physical setup were not independently established.

Read the already-configured 9600-baud serial port for six seconds using a read-only, nonblocking file descriptor. No firmware upload, baud/configuration change, explicit reset, or joystick manipulation was performed. Representative complete lines were:

```text
Motor ADC: 506 | Motor PWM command: 0 | Servo ADC: 500 | Servo pulse command (us): 1488
Motor ADC: 512 | Motor PWM command: 0 | Servo ADC: 512 | Servo pulse command (us): 1500
```

The repeated 506/500 sample agrees with the current mappings: A0 is below the active threshold and commands zero; A1 commands 1488 microseconds. These are reported commands, not measured D3/D9 signals. The capture also contained partial lines and one nominal-initial-value line. An existing serial reader, buffering, or board restart cannot be ruled out; do not use this capture to establish report cadence, uninterrupted operation, or firmware identity. No deliberate reset was requested by the capture code.

| Requested check | Evidence/status |
| --- | --- |
| Live ADC and command telemetry | Captured neutral-like values and correctly corresponding command fields; no controlled axis sweep performed |
| Motor and servo motion | User previously reported both working after restoring the missing ground; qualitative report only |
| Servo frame and pulse bounds | **Not measured:** capture D9 relative to shared ground across teacher-approved travel; record frame period and minimum/center/maximum high pulse durations against configured 20 ms and 1000/1500/2000 microsecond targets |
| Command-to-frame timing | **Not measured:** a waveform capture correlated with the control ISR/compare update is needed to distinguish next-frame transfer from mechanical travel; serial snapshots cannot establish this |
| Continuing D3 PWM | **Not measured:** capture D3 at an intermediate motor command while changing A1 and while foreground reports/waits; record continuing pulses at its separate switching rhythm. Zero/full commands should be steady low/high |
| Independent ADC channels | User confirms the requested physical checks: motor control works in one direction off center and servo control in both directions. Controlled ADC sweep logs were not supplied |
| Neutral release and inactive travel | User confirms the motor operates only in the active direction and everything is stopped/neutral at dead center; captured A0 506 also reports zero motor command. No stop-time measurement supplied |
| Bounded unloaded servo motion | User confirms all requested physical checks and reports servo operation in both directions with a neutral center. No measured travel angles or revised pulse limits supplied |
| Response between serial messages | Covered by the user’s confirmation that all requested physical checks were performed; no trial logs or quantitative response times supplied |
| Actual ISR duration | **Not measured:** the task 3.1 software budget remains an estimate |

Use the student lesson's teacher-approved power checkpoints and observation sequence for motion checks. Remove both supplies before wiring or instrument-connection changes; do not sweep an unverified servo into its endpoints. Power capacity/stability evidence belongs to task 3.3.

User follow-up: “I've checked all of these, the motor control works in one direction off center, the servo works in both, and dead center everything is stopped/neutral.” This records user-confirmed physical checks, including the requested independent-control and response observations, without assigning measured values. A neutral servo remains a position command and may hold its position; it is not necessarily unpowered.

Task 3.2 is complete with user-confirmed physical observations and explicit records of unperformed waveform measurements, as the task permits. No instrumentation availability or waveform results were supplied, so servo frame/pulse bounds, command-to-frame timing, D3 waveform continuity, and actual ISR duration remain unmeasured. Supply checks remain under task 3.3.

## Task 3.3: actual supply and reported operation

On 2026-09-13 the user reported: “I'm using a 1.5 amp supply at 5 volts and everything is working well with good torque.” This supplements the earlier confirmation of working motor/servo controls and neutral behavior.

| Power check | Actual evidence/status |
| --- | --- |
| Supply voltage and capacity | User-reported 5 V DC, 1.5 A supply rating. Supply model, label photograph/datasheet, regulation tolerance, and current-limit behavior were not supplied or independently verified |
| Operation with the actual actuators | User reports everything working well with good torque. This is qualitative operational evidence; no quantified torque or simultaneous-start trial log was supplied |
| Voltage during combined movement/startup | **Not measured:** no load-connector voltage readings or transient captures supplied. Successful operation does not establish a minimum voltage or rule out short dips |
| Actual motor and servo peak/stall demand | **Not measured/documented for these specific units:** no current measurements or part-specific current specifications supplied |
| Combined peak-current margin | **Not established:** cannot calculate margin from the supply rating and qualitative torque alone |
| Branch and decoupling verification | No hardware-specific wire/connector ratings, capacitor values, or placement records supplied; teacher preparation guidance remains in the lesson materials |
| Deliberate stall test | No deliberate stall test performed by the agent or requested from the student; none is inferred from “good torque” |

Task 3.3 is complete as an evidence record of the actual reported supply and operation, with unperformed power checks explicitly distinguished from firmware/document validation. It does not certify worst-case supply adequacy or measured voltage stability. The 1.5 A rating is specific to this reported setup and is not adopted as a universal minimum or recommendation for other SG90/motor combinations. The teacher guide continues to require actual-part peak-current capacity, suitable margin, and load-voltage verification for preparation.
