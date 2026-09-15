# Coding Fundamentals Teacher Guide

These four lessons prepare a beginner to read and run the small programs used later in the robotics curriculum. Keep the focus on observable behavior: information goes in, instructions run, output comes out, stored values can change, and loops repeat work.

## Teaching Scope

Use these words when they become useful: **program**, **instruction**, **source code**, **build**, **run**, **input**, **output**, **stored value**, **counter**, **loop**, and **upload**. The [collection index](README.md#words-we-will-use) gives the student definitions.

The student does not need an explanation of pointers, memory addresses, manual memory management, data structures, algorithms, compiler phases, command-line arguments, Arduino timer registers, or differences between the C and C++ language standards. Some of these ideas are involved behind the scenes. Treat them as teacher notes or small code helpers rather than additional lesson objectives.

Do not require the student to memorize punctuation. Ask what an instruction does before asking how it is spelled. Reading, predicting, building, running, and observing one short complete program are more valuable here than writing a similar program from memory.

## Prepare the Workstation

Before lesson 01:

1. Confirm the laptop runs Linux and VS Code can open a folder.
2. Open a terminal and run `gcc --version`. Confirm GCC is available; installation is a teacher or system-administrator task.
3. Confirm the student can open VS Code's integrated terminal with **Terminal > New Terminal**.
4. Explain that commands run in the folder named at the terminal prompt. The student can run `pwd` to display the current folder and `ls` to list its files.
5. Keep lesson 04's Arduino Uno, USB data cable, PlatformIO extension, and upload permissions ready, but leave the Uno disconnected during lessons 01–03.

Each laptop lesson gives the exact GCC command for its own source and executable name. Have the student type or paste that complete command. Do not replace it with an unexplained build system, shell script, alias, or global executable. A successful GCC command normally returns to the prompt without printing a success message; confirm that the named executable appears with `ls`.

Generated executables are deliberately placed beside their source files so the source/executable distinction can be seen. The collection `.gitignore` excludes only these three expected files:

| Lesson | Source retained in the repository | Generated executable ignored by Git |
| --- | --- | --- |
| 01 | `01-hello-world/main.c` | `01-hello-world/hello-world` |
| 02 | `02-name-greeting/main.c` | `02-name-greeting/name-greeting` |
| 03 | `03-repeating-counter/main.c` | `03-repeating-counter/repeating-counter` |

The student may remove a generated executable with the lesson's exact `rm` command, then build it again. Never instruct the student to use a wildcard with `rm`. Source code is not a cleanup file. PlatformIO's generated `.pio/` and `.vscode/` directories under lesson 04 are also ignored.

## Use the Same Learning Rhythm

Each lesson should follow this short cycle:

```text
READ → PREDICT → BUILD → RUN OR UPLOAD → OBSERVE → EXPLAIN
```

- **Read:** Identify the few instructions that matter to this lesson.
- **Predict:** Write or say what output will appear before running it.
- **Build:** Turn source code into a program and distinguish build success from runtime behavior.
- **Run or upload:** Start a laptop program locally, or send an Arduino program to the Uno.
- **Observe:** Record what actually appeared or happened, even when it differs from the prediction.
- **Explain:** Connect the observation to input, output, a counter, or a loop in one or two sentences.

Accept a prediction that turns out to be wrong. The purpose is to compare an idea with evidence, not to reward guessing the expected result. Prompts should allow short answers and concrete observations. Do not turn these activities into long worksheets or grade spelling and punctuation as coding concepts.

## Terminal Command Conventions

The lesson directions should show one command per code block and tell the student what it does immediately before using it. Use these forms consistently:

```bash
pwd
```

This prints the current folder. The final folder name must match the lesson being built.

```bash
ls
```

This lists the lesson files. The student should see `main.c` before building.

```bash
gcc -std=c11 -Wall -Wextra -Werror main.c -o exact-program-name
```

Each lesson replaces `exact-program-name` in its displayed command with its actual executable name. The flags ask GCC to use the planned C version, report useful warnings, and treat warnings as problems to fix rather than ignore.

```bash
./exact-program-name
```

Each lesson again displays its actual name. The `./` says to run the program from the current folder.

```bash
rm exact-program-name
```

This optional cleanup removes only the generated executable named by that lesson. It does not remove `main.c`.

If a command fails, read the first useful error together. Check the folder with `pwd` and `ls` before changing source code. If `gcc` is missing or access is denied, stop and fix the workstation setup as the teacher rather than adding installation or permission work to the student's programming lesson.

Lesson 03 intentionally continues until Ctrl+C. Introduce Ctrl+C before running it and explain that this requests the terminal program to stop; it is not the letter C being entered into the program. Its one-second `sleep()` call is provided by the Linux/POSIX environment, not by portable ISO C. Keep that distinction in a teacher note unless the student asks.

For lesson 04, use the PlatformIO buttons and terminology already established in the repository: **Build** checks the project, **Upload** sends it through USB, and **Monitor** displays serial text. Do not suggest that clicking Run executes the Arduino program on the laptop. Keep the activity USB-only and use `LED_BUILTIN`; no external circuit should be present.

## Teacher Verification Records

Build and runtime verification happen in later implementation tasks. Record them separately:

| Check | Build result | Runtime or hardware observation |
| --- | --- | --- |
| Lesson 01 exact greeting | | |
| Lesson 02 multiword name | | |
| Lesson 03 first restart at 1 | | |
| Lesson 04 Uno build | | |
| Lesson 04 upload, serial groups, and built-in LED | | |

A passing build does not prove runtime output, timing, an upload, or physical blinking. Mark unavailable hardware checks **not performed** instead of filling them from expected behavior.
