## Context

See proposal.md for motivation. The current curriculum already has working, independently buildable programs and careful hardware safety material. Its difficulty comes mainly from presentation order: student guides mix essential action, incidental syntax, detailed troubleshooting, and teacher-level implementation notes, while the root and collection indexes do not currently show one prerequisite-consistent route.

The change spans several lesson collections and must preserve the established directory numbering, final program outputs, verified circuit arrangements, actuator power boundaries, and existing printable robot-attention investigation.

## Goals / Non-Goals

**Goals:** Make the required path through each lesson immediately visible; produce an observable success before detailed explanation; introduce one main concept at a time; give the student a bounded opportunity to change or compare something; align all indexes to one primary route; and keep technical depth available without making it required student reading.

**Non-Goals:** Replace C or PlatformIO, create a general computer-science course, remove authentic source code, hide electrical hazards, reduce teacher checkpoints, redesign final firmware behavior, renumber lesson directories, or guarantee a fixed lesson duration for every learner.

## Decisions

### Use one canonical route with optional side paths

The root README and collection indexes will show this primary order:

```text
Multimeter and Ohm's Law
        ↓
Coding Fundamentals 01–04
        ↓
Uno 01: digital output and transistor-controlled LED
        ↓
Uno 02: ADC-controlled blink timing
        ↓
Manual Voltage Mapping reinforcement
        ↓
Uno 03–06: PWM, direct control, frequent attention, two-axis control
```

Links may still advertise optional related activities, but they will not direct a beginner to lesson 02 before lesson 01. The multimeter lesson stays first because it supplies safe measurement habits before live D8 and external-circuit observations. Manual voltage mapping stays after the ADC lesson because its eight buckets reinforce an effect the student has already seen.

Maintaining separate “electronics” and “coding” suggested sequences without a primary merged route was rejected because their current entry points can contradict lesson prerequisites.

### Put a Core Mission before reference depth

Each student README will begin its activity with a compact Core Mission containing:

1. the result the student is trying to produce;
2. only the preparation and safety steps required to reach it;
3. numbered actions;
4. one observation checkpoint; and
5. the next safe stopping point.

Concept explanation, Try One Change work, troubleshooting, and optional deeper detail will follow in clearly named sections. Hardware Core Missions will repeat essential power-removal, teacher-check, and emergency-stop directions even when a teacher guide also contains them. Detailed component selection, Linux permissions, timer math, and uncommon failure diagnosis may move to teacher guides or technical notes.

A strict word or page limit was rejected because wiring activities require more safety content than laptop programs. The design instead measures success by whether a learner can locate and complete the required path without reading optional material first.

### Use guided release and postpone the first prediction

For a concept's first appearance, lessons will use:

```text
MAKE IT WORK → NOTICE → NAME THE IDEA → CHANGE OR COMPARE → PREDICT → TRY → EXPLAIN
```

The supplied baseline gives the student evidence before prediction becomes meaningful. Later lessons may predict before running when the student has already encountered the relevant concept. This retains prediction as scientific practice while avoiding an unfamiliar-code guessing exercise.

The teacher guide will describe an “I show, we do, you try” option without requiring a teacher demonstration when a student is ready to proceed independently.

### Keep correct helper code and mark the learning surface

Lesson 02 will retain its bounded whole-line input behavior and error handling. Its guide will map the complete program into focus actions—ask, receive, store, greet—and helper actions—bounds, end-of-input handling, and newline cleanup. The code need not be weakened or replaced merely to look shorter.

The same visual distinction will be applied to Arduino library setup, timer configuration, atomic snapshots, and servo pulse machinery. Real source remains available; assessment prompts address only the focus code named by the lesson.

Simplifying input with an unsafe or single-word scanning function was rejected. Moving every helper into a new abstraction was also rejected because extra files and unexplained function calls can exchange one kind of mystery for another.

### Add one finite counter precursor without changing the final program

Lesson 03 will add a small `count-once.c` precursor with its own documented executable name. The student first observes 1 through 5 followed by a returned prompt, then builds the existing `main.c` and observes the restart at 1. Both retain one-second spacing and prompt flushing as needed. The final `repeating-counter` behavior and source remain the lesson's destination.

Editing braces into a working program during the first loop lesson was rejected because punctuation mistakes could obscure the finite-versus-indefinite comparison.

### Stage the existing built-in LED firmware through observation

