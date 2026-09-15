# Lesson 03: Repeating Counter

This laptop program counts from 1 through 5, with about one second between numbers. Then it starts at 1 and repeats until you stop it.

## Learning Goals

By the end of this lesson, you will be able to:

- describe `count` as a counter whose stored number changes;
- identify a loop that counts from 1 through 5;
- identify a loop that keeps the whole sequence repeating;
- connect `sleep(1)` with the visible pause; and
- stop a repeating terminal program with Ctrl+C.

## Before You Begin

Complete [lesson 02: Name Greeting](../02-name-greeting/). Use the same teacher-prepared Linux laptop, VS Code, terminal, and GCC compiler. You do not need an Arduino.

Before running this lesson, find the **Ctrl** and **C** keys. Pressing Ctrl+C will ask the repeating program to stop and return control to the terminal.

## Read the Source Code

Open `main.c`:

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
    while (1) {
        for (int count = 1; count <= 5; count++) {
            printf("%d\n", count);
            fflush(stdout);
            sleep(1);
        }
    }
}
```

The `for` loop gives `count` the values 1, 2, 3, 4, and 5. Each time through that loop, `printf` outputs the current value and `sleep(1)` waits about one second. `fflush` makes sure the number is sent to the terminal before the wait; treat it as a helper instruction.

The `while (1)` loop repeats everything inside it. After the `for` loop reaches 5, the outside loop starts the counting sequence at 1 again.

```text
outside loop: repeat forever
    inside loop: count 1 → 2 → 3 → 4 → 5
    return to the start of the outside loop
```

Teacher note: `sleep()` is supplied by the Linux/POSIX environment. It is not a portable ISO C timing instruction. This lesson intentionally targets the prepared Linux workstation.

## Predict

Write the first seven numbers you predict the program will print:

____  ____  ____  ____  ____  ____  ____

Circle the place where you think the sequence will restart.

What must you do to stop the program? _________________________________

## Open and Build

1. In VS Code, choose **File > Open Folder...** and select `coding-fundamentals-lessons/03-repeating-counter`.
2. Choose **Terminal > New Terminal**.
3. Check the folder:

   ```bash
   pwd
   ```

   The path must end with `coding-fundamentals-lessons/03-repeating-counter`.

4. List the files and confirm `main.c` appears:

   ```bash
   ls
   ```

5. Build with this exact command:

   ```bash
   gcc -std=c11 -Wall -Wextra -Werror main.c -o repeating-counter
   ```

## Run, Observe, and Stop

Run the program:

```bash
./repeating-counter
```

Watch until it prints at least six numbers. Check that 1 appears after the first 5. Then hold **Ctrl** and press **C** once. You may see `^C`; the terminal prompt should return.

Write the first six numbers you observed:

____  ____  ____  ____  ____  ____

About how long was the pause between numbers? _________________________

Did 1 appear again after 5? **Yes / No**

Complete the explanations:

- The stored value named `count` changed from ____ through ____.
- The `for` loop ________________________________________________.
- The `while` loop ______________________________________________.
- Ctrl+C ________________________________________________________.

The program did not freeze while it counted. Repeating forever was one of its instructions. Ctrl+C is the planned way to stop this terminal activity.

## Optional Cleanup

Remove only the generated executable with:

```bash
rm repeating-counter
```

Do not include `main.c` in the cleanup command.

## Troubleshooting

- **`main.c` does not appear after `ls`:** Reopen the `03-repeating-counter` folder.
- **GCC is missing or reports an error:** Ask the teacher to check the workstation or read the first compiler error with you.
- **The executable cannot be found:** Confirm that `ls` shows `repeating-counter`, then include `./` when running it.
- **All numbers seem to appear together:** Confirm that the supplied code contains `fflush(stdout);` before `sleep(1);`, rebuild, and try again.
- **The prompt does not return by itself:** That is expected. Press Ctrl+C once.
- **You typed the letter C but the program continued:** Hold the Ctrl key while pressing C.

Next, use the same counting idea to control a physical light in [lesson 04: Arduino LED Counting](../04-arduino-led-counting/).
