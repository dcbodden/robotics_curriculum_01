## Why

The curriculum offers engaging, observable results, but several lessons ask a fifth-grade learner to absorb a programming concept, unfamiliar syntax, tool operation, and new hardware in the same pass. A clearer guided path and smaller success checkpoints can preserve the real code and hands-on experiments while reducing the chance that incidental complexity hides the lesson's main idea.

## What Changes

- Establish one canonical beginner route across the root guide and collection indexes: multimeter foundations, coding fundamentals, first digital-output circuit, ADC control, manual voltage-mapping reinforcement, PWM motor work, responsive control, and coordinated joystick control.
- Give each lesson a short **Core Mission** that reaches one observable result before optional explanation, extensions, and detailed troubleshooting.
- Use a concrete-first cycle: make it work, notice the result, name one idea, change one safe thing, predict the new result, try it, and explain the difference.
- Add explicit code-focus guidance that distinguishes the few student-learning lines from safe input helpers, library setup, timer configuration, and other teacher/reference code.
- Stage the repeating-counter lesson through a finite 1-through-5 run before indefinite repetition, while retaining its final repeating behavior.
- Stage the built-in LED lesson through upload, unaided LED observation, Serial matching, and two-loop explanation without adding external wiring or changing its final grouped-blink behavior.
- Add an isolated D8 voltage-observation checkpoint before the first external transistor connection and add a lower-risk PWM light checkpoint before motor PWM.
- Reframe interrupt-driven control around observable 1,000 ms versus 100 ms timelines; move register, `volatile`, atomic-snapshot, and ISR trade-off details out of the student core path and into teacher or deeper-reference material.
- Divide the joystick motor/servo investigation into separate power-off input checks, servo-only, motor-only, and combined-control missions while retaining all existing electrical and motion safety boundaries.
- Keep existing numbered directories and final firmware/circuit behaviors stable; add supporting stages and reorganize guidance without renumbering the established curriculum.

## Capabilities

### New Capabilities

- `beginner-curriculum-progression`: Defines the canonical cross-collection learning route, concrete-first lesson rhythm, layered Core Mission/reference presentation, and progressive observable checkpoints.

### Modified Capabilities

- `coding-fundamentals-lessons`: Stage the four introductory lessons into smaller successes, make student-focus code explicit, and add safe change-and-rebuild opportunities without changing their final required outputs.
- `electronics-fundamentals-lessons`: Reconcile its recommended order with the canonical beginner route and retain manual voltage mapping as reinforcement after the Arduino ADC experience.
- `arduino-uno-lessons`: Stage lesson 01 from direct D8 observation to the external transistor assembly and align its prerequisites with the canonical route.
- `arduino-uno-pwm-motor-lessons`: Add a PWM light bridge, simplify the student-facing interrupt model, and divide the motor/servo investigation into separately observable missions while preserving timing, wiring, power, and safety requirements.

## Impact

The change affects the repository guide; coding-, electronics-, and PlatformIO Uno collection indexes; student lesson READMEs; teacher guides; recording prompts; and small supporting lesson-stage source where a finite counter precursor is useful. It does not renumber existing lesson directories, replace safe input handling, change final firmware behavior, change established circuit requirements, or reduce teacher verification and power-removal rules. Verification will cover documented commands, all affected PlatformIO builds, local links, lesson-path consistency, and an explicit record of any hardware observations that were not performed.
