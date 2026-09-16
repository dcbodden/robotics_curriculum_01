## Context

See proposal.md for motivation. The current curriculum begins Arduino coding with a D8-controlled external Snap Circuits transistor/LED assembly. The repository has separate collections for non-code electronics fundamentals and PlatformIO Uno projects, but no workstation coding sequence. The development workstation is Linux and already supports a terminal, GCC-style tooling, VS Code, and PlatformIO.

## Goals / Non-Goals

**Goals:** Keep each program short enough to read as a complete idea; make build/run/upload differences observable; reuse a small vocabulary; and move from text output to input, repetition, and physical output with minimal setup.

**Non-Goals:** Portable support across operating systems, a general C course, C/C++ comparison, formal syntax coverage, assessment of spelling or punctuation, external Arduino wiring, reusable program architecture, or abstraction introduced only to make code look conventional.

## Decisions

### Keep all four lessons in one new collection

Create `coding-fundamentals-lessons/` with a collection README and four numbered directories. Lessons 01–03 each contain `main.c` and a README. Lesson 04 contains its own `platformio.ini`, `src/main.cpp`, and README so its location does not prevent it from being opened and built independently.

This makes the conceptual sequence visible in one index and avoids renumbering the established Uno series. Putting lesson 04 under `platformio-uno-lessons/` was considered, but it would either need a `00` prefix or shift six established lesson numbers. Separating the laptop and Arduino activities into different collections was rejected because it weakens the intended build/run-to-build/upload bridge.

### Use direct GCC commands on the prepared Linux workstation

Each laptop README shows the source directory, `gcc -std=c11 -Wall -Wextra -Werror main.c -o <program-name>`, and `./<program-name>`. It explains source and executable as two different files and tells the student to read the first compiler error with a teacher rather than teaching the compilation pipeline. Generated executable files are ignored by version control.

The timed lesson uses POSIX `sleep(1)` from `<unistd.h>` because the target workstation is Linux. Standard C has no portable one-second sleep operation, so the teacher notes that this wait belongs to the workstation environment. Busy-waiting and platform-detection code were rejected as misleading complexity.

### Accept a whole name safely while hiding incidental string mechanics

Use a fixed, bounded name buffer, `fgets()`, and removal of the trailing newline before formatting the greeting. This supports spaces and prevents unbounded input. The student-facing explanation treats the variable as a labeled place that remembers text; the buffer limit and newline cleanup receive short teacher annotations rather than becoming learning objectives. A single-word `%s` scan was rejected because it makes ordinary multiword names fail and encourages an unsafe pattern if written without a width.

Handle an empty line by asking once more or printing a simple prompt to enter at least one character, using bounded logic that does not turn the activity into input-validation coursework. End-of-input may exit with a short message. Exact handling can remain simple as long as the specified nonempty name behavior is met.

### Make the counter's timing visible

Lesson 03 uses an outer indefinite loop and an inner count from 1 through 5. It prints and flushes each line before sleeping so redirected or captured output remains visible promptly as well as in a terminal. Instructions show Ctrl+C as the intentional way to stop it. Verification uses a timeout or controlled termination after observing the first restart; an infinite program is not expected to exit normally.

### Bridge to Arduino with matching groups

Lesson 04 uses `LED_BUILTIN`, begins Serial at a documented rate, and implements groups 1–5 with two small loops: the outer loop chooses a group number and the inner loop produces that many on/off flashes. A longer group pause makes the boundary between groups visible. Serial prints the group number before its flashes, directly connecting laptop text output with physical output.

Use named millisecond constants and `delay()` because timing internals are outside this introduction. Explain the loops as “choose the group” and “make that many flashes”; naming nested loops is optional teacher vocabulary. Use only USB and the built-in LED, with power removed before swapping boards or cables. The existing external D8 lesson remains unchanged apart from prerequisite/discovery text.

### Keep activities observational and cumulative

Each README includes a small predict-observe-explain prompt: predict the text, try a different name, mark where the counter restarts, or predict the next blink group. Avoid worksheets or large quizzes for this short sequence. The collection index includes a compact vocabulary and a laptop-versus-Arduino comparison. Root and existing lesson links make the intended order discoverable.

## Risks / Trade-offs

- [Terminal commands create incidental difficulty] → Provide exact commands, expected output, directory checks, and concise “command not found,” wrong directory, compile error, and stop-program guidance.
- [C input mechanics distract from the concept] → Explain only the stored-name behavior to the student and put bounds/newline details in comments or teacher notes.
- [An infinite loop appears to freeze the terminal] → Predict repetition first and explicitly teach Ctrl+C before running lesson 03.
- [Two Arduino loops feel like a large jump] → Use matching names and a visual 1-to-5 group diagram, and relate the outer loop directly to lesson 03.
- [Built-in LED polarity or board definitions vary] → Target the specified Uno and `LED_BUILTIN`; verify the visible result on available hardware without external wiring.
- [The new collection duplicates existing lesson concepts] → Frame it as prior learning and link forward; preserve the richer circuit, measurement, and robotics purposes of existing lessons.

## Migration Plan

Add the new collection and lesson files, validate the laptop programs and Uno build, then update indexes and the existing lesson 01 prerequisite link. Existing numbered directories and behavior remain intact. Rollback removes the new collection and its discovery links without changing existing firmware.
