## Purpose

Defines a short, fifth-grade-friendly sequence that introduces input, output, stored values, counters, loops, building, running, and uploading before external-circuit Arduino work.

## ADDED Requirements

### Requirement: Standalone coding fundamentals collection
The curriculum SHALL provide `coding-fundamentals-lessons/` with a collection index and four numbered, standalone lessons in this order: `01-hello-world`, `02-name-greeting`, `03-repeating-counter`, and `04-arduino-led-counting`. Each lesson SHALL state its learning goals, prerequisites, steps, expected observations, and concise troubleshooting in fifth-grade-friendly language.

#### Scenario: Follow the introductory sequence
- **WHEN** a student opens the coding-fundamentals collection index
- **THEN** the student can follow a progression from laptop output, to laptop input, to repetition, and finally to a physical Arduino output

### Requirement: Laptop hello-world output
Lesson 01 SHALL provide a small C program that prints `Hello, world!` followed by a newline. Its instructions SHALL distinguish source code, building with GCC on the Linux workstation, the resulting executable program, and running that program in a terminal.

#### Scenario: Build and run hello world
- **WHEN** the student follows lesson 01 on the prepared Linux workstation
- **THEN** GCC builds the C source without warnings and the executable prints `Hello, world!`

### Requirement: Laptop name input and greeting
Lesson 02 SHALL provide a bounded C program that asks for the student's name, accepts a nonempty line including spaces, stores it, and prints `Nice to meet you, <name>!`. The student explanation SHALL present the stored name as a labeled place for input without requiring memory-layout, array, or string-algorithm knowledge.

#### Scenario: Enter a name containing a space
- **WHEN** the student runs lesson 02 and enters `Ada Lovelace`
- **THEN** the program prints `Nice to meet you, Ada Lovelace!`

### Requirement: Repeating laptop counter
Lesson 03 SHALL provide a C program that visibly prints the numbers 1, 2, 3, 4, and 5 on separate lines, approximately one second apart, then begins again at 1 and continues until the user stops it. Instructions SHALL explain a counter, a finite 1-through-5 loop, an indefinitely repeating loop, and how to stop the terminal program, while identifying the one-second wait as Linux-specific rather than portable ISO C behavior.

#### Scenario: Observe more than one counting cycle
- **WHEN** the student runs lesson 03 for at least six displayed values
- **THEN** the visible sequence begins `1, 2, 3, 4, 5, 1` with approximately one second between values

### Requirement: Arduino built-in LED counting groups
Lesson 04 SHALL be an independently buildable Arduino Uno PlatformIO project that uses only USB and the board's built-in LED. It SHALL repeatedly announce a group number from 1 through 5 in the Serial Monitor, blink the LED that many times, visibly separate consecutive groups, and restart at group 1 after group 5.

#### Scenario: Build and upload the physical-output lesson
- **WHEN** the student builds and uploads lesson 04 to an Arduino Uno and opens the Serial Monitor at the documented rate
- **THEN** the displayed group numbers and built-in LED produce matching groups of one, two, three, four, and five blinks before restarting

#### Scenario: Use no external circuit
- **WHEN** lesson 04 is prepared and run
- **THEN** it requires no breadboard, external LED, Snap Circuits assembly, external power supply, or jumper wiring

### Requirement: Focused beginner coding scope
The lessons SHALL consistently explain program, instruction, source code, build, run, input, output, stored value, counter, loop, and upload as they become relevant. Student understanding SHALL NOT depend on pointers, manual memory management, data structures, algorithms, compiler phases, command-line argument processing, Arduino timer internals, or general C-versus-C++ language theory.

#### Scenario: Explain the laptop-to-Arduino transition
- **WHEN** a student completes all four lessons
- **THEN** the student can describe that laptop programs are built and run on the workstation while Arduino programs are built on the workstation, uploaded, and then run on the board

### Requirement: Beginner verification without internal implementation details
Each lesson SHALL provide a short observation or prediction prompt grounded in visible program behavior. Teacher verification SHALL build the three laptop programs with warnings enabled, exercise the name-with-spaces case, observe at least one counter restart, build the Uno project, and clearly record any upload or physical LED check that could not be performed.

#### Scenario: Review the completed lesson set
- **WHEN** the teacher performs available verification
- **THEN** the recorded results distinguish successful builds from observed runtime and hardware behavior and do not invent unperformed observations
