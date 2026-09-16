## MODIFIED Requirements

### Requirement: Standalone coding fundamentals collection
The curriculum SHALL provide `coding-fundamentals-lessons/` with a collection index and four numbered, standalone lessons in this order: `01-hello-world`, `02-name-greeting`, `03-repeating-counter`, and `04-arduino-led-counting`. Each lesson SHALL state its learning goals and prerequisites, present a concise Core Mission before deeper explanation, produce an observable result, provide a bounded prediction or safe change-and-retry opportunity, and include concise troubleshooting in fifth-grade-friendly language.

#### Scenario: Follow the introductory sequence
- **WHEN** a student opens the coding-fundamentals collection index
- **THEN** the student can follow a progression from laptop output, to laptop input, to finite and indefinite repetition, and finally to a physical Arduino output

#### Scenario: Find the essential activity
- **WHEN** a student opens any coding-fundamentals lesson
- **THEN** the student can identify the short required path and distinguish it from optional cleanup, extensions, teacher notes, and deeper code explanation

### Requirement: Laptop hello-world output
Lesson 01 SHALL provide a small C program that prints `Hello, world!` followed by a newline. Its instructions SHALL guide the student to build and run the supplied program for an initial success before distinguishing source code, GCC building, the resulting executable, and terminal execution. The lesson SHALL then invite one bounded greeting edit followed by rebuilding and observing the changed output.

#### Scenario: Build and run hello world
- **WHEN** the student follows lesson 01 on the prepared Linux workstation
- **THEN** GCC builds the C source without warnings and the executable prints `Hello, world!`

#### Scenario: Change the visible greeting
- **WHEN** the student changes only the message inside the documented quotation marks and rebuilds
- **THEN** the executable prints the new message and the lesson connects that observed change to edited source code

### Requirement: Laptop name input and greeting
Lesson 02 SHALL provide a bounded C program that asks for the student's name, accepts a nonempty line including spaces, stores it, and prints `Nice to meet you, <name>!`. The student explanation SHALL present the stored name as a labeled place for input and SHALL explicitly mark bounded input, newline cleanup, and error handling as helper mechanics outside the student learning goal.

#### Scenario: Enter a name containing a space
- **WHEN** the student runs lesson 02 and enters `Ada Lovelace`
- **THEN** the program prints `Nice to meet you, Ada Lovelace!`

#### Scenario: Identify focus code
- **WHEN** the student compares the lesson's behavior map with the complete safe-input source
- **THEN** the student can identify ask, receive, store, and greet as the focus actions without being required to explain buffer bounds, end-of-input handling, or newline-removal mechanics

### Requirement: Repeating laptop counter
Lesson 03 SHALL first provide a finite C counter stage that visibly prints 1 through 5 once, followed by the final program that begins again at 1 and continues until the user stops it. Both stages SHALL use approximately one-second spacing. Instructions SHALL introduce the changing counter and finite loop before the indefinitely repeating loop, explain how to stop the final program, and identify the one-second wait as Linux-specific rather than portable ISO C behavior.

#### Scenario: Observe the finite counter
- **WHEN** the student builds and runs the finite precursor
- **THEN** it prints `1, 2, 3, 4, 5` on separate lines at approximately one-second intervals and then returns to the terminal prompt

#### Scenario: Observe more than one counting cycle
- **WHEN** the student runs the final lesson 03 program for at least six displayed values
- **THEN** the visible sequence begins `1, 2, 3, 4, 5, 1` with approximately one second between values

### Requirement: Arduino built-in LED counting groups
Lesson 04 SHALL remain an independently buildable Arduino Uno PlatformIO project that uses only USB and the board's built-in LED. Its final program SHALL repeatedly announce a group number from 1 through 5 in the Serial Monitor, blink the LED that many times, visibly separate consecutive groups, and restart at group 1 after group 5. Its Core Mission SHALL stage observation by first uploading and watching the light, then matching Serial messages with physical blink groups, and finally identifying the group-selection and blink-count loops.

#### Scenario: Build and upload the physical-output lesson
- **WHEN** the student builds and uploads lesson 04 to an Arduino Uno and opens the Serial Monitor at the documented rate
- **THEN** the displayed group numbers and built-in LED produce matching groups of one, two, three, four, and five blinks before restarting

#### Scenario: Observe before explaining both loops
- **WHEN** the student follows the staged Core Mission
- **THEN** the student sees the physical repeating pattern and matches it to Serial output before being asked to explain the two-loop structure

#### Scenario: Use no external circuit
- **WHEN** lesson 04 is prepared and run
- **THEN** it requires no breadboard, external LED, Snap Circuits assembly, external power supply, or jumper wiring

### Requirement: Focused beginner coding scope
The lessons SHALL consistently explain program, instruction, source code, build, run, input, output, stored value, counter, loop, and upload as they become relevant. Student understanding SHALL NOT depend on pointers, manual memory management, data structures, algorithms, compiler phases, command-line argument processing, safe-input helper internals, Arduino timer internals, or general C-versus-C++ language theory. Non-focus code SHALL be visibly identified as helper or reference material.

#### Scenario: Explain the laptop-to-Arduino transition
- **WHEN** a student completes all four lessons
- **THEN** the student can describe that laptop programs are built and run on the workstation while Arduino programs are built on the workstation, uploaded, and then run on the board

#### Scenario: Encounter incidental complexity
- **WHEN** a student sees a source line needed for safe input, tool setup, or platform operation but outside the current concept
- **THEN** the lesson identifies its role without requiring the student to explain its syntax or internal operation