Lesson 04 will continue to use one final PlatformIO project and the existing grouped-blink firmware. The guide will break the same run into three observations: watch the LED without interpreting code, open Serial Monitor and match numbers to groups, then trace the outer and inner loops. This avoids maintaining multiple Arduino source variants while still separating upload, physical output, and loop reasoning.

### Isolate output measurement before adding circuit behavior

Uno lesson 01 will upload its existing D8 HIGH/LOW program with the external control lead disconnected. The student will measure D8 relative to Arduino GND in voltage mode, then remove USB power before connecting the teacher-approved Snap Circuits ground and protected transistor control point. A second observation will compare the same D8 states with the external LED. This keeps the firmware and assembly unchanged while separating program-created voltage from transistor behavior.

### Reuse the light assembly as the PWM bridge

Before powering the lesson 03 motor circuit, the teacher-approved transistor-and-LED assembly will be controlled from D3 using the existing PWM ramp firmware. With power removed, the control lead moves from D8 to D3 as documented. The student observes the light change and relates it to the 0–255 command. The lesson then stops at a power-off checkpoint before preparing the protected MOSFET motor circuit.

The bridge will not claim that LED brightness is a direct measurement of motor speed or exact PWM duty cycle. It introduces the repeated-power idea with a visible, stationary load before adding a flywheel diode, separate motor supply, starting threshold, coasting, and moving shaft.

Adding a newly numbered PWM-LED project was rejected because the existing lesson 03 firmware and protected LED assembly can provide the checkpoint without shifting the established sequence.

### Teach interrupt behavior with timelines and move implementation depth

Lesson 05's Core Mission and robot-attention materials will center on two timelines: one control check per 1,000 ms versus ten checks during the same interval. Student text will describe the ISR as a brief robot-attention routine and distinguish control updates from one-second reports and motor PWM switching.

Register names, prescaler calculations, `volatile`, atomic snapshots, 8-bit/16-bit copy hazards, and the trade-off of calling `analogRead()` in an ISR will move to a teacher or technical-reference section. The actual firmware remains unchanged and available for inspection. Troubleshooting that asks for register-level inspection will be teacher-facing.

Removing the authentic interrupt implementation was rejected because later teachers and advanced learners need it, and the working timing behavior should remain verifiable.

### Divide lesson 06 by powered capability

Lesson 06 will retain one circuit and final program but present four gated missions:

1. actuator power off: verify both joystick readings and a zero neutral motor command;
2. actuator power on with teacher approval: move the secured unloaded servo while keeping the motor command zero;
3. observe motor center-off and active-direction behavior while holding the servo axis near center;
4. combine both axes and observe response between serial reports.

The student README will retain connector colors, shared ground, separated positive supplies, clear motion areas, and stop conditions. Supply capacity selection, voltage-stability diagnosis, timer modes, pulse buffering, and calibration rationale remain in the teacher guide or deeper reference. This separates learning steps without weakening the existing power and motion controls.

## Risks / Trade-offs

- [Core Missions duplicate some safety or setup text] → Accept deliberate repetition where it prevents a student from bypassing a required power-removal or teacher checkpoint.
- [More headings make documents longer] → Move detail rather than merely adding summaries; review the required path separately from optional and teacher material.
- [A finite precursor adds another executable] → Give it a distinct name, ignore only that exact generated file, and verify both documented build commands.
- [Changing the lesson 01 measurement order could lead to live rewiring] → Put an explicit USB-disconnect checkpoint between direct D8 measurement and connection of the external assembly.
- [The PWM light assembly may respond with different apparent direction or brightness] → Ask for actual observation and avoid promising linear brightness or a specific polarity response.
- [Moving technical detail may make it harder to find] → Link the teacher or deeper-reference section directly from the simplified student explanation and keep the real source unchanged.
- [Lesson 06 staging may imply separate circuit rebuilds] → State that the approved circuit remains fixed and that stages change only joystick actions and whether actuator power is enabled.

## Migration Plan

Update the canonical indexes first, then revise the shared teacher guidance and coding fundamentals. Add and verify the finite counter precursor. Restructure Uno lessons 01, 03, 05, and 06 around their Core Missions and reference layers without changing final firmware. Rebuild affected programs, check every local link and documented command, and record hardware observations as performed or not performed.

Rollback restores the prior documentation and removes the finite precursor; no directory renaming, firmware migration, or circuit conversion is required.
