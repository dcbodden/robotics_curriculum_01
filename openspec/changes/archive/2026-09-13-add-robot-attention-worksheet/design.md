## Context

See proposal.md for motivation and the delta spec for the student-facing contract. The existing programs already provide the desired comparison: lesson 04 reads A0 and updates D3 in loop() before a 1,000 ms delay; lesson 05 performs those operations in ISR(TIMER1_COMPA_vect) every 100 ms while loop() prints snapshots and delays. Both READMEs already document the B1/RV circuit and comparison precautions.

This design is included because the shared activity spans both lessons and needs deliberate decisions about printable layout and age-appropriate explanations.

## Goals / Non-Goals

**Goals:** Keep the student reading, forming a testable hypothesis, observing, and explaining cause and effect; provide reproducible print output; keep one shared comparison activity easy to find from either lesson.

**Non-Goals:** Firmware changes, joystick wiring, extra sensors, moving-robot tests, asynchronous programming instruction, or student-level explanations of timer registers and shared-memory mechanics.

## Decisions

### Store one shared activity with lesson 05

Create `platformio-uno-lessons/05-interrupt-driven-motor-control/robot-attention-worksheet.html`, its matching `robot-attention-worksheet.pdf`, and `robot-attention-teacher-guide.md`. Link them from both lesson READMEs and `platformio-uno-lessons/README.md`. Lesson 05 hosts the comparison because it builds on lesson 04. Duplicating worksheets in both directories risks inconsistent revisions; a new top-level curriculum directory adds unnecessary organization for one activity.

### Use printable HTML as the editable worksheet source

Use self-contained HTML with print CSS, US Letter page sizing, deliberate page breaks, readable monochrome type, and no remote assets. Generate and commit the matching PDF and document the generation command in the teacher guide. This gives explicit control over writing space and page boundaries; plain Markdown rendering is less predictable for a three-page worksheet. No image generation or external fonts are needed.

Use three spacious pages rather than compress the investigation into the explanation page:

- Page 1 introduces robot attention, explains lesson 04's five steps, contrasts lesson 05's ISR, and includes the timing diagram. Preserve readable code excerpts and 12 pt body text.
- Page 2 poses “Will the motor take longer to respond to an RV change when checks are 1000 ms apart than when they are 100 ms apart?” Define a hypothesis as a prediction we can test. Ask the student to write their hypothesis and reason in their own words, with at least four handwriting lines. Follow with numbered experimental steps for running lesson 04, recording observations on page 3, switching to lesson 05, and repeating the comparison.
- Page 3 provides at least three paired trial records with generous writing areas, then asks whether the 100 ms version seemed more responsive, which trial observations support that conclusion, and whether the student's hypothesis was supported. Retain the ten-checks-per-second calculation and a robot-safety transfer question.

The teacher's example hypothesis is “A 1000 ms delay between checks of the RV position will cause a more noticeable lag in motor-speed response than checking every 100 ms.” Keep this in the teacher guide; the student gets the question and writes their own prediction. Record hypotheses before testing and observations before conclusions. Accept similar, mixed, and unclear observations as well as the expected faster response; discuss evidence rather than grade agreement with the example hypothesis.

Keep excerpts short and faithful to the current code; explain that the named delay constants equal 1,000 ms rather than present `delay(1000)` as a literal source line. A one-second diagram has ten 100 ms intervals; boundary marks must not be presented as eleven checks within a single second.

### Teach checking frequency with a timer-reminder analogy

Use “Follow the plan—and keep checking what has changed.” Explain an interrupt as a timer reminder that briefly pauses regular work to run the checking routine, then returns to that work. Show the read/map/apply operations inside the actual ISR, but omit its register setup and variable-sharing details from student text. A full source listing would distract from the intended learning objective.

Explicitly state that lesson 05 still calls delay(), both programs print about once per second, and the PWM power setting is reconsidered ten times per second even if the RV remains unchanged. Explain that hardware keeps applying the old setting during lesson 04's wait. Avoid describing the programs as simultaneous workers or the PWM pulses themselves as occurring ten times a second.

### Reuse the approved B1/RV demonstration

Keep the existing RV voltage divider and protected motor wiring. Number the procedure: prepare the approved unpowered circuit, upload lesson 04 with B1 off, enable motor power after approval, make and record at least three low/high RV changes just after reports, turn B1 off for the lesson 05 upload, repeat the same movements, then turn B1 off and disconnect USB. Keep all wires unchanged between programs. Use comparable low-to-high and high-to-low movements just after reports and at least three paired trials; teacher notes can suggest additional repetitions if needed. Choose low and high values that make motor behavior audible or visible, and record qualitative delay rather than stopwatch readings. Reference existing wiring and upload instructions rather than duplicate their full tables.

### Separate the teacher's interpretation from the student's reading

The Markdown teacher guide provides an approximately 20–30 minute sequence, preparation, print generation instructions, expected answers, and interpretation notes. This duration is a planning assumption, not an acceptance constraint. Collision and overheating examples are discussion scenarios requiring appropriate sensors; the bench activity demonstrates a changed control instruction only. Explain coasting, starting thresholds, and why appropriate checking rates depend on the robot and hazard. Do not conduct collisions or overheating tests.

## Risks / Trade-offs

- [Investigation becomes crowded or ambiguous] → Separate background, hypothesis/procedure, and observations/conclusion across three pages; use explicit prompts and preserve handwriting space.
- [Computer messages are mistaken for control checks] → State the shared reporting pace and direct observation of motor changes between messages in both materials.
- [Motor inertia hides timing differences] → Use repeated large RV changes and qualitative observations, with teacher guidance on coasting and starting thresholds.
- [Frequent checks are treated as automatic safety] → Ask what sensor and response a robot needs; avoid guaranteed stopping or collision-prevention claims.
- [PDF drifts from source] → Regenerate after every worksheet edit and inspect all three pages before completion.

## Migration Plan

Add the three activity files and README links without altering firmware or wiring instructions. Render the PDF, inspect the three-page layout and question-to-conclusion sequence, and verify all links and timing claims against the source programs. The materials can be withdrawn by removing the activity files and their links; there is no runtime migration.
