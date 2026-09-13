## ADDED Requirements

### Requirement: Printable robot attention comparison worksheet
The curriculum SHALL provide an editable student worksheet source and matching two-page printable PDF comparing lessons 04 and 05 in fifth-grade language. The worksheet SHALL include name and date fields, short explanations, readable code excerpts, a timing diagram, prediction and observation prompts, and space for handwritten answers. It SHALL remain legible in black-and-white on US Letter paper without clipped content or split observation rows.

#### Scenario: Print the student activity
- **WHEN** a teacher prints the worksheet PDF on US Letter paper
- **THEN** the student receives two readable pages with code, diagrams, and usable writing space, without needing the IDE or teacher guide to understand the questions

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
The worksheet and separate teacher guide SHALL use the existing B1 two-AA supply, three-terminal RV on A0, and protected D3 motor circuit for both lessons. The activity SHALL include at least three matching low-to-high or high-to-low trials in each program, use RV movements just after serial reports to make lesson 04's lag observable, and record qualitative motor response rather than require precise reaction-time measurement. Both lessons SHALL link to the shared activity, and the lesson index SHALL identify it.

#### Scenario: Compare responsiveness fairly
- **WHEN** the student and teacher run the two programs using the comparison instructions
- **THEN** the circuit, supply, RV movements, and reporting pace remain comparable while the control-check schedule changes, and the student records observations for both programs

### Requirement: Teacher guidance for safe and accurate interpretation
The teacher guide SHALL provide a short lesson sequence, expected answers, demonstration preparation, and references to existing build, upload, wiring, and safety instructions. It SHALL require the secured bare motor with no wheel or propeller, teacher verification before power, both power sources removed before wiring changes, and motor power off during uploads. It SHALL explain that PWM command updates are distinct from physical motor response, that motors can coast or have starting thresholds, and that ten checks per second is the demonstration rate rather than a universally safe rate for every robot.

#### Scenario: Prepare and interpret the demonstration
- **WHEN** a teacher follows the guide
- **THEN** the teacher can conduct the B1/RV comparison using existing safety precautions and explain differences between serial reporting, control updates, and observed motor motion without requiring advanced code details from the student
