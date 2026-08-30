## Purpose

Defines progressive Arduino Uno lessons that use PWM, analog input, and timer interrupts to control a protected external DC motor circuit safely and observably.

## ADDED Requirements

### Requirement: Standalone PWM motor lesson projects
The curriculum SHALL provide `03-pwm-motor-ramp`, `04-adc-pwm-motor-control`, and `05-interrupt-driven-motor-control` as independently buildable Arduino Uno PlatformIO projects with their own configuration, source code, and student instructions.

#### Scenario: Open one motor lesson
- **WHEN** a student opens any one of the three motor lesson directories in VS Code
- **THEN** PlatformIO can build and upload that lesson without compiling another lesson project

### Requirement: Protected external motor power stage
The motor lessons SHALL document a teacher-verified low-side switching circuit in which Arduino D3 drives an RFP30N06LE gate through a 220–330 ohm series resistor, a 10 kilohm resistor holds the gate at shared ground, a two-AA B1 supply powers a 3–6 V yellow gearbox motor, and a 1N5817 Schottky diode is connected across the motor with its cathode toward B1 positive and its anode toward the MOSFET drain.

#### Scenario: Wire the motor circuit with power removed
- **WHEN** a student follows the wiring instructions before connecting USB or switching on B1
- **THEN** the MOSFET source, B1 negative, and Arduino GND share ground while B1 positive remains isolated from Arduino 5 V and VIN

#### Scenario: Interrupt motor current
- **WHEN** PWM turns the conducting MOSFET off while the motor is spinning
- **THEN** the flywheel diode provides a current path across the motor with the documented polarity

### Requirement: Programmatic eight-second PWM ramp
Lesson 03 SHALL repeatedly vary the D3 PWM command from 0 through 255 over approximately four seconds and from 255 through 0 over approximately four seconds.

#### Scenario: Observe one complete ramp cycle
- **WHEN** lesson 03 runs with the approved motor circuit powered
- **THEN** the commanded duty cycle rises from 0% to 100% and falls to 0% over an approximately eight-second cycle

#### Scenario: Compare command with physical response
- **WHEN** a student observes the motor during a complete ramp
- **THEN** the activity distinguishes the smoothly changing PWM command from motor behaviors such as a starting threshold, coasting, or non-linear speed changes

### Requirement: Blocking ADC-to-PWM motor control
Lesson 04 SHALL read A0 as a value from 0 through 1023, map it directly to a D3 PWM command from 0 through 255, report both values over the serial connection, and wait 500 milliseconds before the next sample-and-report cycle.

#### Scenario: Adjust the RV control
- **WHEN** a student changes the three-terminal RV position while lesson 04 is running
- **THEN** a subsequent sample maps the changed ADC reading to the motor PWM command and reports both values

#### Scenario: Observe blocking responsiveness
- **WHEN** the RV position changes immediately after a sample
- **THEN** the motor command can retain the preceding value until the next 500-millisecond sample-and-report cycle

### Requirement: Interrupt-driven ADC-to-PWM motor control
Lesson 05 SHALL update the A0 reading and corresponding D3 PWM command every 100 milliseconds from a timer interrupt while the foreground loop reports the latest values every 500 milliseconds and blocks between reports.

#### Scenario: Adjust the RV while the foreground waits
- **WHEN** a student changes the RV position during the foreground loop's 500-millisecond wait
- **THEN** timer interrupts continue updating the motor command at 100-millisecond intervals before the next serial report

#### Scenario: Report shared interrupt data safely
- **WHEN** the foreground loop prepares a serial report
- **THEN** it obtains a consistent snapshot of the most recent ADC and PWM values without performing serial output inside the interrupt handler

### Requirement: Safe cumulative motor learning activity
Each motor lesson SHALL provide fifth-grade-friendly prerequisites, build and upload steps, a wiring checkpoint, an observation activity, and troubleshooting that keeps the motor secured without a wheel or propeller and requires all power to be removed before circuit changes.

#### Scenario: Complete a motor lesson activity
- **WHEN** a student follows a motor lesson from wiring through observation
- **THEN** the student can relate the reported or programmed PWM value to observed motor behavior without handling a moving attachment or changing powered wiring

### Requirement: RV voltage-divider input
Lessons 04 and 05 SHALL retain the lesson-02 three-terminal RV voltage-divider arrangement with its outer terminals connected to Arduino 5 V and GND and its center terminal connected to A0.

#### Scenario: Reuse the analog control
- **WHEN** a student wires the RV with power removed for lesson 04 or lesson 05
- **THEN** the wiper can vary A0 within the Arduino 0–5 V input range without applying B1 positive to the analog input
