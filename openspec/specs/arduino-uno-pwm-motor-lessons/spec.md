# arduino-uno-pwm-motor-lessons Specification

## Purpose

Defines progressive Arduino Uno lessons that use PWM, analog input, and timer interrupts to control a protected external DC motor circuit safely and observably.

## Requirements

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
Lesson 04 SHALL read A0 as a value from 0 through 1023, map it directly to a D3 PWM command from 0 through 255, report both values over the serial connection, and wait 1,000 milliseconds before the next sample-and-report cycle.

#### Scenario: Adjust the RV control
- **WHEN** a student changes the three-terminal RV position while lesson 04 is running
- **THEN** a subsequent sample maps the changed ADC reading to the motor PWM command and reports both values

#### Scenario: Observe blocking responsiveness
- **WHEN** the RV position changes immediately after a sample
- **THEN** the motor command can retain the preceding value until the next 1,000-millisecond sample-and-report cycle

### Requirement: Interrupt-driven ADC-to-PWM motor control
Lesson 05 SHALL update the A0 reading and corresponding D3 PWM command every 100 milliseconds from a timer interrupt while the foreground loop reports the latest values every 1,000 milliseconds and blocks between reports.

#### Scenario: Adjust the RV while the foreground waits
- **WHEN** a student changes the RV position during the foreground loop's 1,000-millisecond wait
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

### Requirement: Printable robot attention comparison worksheet
The curriculum SHALL provide an editable student worksheet source and matching three-page printable PDF comparing lessons 04 and 05 in fifth-grade language. The worksheet SHALL include name and date fields, short explanations, readable code excerpts, a timing diagram, a clearly posed investigation question, a student-written hypothesis, experimental steps, observation prompts, and an evidence-based conclusion, and space for handwritten answers. It SHALL remain legible in black-and-white on US Letter paper without clipped content or split observation rows.

#### Scenario: Print the student activity
- **WHEN** a teacher prints the worksheet PDF on US Letter paper
- **THEN** the student receives three readable pages with code, diagrams, and usable writing space, without needing the IDE or teacher guide to understand the questions

### Requirement: Student hypothesis and evidence-based conclusion
The worksheet SHALL ask whether waiting 1000 ms between RV checks in lesson 04 produces more noticeable lag in motor response than checking every 100 ms in lesson 05. It SHALL define a hypothesis as a testable prediction, provide a clearly labeled space for the student to write one in their own words before testing, and ask for a reason. After the experiment, it SHALL ask whether the 100 ms version seemed more responsive, invite evidence from recorded trials, and ask whether the observations supported the student's hypothesis. It SHALL allow faster, similar, mixed, or unclear responses rather than assume the expected outcome was observed.

#### Scenario: Write a hypothesis before testing
- **WHEN** a student reaches the investigation question before running the comparison
- **THEN** the student has a clear question and ample writing space to predict which checking interval will produce more noticeable lag and explain why

#### Scenario: Draw a conclusion from observations
- **WHEN** a student has recorded results for both programs
- **THEN** the student reports whether the 100 ms version responded better, cites trial observations, and explains whether they support the hypothesis or leave the result uncertain

### Requirement: Accurate student explanation of control timing
The worksheet SHALL explain lesson 04's read, map, apply, report, and blocking 1,000-millisecond wait sequence, and that an RV change just after sampling can take about one second to affect the motor command. It SHALL explain lesson 05's timer interrupt and ISR as a brief checking routine that reads A0 and adjusts the D3 PWM command every 100 milliseconds, or ten times per second, while its main loop still waits between roughly one-second serial reports. The explanation SHALL distinguish repeated control checks from the PWM signal's switching frequency and state that the previous motor power command continues during lesson 04's wait. Student understanding SHALL NOT depend on asynchronous programming, timer register calculations, volatile variables, or atomic snapshots.

#### Scenario: Compare the two programs
- **WHEN** a student reads the worksheet explanations and timing diagram
- **THEN** the student can identify where lesson 04 waits, why its response time varies with when the RV moves, and why lesson 05 can change the motor command before the next computer message

### Requirement: Frequent monitoring connected to robot decisions
The worksheet SHALL connect following instructions with repeatedly checking changing controls and surroundings, using collision, damage, and overheating examples. It SHALL distinguish the RV control input used in this activity from obstacle or temperature sensors a robot would need for those other checks, and SHALL NOT claim the demonstrated programs detect obstacles, detect overheating, or guarantee collision avoidance or immediate stopping.

#### Scenario: Apply the lesson to a robot problem
- **WHEN** a student answers a reflection about an obstacle, changed instruction, or hot motor
- **THEN** the activity asks what information the robot would need to check and how checking frequently could help it respond sooner

### Requirement: Guided B1 and RV responsiveness comparison
The worksheet and separate teacher guide SHALL use the existing B1 two-AA supply, three-terminal RV on A0, and protected D3 motor circuit for both lessons. The activity SHALL provide numbered experimental steps to run lesson 04 first, record its response, switch to lesson 05 with motor power off during upload, repeat the same RV movements, and compare the results. It SHALL include at least three matching low-to-high or high-to-low trials in each program, use RV movements just after serial reports to make lesson 04's lag observable, and record qualitative motor response rather than require precise reaction-time measurement. Both lessons SHALL link to the shared activity, and the lesson index SHALL identify it.

