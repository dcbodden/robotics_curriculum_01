## Purpose

Defines progressive, independently buildable Arduino Uno lessons that connect simple programs to observable physical-computing behavior and beginner-friendly measurement activities.

## Requirements

### Requirement: Standalone Arduino Uno lesson projects
The curriculum SHALL keep each Arduino Uno lesson as an independently buildable PlatformIO project with its own configuration, source code, and student instructions.

#### Scenario: Open the first GPIO lesson
- **WHEN** a student opens the first GPIO lesson directory in VS Code
- **THEN** the directory contains everything PlatformIO needs to build and upload that lesson without compiling another lesson

### Requirement: Two-second GPIO state cycle
The first lesson SHALL configure Arduino Uno digital pin 8 as an output and alternate it between HIGH and LOW, holding each state for two seconds.

#### Scenario: Observe repeated output states
- **WHEN** the uploaded lesson runs on an Arduino Uno
- **THEN** digital pin 8 repeatedly remains HIGH for two seconds and LOW for two seconds

### Requirement: Existing Snap Circuits assembly connection
The first lesson SHALL explain how to connect Arduino ground to the existing Snap Circuits project ground and Arduino digital pin 8 to the existing NPN transistor base control point without instructing the student to rebuild the transistor-and-LED assembly.

#### Scenario: Connect the external assembly
- **WHEN** a student follows the wiring instructions with power disconnected
- **THEN** the Arduino and Snap Circuits project share ground and digital pin 8 supplies the transistor base control signal

### Requirement: Multimeter observation
The first lesson SHALL guide a student to compare the voltage at digital pin 8 with the visible LED state during both two-second output states.

#### Scenario: Compare HIGH state with LED
- **WHEN** digital pin 8 is in its HIGH interval
- **THEN** the student can observe a voltage near the Arduino logic-high level and compare it with the LED state

#### Scenario: Compare LOW state with LED
- **WHEN** digital pin 8 is in its LOW interval
- **THEN** the student can observe a voltage near zero volts and compare it with the LED state

### Requirement: Beginner PlatformIO upload guide
The first lesson SHALL provide fifth-grade-friendly steps for connecting an Arduino Uno to the workstation with a USB data cable, opening the standalone project in VS Code, building it, uploading it through the PlatformIO extension, and recognizing success or common connection problems.

#### Scenario: Upload the lesson
- **WHEN** a student follows the guide with a connected Arduino Uno and the PlatformIO extension available
- **THEN** the student can build and upload the program and identify the successful upload message
