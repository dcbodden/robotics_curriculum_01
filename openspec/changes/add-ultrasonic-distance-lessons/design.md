## Context

Lesson 06 uses Timer1 overflow every nominal 20 ms to read A0/A1, update D3 motor PWM and D9 servo pulse commands, and publish telemetry; its foreground reports a snapshot while blocking. Lessons 08 and 09 must preserve that work while adding an HC-SR04 whose Echo pulse is captured on D2/INT0. The sensor recommends trigger cycles longer than 60 ms, while the behavior of interest in lesson 09 is a target closer than 10 cm, corresponding to an Echo HIGH duration of approximately 580 microseconds.

The HC-SR04 can advertise Echo durations near 23.2 ms at 4 m, but the interrupt-integrated lessons deliberately accept only measurements completed before the next 20 ms control frame. Lesson 07 remains the full-range blocking introduction. See the ultrasonic delta spec for observable behavior and the proposal for motivation.

## Goals / Non-Goals

**Goals:**

- Keep all joystick reads and motor/servo command updates on lesson 06's nominal 20 ms Timer1 schedule.
- Place the Trigger pulse after the control work so near-range Echo interrupts have nearly a full frame in which to complete.
- Separate short interrupt-time acquisition/control decisions from foreground floating-point conversion and Serial output.
- Make measurement validity, requested PWM, applied PWM, and the 50 percent policy directly observable and host-testable.
- Preserve the repository's standalone-project, concrete-first, and teacher-gated safety conventions.

**Non-Goals:**

- Provide safety-rated collision avoidance, a mobile robot trial, environmental compensation, sensor calibration, filtering, or precision ranging.
- Preserve interrupt-driven measurements whose Echo does not complete within one 20 ms control interval.
- Add an ultrasonic library or reassign Timer1, Timer2, D3, or D9.
- Change lessons 01–06 or claim that PWM command is a direct measurement of physical motor speed.

## Decisions

### Use D4 Trigger and D2/INT0 Echo

D2 provides the Uno's INT0 external interrupt while remaining unused by lesson 06; D4 is also unused and can generate the short Trigger pulse. Lessons use a CHANGE interrupt and an acquisition state machine rather than polling Echo.

Alternative considered: Timer1 input capture on D8 would provide hardware edge timestamps, but Timer1 is already configured around the servo frame and the requested lesson specifically teaches the common D2 external interrupt path.

### Trigger every fourth Timer1 frame

A small frame divider starts one measurement every four 20 ms control frames, yielding 80 ms spacing and satisfying the sensor recommendation of more than 60 ms. The Timer1 ISR first expires any prior incomplete acquisition, performs every normal ADC and actuator update, and then—only on the scheduled frame—arms Echo acquisition and produces the 10 microsecond D4 pulse as its final operation.

Alternative considered: 500 ms measurement spacing would match reports but make the sensor/control relationship less observable. Triggering every 20 ms would violate the documented spacing recommendation. Exactly three frames gives 60 ms rather than more than 60 ms.

### Bound interrupt acquisition to one control interval

Acquisition uses explicit `IDLE`, `WAITING_FOR_RISE`, and `WAITING_FOR_FALL` states. A recognized rising edge records `micros()`; the matching falling edge publishes unsigned elapsed microseconds and returns acquisition to `IDLE`. If either expected edge is still pending at the next Timer1 overflow, that ISR marks the attempt invalid before performing its normal control work. D2 edges that do not match the active state are ignored.

This intentionally rejects long-range Echo pulses around and beyond the control-frame interval. It avoids skipping joystick reads or carrying ambiguous edge state across the next control cycle, while comfortably covering the sub-10-cm decision range.

Alternative considered: pausing joystick work while Echo is active would preserve longer measurements but contradicts the requirement to perform every control cycle. Allowing acquisition to span frames would create ISR contention at the next frame and complicate the teaching model.

### Keep acquisition state separate from the latest control policy

Acquisition state describes the in-progress ping; policy state describes how the most recently completed attempt affects motor output. Starting a new measurement does not erase the previous policy. A valid falling edge updates the policy, while a timeout at the next Timer1 frame makes the result invalid and selects full-control policy.

