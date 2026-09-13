## Why

Lessons 04 and 05 demonstrate different control response times, but a fifth-grade student needs a focused, printable comparison that connects those times to robot decisions. The lesson will explain why a robot must keep monitoring changing controls and surroundings while following its instructions.

## What Changes

- Add a two-page, fifth-grade student worksheet comparing lesson 04's blocking control loop with lesson 05's timer-driven control checks, with an editable source and printable PDF.
- Explain lesson 04's read, map, apply, report, and one-second wait steps using small code excerpts and plain language; contrast these with lesson 05's 100-millisecond ISR checks and ten power-command updates per second.
- Include a timing diagram, prediction, repeated RV observations, and reflection about collisions, damage, and overheating.
- Add a separate teacher guide with demonstration steps, answer guidance, safety checkpoints, and distinctions between command timing, serial reporting, and physical motor response.
- Link the comparison materials from both lesson READMEs and the lesson index.
- Use the existing Snap Circuits B1 battery holder, RV voltage-divider control, and protected motor circuit for both demonstrations.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `arduino-uno-pwm-motor-lessons`: Require a printable, fifth-grade comparison activity and teacher guidance connecting blocking versus interrupt-driven control checks to responsive robot behavior.

## Impact

Curriculum materials under `platformio-uno-lessons/`, primarily lesson 05, with links from lesson 04 and the top-level lesson index. Adds worksheet source, printable PDF, and teacher guidance. Existing firmware, timing, pin assignments, motor protection, and B1/RV wiring remain the basis of the demonstration. No new runtime dependency or hardware is required.
