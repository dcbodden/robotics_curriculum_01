## ADDED Requirements

### Requirement: PWM light bridge before motor motion
Before lesson 03 asks the student to power the DC motor, it SHALL provide a teacher-approved checkpoint that applies the lesson's changing D3 PWM command to the existing protected transistor-and-LED assembly so the student can observe changing light output before adding motor power, flywheel protection, and moving-part behavior. The instructions SHALL require power removal before moving the control connection between D8 and D3 or changing the circuit.

#### Scenario: Observe PWM with a light
- **WHEN** the student runs the lesson 03 PWM ramp with the approved transistor-and-LED assembly controlled from D3
- **THEN** the student observes changing light output and connects it to the changing PWM command before preparing the motor circuit

#### Scenario: Transition from light to motor safely
- **WHEN** the class finishes the PWM light checkpoint and prepares the motor activity
- **THEN** USB and external power are removed before any connection changes and the motor's MOSFET, gate resistors, flywheel diode, separate supply, and secured-shaft checks remain required

## MODIFIED Requirements

### Requirement: Safe cumulative motor learning activity
Each motor lesson SHALL provide fifth-grade-friendly prerequisites and a concise Core Mission before deeper explanation and exhaustive troubleshooting. The required path SHALL include build and upload steps, a wiring checkpoint, one observable comparison, and all safety instructions needed to keep the motor secured without a wheel or propeller and remove all power before circuit changes. Component identification, failure diagnosis, and implementation details outside the immediate observation SHALL be separated into teacher or deeper-reference sections.

#### Scenario: Complete a motor lesson activity
- **WHEN** a student follows a motor lesson from wiring through observation
- **THEN** the student can relate the reported or programmed PWM value to observed motor behavior without handling a moving attachment, changing powered wiring, or first mastering optional technical detail

### Requirement: Accurate student explanation of control timing
The lesson 04/05 worksheet and lesson 05 student Core Mission SHALL explain lesson 04's read, map, apply, report, and blocking 1,000-millisecond wait sequence, including that an RV change just after sampling can take about one second to affect the motor command. They SHALL explain lesson 05's timer interrupt and ISR as a brief checking routine that reads A0 and adjusts the D3 PWM command every 100 milliseconds, or ten times per second, while its main loop still waits between roughly one-second serial reports. The explanation SHALL distinguish repeated control checks from the PWM signal's switching frequency and state that the previous motor power command continues during lesson 04's wait. Student understanding SHALL NOT depend on asynchronous programming, timer register calculations, volatile variables, atomic snapshots, data-width hazards, or ISR implementation trade-offs; those details SHALL appear only in teacher or deeper-reference material.

#### Scenario: Compare the two programs
- **WHEN** a student reads the Core Mission or worksheet explanations and timing diagram
- **THEN** the student can identify where lesson 04 waits, why its response time varies with when the RV moves, and why lesson 05 can change the motor command before the next computer message

#### Scenario: Find deeper timer details
- **WHEN** a teacher or advanced learner needs to understand the real Timer1 and shared-data implementation
- **THEN** the curriculum retains that explanation outside the student's required path without making it an assessment prerequisite

### Requirement: Guided two-axis investigation
Lesson 06 SHALL provide wiring, build/upload, preparation, and troubleshooting, then stage its required observation work as power-off joystick input checks, servo-only motion, motor-only center-off behavior, and combined-axis control. It SHALL require all power removed before wiring changes, actuator power off during uploads, teacher verification before power, a secured bare motor without a wheel or propeller, and a secured unloaded servo with a clear motion area. The activity SHALL record observations honestly and explain 50 command checks per second versus servo frames, motor PWM pulses, and serial reporting. Lesson 05 and the lesson index SHALL link to lesson 06. Supply sizing, timer configuration, pulse buffering, and detailed failure analysis SHALL remain available to the teacher without blocking the student's Core Missions.

#### Scenario: Check inputs before actuator power
- **WHEN** the student uploads lesson 06 with actuator power disabled
- **THEN** the student verifies neutral motor commands and changing joystick readings before either actuator can move

#### Scenario: Compare independent and combined controls
- **WHEN** the student follows the approved staged activity
- **THEN** the student observes bounded servo movement by itself, motor neutral and active-direction behavior by itself, and both controls responding together without handling moving parts

#### Scenario: Unexpected behavior occurs
- **WHEN** a motor fails to turn when commanded, a servo binds or persistently buzzes, parts become hot, or power becomes unstable
- **THEN** instructions direct the teacher and student to remove actuator and USB power and inspect the setup before continuing
