## MODIFIED Requirements

### Requirement: Multimeter observation
The first lesson SHALL stage digital-output measurement before external-circuit control. It SHALL first guide the student to compare the voltage at digital pin 8 with its HIGH and LOW program states while the external transistor control lead is disconnected, then connect the teacher-approved transistor assembly and compare the same pin states with the visible LED response.

#### Scenario: Measure D8 before connecting the external control
- **WHEN** the uploaded lesson runs with no external control lead connected to D8
- **THEN** the student can observe D8 alternate between a voltage near the Arduino logic-high level and a voltage near zero while learning that the program creates the changing output

#### Scenario: Compare HIGH state with LED
- **WHEN** digital pin 8 is in its HIGH interval after the approved external assembly is connected
- **THEN** the student can observe a voltage near the Arduino logic-high level and compare it with the LED state

#### Scenario: Compare LOW state with LED
- **WHEN** digital pin 8 is in its LOW interval after the approved external assembly is connected
- **THEN** the student can observe a voltage near zero volts and compare it with the LED state

### Requirement: Beginner PlatformIO upload guide
The first existing external-circuit Arduino lesson SHALL identify the coding-fundamentals built-in LED lesson as recommended prior learning and provide a concise Core Mission for connecting an Arduino Uno with a USB data cable, opening the standalone project in VS Code, building it, uploading it through the PlatformIO extension, and recognizing success. Detailed connection troubleshooting SHALL remain available after the Core Mission.

#### Scenario: Upload the lesson
- **WHEN** a student follows the guide with a connected Arduino Uno and the PlatformIO extension available
- **THEN** the student can build and upload the program and identify the successful upload message

#### Scenario: Continue after coding fundamentals
- **WHEN** a student moves from the built-in LED coding lesson to the first external-circuit Uno lesson
- **THEN** the instructions connect familiar build, upload, counter, loop, and output ideas to control of the existing transistor-and-LED assembly
