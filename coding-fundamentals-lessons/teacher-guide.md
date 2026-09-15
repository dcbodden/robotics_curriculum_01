# Coding Fundamentals Teacher Guide

These four lessons prepare a beginner to read and run the small programs used later in the robotics curriculum. The lesson pattern in this guide also applies to the later beginner electronics and Arduino lessons. Keep the focus on observable behavior: information goes in, instructions run, output comes out, stored values can change, and loops repeat work.

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

## Begin with a Core Mission

Put a short **Core Mission** before concept explanations, optional challenges, and detailed reference material. It gives the student a clear path to one observable success. A student should be able to complete it without reading the rest of the lesson first.

Use this structure:

1. **Goal:** Name the one result the student will produce or observe.
2. **Prepare and stay safe:** Include only the setup and safety directions required for this mission.
3. **Do:** Give the minimum numbered actions needed to reach the result.
4. **Notice:** Ask one short question about what actually happened.
5. **Stop safely:** State the next safe stopping point before an optional change, circuit change, or deeper explanation.

After the Core Mission, place the lesson's main concept explanation, a bounded **Try One Change** or comparison, common troubleshooting, and optional deeper detail under clear headings. There is no fixed word or page limit. The test is whether the required path is easy for a beginner to find and follow.

Safety does not become optional or move out of the Core Mission. A hardware mission must repeat every power-removal step, teacher checkpoint, clear-motion-area direction, and emergency stop instruction needed at that stage. Keep these directions beside the action where the student needs them, even when a teacher guide or earlier lesson also gives them.

## Use a Concrete-First Learning Cycle

For the first encounter with a concept, use this cycle:

```text
MAKE IT WORK → NOTICE → NAME → CHANGE OR COMPARE → PREDICT → TRY → EXPLAIN
```

- **Make it work:** Build, run, upload, or safely assemble the supplied baseline.
- **Notice:** Record one visible, audible, measured, or printed result.
- **Name:** Give the observed idea a useful name such as input, output, counter, loop, voltage, or PWM.
- **Change or compare:** Choose one bounded difference that makes the idea easier to see.
- **Predict:** Say or write what the student expects that one difference to do.
- **Try:** Make the safe change or run the comparison.
- **Explain:** Compare the prediction with the result in one or two sentences.

The first prediction comes after the student has observed a working example. In a later lesson, the student may predict before the first run when the relevant idea is already familiar. Accept a prediction that turns out to be wrong; the purpose is to compare an idea with evidence. Prompts should allow short answers and concrete observations. Do not grade spelling or punctuation as though they were the lesson concept.

## Use Guided Release When Helpful

The teacher may divide a new or difficult action into three passes:

- **I show:** The teacher demonstrates one complete safe example while naming the important action.
- **We do:** The teacher and student complete the action together, with the student making choices and observations.
- **You try:** The student completes the same kind of action or one bounded variation and explains the result.

This release pattern is optional. Skip a demonstration or shared pass when the student is ready to proceed independently. Add support when tools, wiring, or an unfamiliar command would otherwise hide the lesson's main idea.

## Separate Focus from Helper and Reference Material

Tell the student which small part is the **focus** of the lesson. Focus material is what the student should observe, name, change or compare, and explain. Keep assessment questions on that stated focus.

Label supporting material as **Helper Code**, **Teacher Note**, **Troubleshooting**, or **Deeper Reference**. Examples include safe input bounds, end-of-input handling, newline cleanup, compiler or upload details, Arduino library setup, timer registers, atomic snapshots, servo pulse machinery, component selection, supply sizing, and uncommon failure diagnosis. A student may inspect or ask about this material, but does not need to explain or memorize it to complete the lesson.

Keep correct safety and helper code in the real program. Do not shorten it into unsafe code or hide it behind extra unexplained files merely to make the source look smaller. Instead, mark the few lines or actions that form the current learning surface and explain what the remaining helper work provides.

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

## Teacher Verification Record

The following checks were performed on September 14, 2026. Build and runtime results are recorded separately:

| Check | Build result | Runtime or hardware observation |
| --- | --- | --- |
| Lesson 01 exact greeting | Passed with the documented warnings-as-errors GCC command. | Observed exactly `Hello, world!` followed by a newline. |
| Lesson 02 multiword name | Passed with the documented warnings-as-errors GCC command. | Entered `Ada Lovelace` and observed `Nice to meet you, Ada Lovelace!`. |
| Lesson 03 first restart at 1 | Passed with the documented warnings-as-errors GCC command. | Observed `1, 2, 3, 4, 5, 1, 2` at about one-second intervals, then stopped it with a 6.2-second timeout. |
| Lesson 04 Uno build | Passed for the PlatformIO `uno` environment. | A build does not test the board's physical behavior. |
| Lesson 04 upload, Serial groups, and built-in LED | Not applicable to the build check. | **Not performed:** no hardware upload, Serial Monitor observation, or physical LED observation was performed during this verification. |

A passing build does not prove runtime output, timing, an upload, or physical blinking. Mark unavailable hardware checks **not performed** instead of filling them from expected behavior.
