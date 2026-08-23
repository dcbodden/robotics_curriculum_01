# arduino-uno-adc-lessons Specification

## Purpose

Defines beginner Arduino Uno lessons that turn analog input measurements into observable program behavior while remaining safe, independently buildable, and suitable for classroom experimentation.

## Requirements

### Requirement: Standalone second Arduino Uno lesson
The curriculum SHALL provide `02-adc-variable-delay` as an independently buildable Arduino Uno PlatformIO project beside `01-gpio-transistor-switch`, with its own configuration, source code, and student instructions.

#### Scenario: Open the ADC lesson
- **WHEN** a student opens the `02-adc-variable-delay` directory in VS Code
- **THEN** PlatformIO can build and upload that lesson without compiling lesson 01 or another project

### Requirement: Ten-bit analog input measurement
The lesson SHALL read analog input A0 and treat the Arduino Uno ADC result as a value in the inclusive range 0–1023.

#### Scenario: Read the adjustable input
- **WHEN** the lesson is running and the voltage on A0 changes within the supported 0–5 V input range
- **THEN** the program obtains a corresponding ADC value from 0 through 1023

### Requirement: Analog-controlled blink timing
The lesson SHALL map ADC readings from 0–1023 to delay values from 100–2,000 milliseconds and use the mapped value for both HIGH and LOW intervals on digital pin 8, with lower readings producing shorter intervals and higher readings producing longer intervals.

#### Scenario: Set a low analog input
- **WHEN** A0 produces an ADC reading near 0
- **THEN** digital pin 8 alternates using intervals near 100 milliseconds

#### Scenario: Set a high analog input
- **WHEN** A0 produces an ADC reading near 1023
- **THEN** digital pin 8 alternates using intervals near 2,000 milliseconds

#### Scenario: Adjust the input while running
- **WHEN** a student changes the A0 voltage while the sketch is running
- **THEN** a subsequent HIGH or LOW interval uses a delay derived from a recent ADC reading

### Requirement: Serial observation of conversion results
The lesson SHALL report the raw ADC reading and its mapped delay value over a serial connection at a rate that remains readable during normal operation.

#### Scenario: Compare input and timing values
- **WHEN** a student opens the serial monitor at the documented baud rate
- **THEN** each reported sample identifies both the 0–1023 ADC reading and the corresponding delay in milliseconds

### Requirement: Safe potentiometer input instructions
The lesson SHALL explain how to wire a three-terminal potentiometer as a voltage divider with its outer terminals connected to Arduino 5 V and GND and its center terminal connected to A0, while retaining the shared-ground and pin-8 output arrangement from lesson 01.

#### Scenario: Wire the analog control
- **WHEN** a student follows the instructions with power disconnected
- **THEN** rotating the potentiometer can vary A0 between ground and the Arduino 5 V rail without connecting an external voltage source to A0

### Requirement: Beginner ADC learning activity
The lesson SHALL explain ADC resolution, the relationship between input voltage and digital readings, the direction of the delay mapping, and a build-upload-observe experiment in fifth-grade-friendly language.

#### Scenario: Complete the observation activity
- **WHEN** a student builds and uploads the lesson and tests at least three potentiometer positions
- **THEN** the student can compare the reported ADC values, reported delays, and observed blink speeds