This separation prevents the motor from oscillating back to full power merely because another measurement has started. It also gives one explicit place to apply the user's chosen invalid/no-Echo behavior.

### Compare raw duration in the ISR and convert units in foreground code

The D2 falling-edge path compares elapsed time directly with the 10 cm threshold of 580 microseconds. Lesson 09 then updates the slowdown flag and immediately issues either the latest requested PWM divided by two or the full request. The Timer1 ISR uses that same policy when it calculates each subsequent applied command. Floating-point millisecond and centimeter conversion occurs only when foreground code formats a report.

Alternative considered: calculate floating-point distance inside the D2 ISR. Direct duration comparison is equivalent at the policy boundary and keeps the ISR shorter and easier to reason about.

### Publish requested and applied commands separately

Lesson 09 retains a requested command produced by the lesson 06 joystick mapping and derives an applied command through a pure distance policy. Both values, the slowdown flag, measurement validity, and Echo duration are published for an atomic foreground snapshot. The 50 percent operation uses bounded integer arithmetic, so zero stays zero and 0–255 inputs cannot overflow the PWM range.

Lesson 08 has no distance policy and therefore preserves the lesson 06 motor command directly while adding measurement telemetry.

### Use bounded blocking measurement only in lesson 07

Lesson 07 uses the direct Arduino pulse-duration operation with an explicit timeout slightly beyond the approximately 23.2 ms maximum advertised Echo duration. It prints a fractional millisecond value and centimeters for a valid duration, or an explicit invalid/no-Echo line, then calls `delay(500)`. Lessons 08 and 09 replace that pulse wait with D2 edge capture while retaining approximately 500 ms foreground reports.

### Isolate deterministic math for host checks

Small headers will hold duration-to-distance conversion, threshold comparison, and requested-to-applied PWM mapping. Each standalone project will include the deterministic code it needs rather than reaching into another lesson directory. Host tests will exercise zero, boundary, representative, invalid, and maximum values; lessons 08 and 09 will also preserve the existing lesson 06 mapping checks.

### Layer student and teacher explanations

Each README begins with a teacher-gated Core Mission and one observable comparison. Implementation detail about external-interrupt state, atomic multi-byte values, ISR ordering, Timer ownership, and the deliberately shortened interrupt measurement range belongs in teacher/deeper-reference material. Lesson 09 uses only a secured bare motor and stationary target and explicitly describes the behavior as a demonstration rather than collision avoidance.

## Risks / Trade-offs

- **[Long-distance Echo is rejected in lessons 08 and 09]** → Document the one-frame validity window, report invalid status, and use lesson 07 for full advertised-range observation.
- **[A very early Echo edge could become pending before the Timer1 ISR returns]** → Make Trigger the ISR's final operation and keep all work after its falling edge to the minimum required ISR epilogue; INT0 services the pending edge immediately afterward.
- **[A late edge from an expired measurement could corrupt the next attempt]** → Ignore edges outside the expected state and leave 80 ms between triggers, well beyond the normal maximum Echo duration.
- **[The next Timer1 update could overwrite an immediate slowdown]** → Persist policy state and require both ISRs to derive applied PWM from the same latest requested command and policy.
- **[Rapid readings near 10 cm can alternate across the threshold]** → Present this as raw threshold behavior; do not add filtering or hysteresis that would obscure the lesson's single decision rule.
- **[`micros()` and shared durations are multi-byte on AVR]** → Use unsigned rollover-safe subtraction in the edge ISR and atomic foreground snapshots for duration and related status.
- **[No Echo restores full power]** → State this demonstration-specific policy prominently and retain the immobile, secured, bare-motor rig; do not characterize it as fail-safe behavior.
- **[Duplicated helper code can drift across standalone lessons]** → Keep helpers small, use parallel host tests, and compare copied lesson 06 mapping behavior during implementation review.

## Migration Plan

1. Add lessons 07–09 without modifying the behavior of existing lesson projects.
2. Build each project and run its documented host checks before updating navigation.
3. Validate student instructions against the planned wiring, timing, telemetry, and safe stationary demonstrations.
4. Update the lesson collection index, repository beginner route, and prerequisite/next-lesson links only after all three standalone projects exist.
5. Roll back by removing the three new lesson directories and their navigation entries; existing lessons remain independently usable.
