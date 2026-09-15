## Why

The curriculum begins its code sequence with an Arduino, an external transistor circuit, and PlatformIO, so a ten-year-old learner must absorb programming and physical-computing concepts at the same time. A short introductory sequence can establish output, input, variables, counters, loops, building, running, and uploading without expanding into a general computer-science curriculum.

## What Changes

- Add `coding-fundamentals-lessons/` as a numbered four-lesson collection for beginner code activities.
- Add three laptop lessons using small C programs built with GCC on the Linux workstation: print “Hello, world!”, ask for and use a name, and repeatedly count from 1 through 5 with one-second spacing.
- Add a fourth, self-contained PlatformIO Arduino Uno lesson that applies counters and loops by flashing the built-in LED in groups of one through five, then restarting.
- Explain the progression from source code to building and running a laptop program, then from building to uploading an Arduino program.
- Link the new collection from the repository guide and position it before the current external-circuit Arduino lessons.
- Keep the scope focused on observable input, output, stored values, counters, and loops rather than data structures, algorithms, memory internals, or advanced C/C++ concepts.

## Capabilities

### New Capabilities

- `coding-fundamentals-lessons`: Define the four standalone, fifth-grade-friendly lessons and their conceptual progression from laptop text programs to a built-in Arduino LED.

### Modified Capabilities

- `repository-structure`: Add the numbered coding-fundamentals collection to the documented top-level curriculum layout.
- `arduino-uno-lessons`: Position the existing first external-circuit Uno lesson after the built-in LED coding lesson as recommended prior learning.

## Impact

The change adds a new top-level lesson collection containing C source, student instructions, and one independent PlatformIO Uno project. It updates the root guide, the new collection index, and discovery/prerequisite links around the existing Uno sequence. Laptop programs target the repository’s Linux workstation and use GCC; the timed counter uses the POSIX `sleep()` function. The Arduino activity uses only the Uno’s built-in LED and USB connection, so it adds no external circuit or supply requirements and does not renumber or change existing lessons.