#### Scenario: Compare responsiveness fairly
- **WHEN** the student and teacher run the two programs using the comparison instructions
- **THEN** the circuit, supply, RV movements, and reporting pace remain comparable while the control-check schedule changes, and the student records observations for both programs

### Requirement: Teacher guidance for safe and accurate interpretation
The teacher guide SHALL provide a short lesson sequence, expected answers, an example hypothesis that 1000 ms between checks will produce more noticeable motor-speed response lag than checks every 100 ms, guidance to accept student hypotheses and conclusions based on their own observations, demonstration preparation, and references to existing build, upload, wiring, and safety instructions. It SHALL require the secured bare motor with no wheel or propeller, teacher verification before power, both power sources removed before wiring changes, and motor power off during uploads. It SHALL explain that PWM command updates are distinct from physical motor response, that motors can coast or have starting thresholds, and that ten checks per second is the demonstration rate rather than a universally safe rate for every robot.

#### Scenario: Prepare and interpret the demonstration
- **WHEN** a teacher follows the guide
- **THEN** the teacher can conduct the B1/RV comparison using existing safety precautions and explain differences between serial reporting, control updates, and observed motor motion without requiring advanced code details from the student

### Requirement: Standalone two-axis motor and servo lesson
The curriculum SHALL provide `06-joystick-motor-servo-control` as an independently buildable Arduino Uno PlatformIO project with fifth-grade instructions. It SHALL use an Elegoo analog joystick to control the existing protected DC motor and an SG90 position servo. Lessons 04 and 05 and their shared worksheet SHALL retain their existing behavior and wiring.

#### Scenario: Open the follow-on project
- **WHEN** a student opens lesson 06
- **THEN** the project builds independently and its instructions identify lesson 05 as prior learning

### Requirement: Twenty-millisecond two-channel interrupt control
Lesson 06 SHALL read joystick axes on A0 and A1 and update the D3 motor power command and D9 servo position command from a timer interrupt every 20 ms, approximately 50 times per second. Both output signals SHALL continue between command updates. The servo signal SHALL repeat on a nominal 20 ms frame with bounded position pulse widths, distinct from motor duty-cycle PWM. The foreground SHALL report a consistent snapshot of both readings and commands approximately once per second while blocking between reports, without serial output or pulse-length waits inside the control interrupt.

#### Scenario: Move both axes while the foreground waits
- **WHEN** both joystick axes change during the foreground's reporting wait
- **THEN** the interrupt routine reads both channels and updates both commands on its 20 ms schedule without waiting for the next report

#### Scenario: Observe the signals
- **WHEN** the servo and motor outputs are measured
- **THEN** servo frames repeat approximately every 20 ms with the selected bounded pulse width while motor PWM continues at its separate switching frequency

### Requirement: Center-off motor and bounded servo mapping
The centered motor-control axis, its neutral deadband, and the opposite direction SHALL command zero motor power. Movement beyond the deadband in the active direction SHALL increase the command monotonically to its maximum. The other axis SHALL map to a bounded servo position with center corresponding to the middle command. Instructions SHALL explain the active direction, neutral check, and conservative servo limits without promising exact angles or instant physical response.

#### Scenario: Release the joystick
- **WHEN** the motor-control axis returns to the verified neutral deadband
- **THEN** the next control check commands zero motor power, although the motor may coast

#### Scenario: Use the opposite direction or endpoints
- **WHEN** the motor axis moves in the inactive direction or the servo axis reaches either endpoint
- **THEN** motor power remains zero in the inactive direction and servo commands remain within the documented limits

### Requirement: External five-volt actuator power
Lesson 06 SHALL document a regulated external 5 V supply for the servo and the documented 3–6 V DC motor, selected for their combined starting/stall current with suitable margin. USB SHALL power the Uno and Arduino 5 V SHALL power the joystick. External actuator positive SHALL remain separate from Arduino 5 V and VIN; actuator negative, servo ground, MOSFET source, joystick ground, and Arduino GND SHALL share a ground reference. The motor SHALL retain its MOSFET, gate series resistor, gate pulldown, and correctly oriented flywheel diode. The servo SHALL receive power directly from the actuator supply and a separate control signal.

#### Scenario: Prepare power with the teacher
- **WHEN** the teacher checks the unpowered lesson 06 circuit
- **THEN** the instructions support verification of 5 V polarity, combined current capacity, shared ground, isolated positive supplies, motor protection, and servo connector polarity before actuator power is enabled

### Requirement: Guided two-axis investigation
Lesson 06 SHALL provide wiring, build/upload, preparation, an observation activity, and troubleshooting. It SHALL require all power removed before wiring changes, actuator power off during uploads, teacher verification before power, a secured bare motor without a wheel or propeller, and a secured unloaded servo with a clear motion area. The activity SHALL separately exercise each axis and then both together, record observations honestly, and explain 50 command checks per second versus servo frames, motor PWM pulses, and serial reporting. Lesson 05 and the lesson index SHALL link to lesson 06.

#### Scenario: Compare independent and combined controls
- **WHEN** a student follows the approved activity
- **THEN** the student observes motor neutral and active-direction behavior, bounded servo movement, and both controls responding between serial reports without handling moving parts

#### Scenario: Unexpected behavior occurs
- **WHEN** a motor fails to turn when commanded, a servo binds or persistently buzzes, parts become hot, or power becomes unstable
- **THEN** instructions direct the teacher and student to remove actuator and USB power and inspect the setup before continuing
