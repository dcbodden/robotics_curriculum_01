## Context

The repository has numbered standalone Uno lessons and an advanced `platformio-esp32-robotics-template/`, but it has no beginner-sized ESP32 collection. The new first lesson needs to verify the specific 30-pin ESP32-WROOM DevKit, PlatformIO upload path, serial connection, and controllable onboard LED without introducing an external circuit. See `proposal.md` for the motivation and the `esp32-lessons` delta for observable requirements.

## Goals / Non-Goals

**Goals:**

- Make the supported board and PlatformIO environment explicit and independently buildable.
- Keep the firmware small enough that students can connect each serial announcement to the following physical blink group.
- Establish repeatable directory, index, and lesson-document conventions for later ESP32 lessons.
- Separate build success from serial and physical observations in the verification record.

**Non-Goals:**

- Calculate Fibonacci values at runtime or teach the Fibonacci algorithm.
- Introduce nonblocking timing, tasks, interrupts, Wi-Fi, Bluetooth, or external circuits.
- Generalize lesson 01 across every ESP32 DevKit clone or board family.
- Insert the new ESP32 collection into the existing canonical beginner route.

## Decisions

### Use the DOIT-specific PlatformIO board definition

The lesson will use an `esp32doit-devkit-v1` environment with `platform = espressif32`, `board = esp32doit-devkit-v1`, `framework = arduino`, and `monitor_speed = 115200`. This matches the named 30-pin DevKit V1 more precisely than the generic `esp32dev` environment used by the advanced template and selects the Arduino variant that defines its built-in LED.

Alternative considered: reuse `esp32dev` for consistency with the advanced template. It is broader than the hardware named for this lesson and provides weaker evidence that the intended variant was selected.

### Address the onboard LED through `LED_BUILTIN`

The source will use `LED_BUILTIN` rather than a numeric pin constant. The selected Arduino board variant maps it to GPIO 2, keeping the code readable while tying the pin choice to the board definition. Setup will configure it as an output and start it LOW before any groups run.

Alternative considered: hard-code GPIO 2. That makes the wiring fact obvious but duplicates the board definition and makes a future hardware-specific adjustment less localized.

### Encode a fixed sequence rather than calculate Fibonacci values

The repeating Arduino `loop()` will make five explicit group calls with counts `1`, `1`, `2`, `3`, and `5`. A small group helper will print `Blinking Fibonacci N`, perform exactly `N` blocking blink cycles, and then apply the additional group pause. This exposes the intended sequence directly and keeps the lesson focused on successful board operation rather than arrays, recursion, or sequence-generation state.

Alternative considered: store the values in an array and iterate over it. That is concise but introduces a collection concept unrelated to the lesson's evidence goal. Calculating the sequence dynamically adds still more incidental complexity.

### Treat the group pause as additional to a complete blink cycle

Every blink will include 250 milliseconds HIGH and 250 milliseconds LOW. After all blinks in a group, the helper will add a 1,000-millisecond delay. Consequently, the time from the final falling edge to the next rising edge is 1,250 milliseconds; the README will describe this as the normal final off phase plus an additional one-second group gap.

Alternative considered: shorten or omit the final blink's off phase to make the falling-edge-to-rising-edge interval exactly one second. That gives the last blink different timing and complicates an otherwise uniform loop.

### Use two correlated observations as acceptance evidence

The lesson README will lead with a Core Mission: build and upload, observe the onboard LED, open the 115200-baud monitor, and match a displayed value with its blink count. Expected output will list both occurrences of Fibonacci `1`, avoiding the mistaken expectation that groups are ordinally numbered 1 through 5. Build verification and hardware observation will be recorded separately because a successful compilation cannot prove LED, upload, or serial behavior.

Alternative considered: add host-side tests for the five constants. That would add infrastructure without validating the actual LED and serial path that this lesson exists to demonstrate.

## Risks / Trade-offs

- [Some 30-pin clones use different board circuitry or omit a controllable GPIO 2 LED] → Name the supported DOIT variant, use `LED_BUILTIN`, distinguish the power LED, and provide a troubleshooting checkpoint for the exact board marking and GPIO 2 LED.
- [A student may miss early serial text while opening Monitor] → The sequence repeats forever, so the instructions can tell the student to wait for the next `1, 1, 2, 3, 5` cycle or press reset.
- [The two consecutive one-blink groups can look like one group] → Print before every group and retain the additional one-second pause, then ask the student to correlate each message with one group.
- [The observed dark interval is longer than the named group delay] → Explain that the final 250-millisecond off phase completes the blink before the additional 1,000-millisecond group gap begins.
- [Blocking delays are unsuitable for later responsive robotics work] → Present blocking behavior as intentional for this bring-up lesson and leave nonblocking or concurrent timing to later lessons.

## Migration Plan

This is an additive curriculum change. Add the collection and lesson, build the standalone project, perform available upload/serial/LED checks on the named board, and then expose it through the collection and repository indexes. If the change must be rolled back, remove the new collection and its root-guide entry; no existing lesson or template depends on it.
