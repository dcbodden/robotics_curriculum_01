# Lesson 01: Hello, World

In this lesson, you will build and run a tiny C program on a Linux laptop. Its output is one greeting in the terminal:

```text
Hello, world!
```

## Learning Goals

By the end of this lesson, you will be able to:

- describe **source code** as program text a person can read and edit;
- **build** source code into a program the laptop can run;
- **run** that program from the terminal; and
- identify the greeting as the program's **output**.

## Before You Begin

You need a teacher-prepared Linux laptop with VS Code and the GCC C compiler. You do not need an Arduino or any electronic parts for this lesson.

Ask your teacher to confirm that GCC is ready. Installing programs or changing computer permissions is a teacher task.

## Read the Source Code

Open `main.c`. This is the source-code file:

```c
#include <stdio.h>

int main(void) {
    printf("Hello, world!\n");
    return 0;
}
```

The program starts following instructions inside `main`. The `printf` instruction produces text output. The letters between quotation marks are the message. The `\n` moves the terminal to a new line after the message. `return 0` tells the laptop that the program finished normally.

The first line makes the standard C input-and-output helper available so this program can use `printf`. You do not need to memorize that line or the punctuation in this lesson. Focus on the instruction and the output it produces.

## Predict

Before building, write or say your prediction:

**What exact message will appear when this program runs?**

Prediction: ________________________________________________

## Open the Lesson Folder and Terminal

1. In VS Code, choose **File > Open Folder...**.
2. Select the `coding-fundamentals-lessons/01-hello-world` folder. Open this lesson folder itself, not only the `main.c` file.
3. Choose **Terminal > New Terminal**.
4. Run this command to print the terminal's current folder:

   ```bash
   pwd
   ```

5. Check that the printed path ends with `coding-fundamentals-lessons/01-hello-world`.
6. Run this command to list the files in that folder:

   ```bash
   ls
   ```

You should see `main.c` and `README.md`. If you do not, stop and reopen the correct folder before building.

## Build the Program

Run this exact command:

```bash
gcc -std=c11 -Wall -Wextra -Werror main.c -o hello-world
```

GCC reads the source file named `main.c` and builds an executable program named `hello-world`. **Source code** and the **executable** are different files:

| File | What it is |
| --- | --- |
| `main.c` | Source code that you can read and edit |
| `hello-world` | The program built for the laptop to run |

A successful build usually prints no message and returns to the terminal prompt. Run `ls` again:

```bash
ls
```

You should now see both `main.c` and `hello-world`. Seeing the new file proves that GCC built a program. It does not yet prove what the program will print.

## Run and Observe

Run the program:

```bash
./hello-world
```

The `./` means “run the program named `hello-world` from this folder.” Compare the actual output with your prediction.

Actual output: ______________________________________________

Did the terminal place its next prompt on a new line? **Yes / No**

Complete this explanation:

The source file was ____________________. GCC built the executable named ____________________. The `printf` instruction produced the output ____________________.

## Optional Cleanup and Rebuild

The executable was generated from `main.c`, so it can be removed and built again. To remove only the generated executable, run:

```bash
rm hello-world
```

Run `ls` and confirm that `main.c` remains. Then repeat the GCC build command and run the program again. Never add `main.c` to the `rm` command.

## Troubleshooting

- **`pwd` ends with a different folder:** Use **File > Open Folder...** and select `01-hello-world`, then open a new terminal.
- **`ls` does not show `main.c`:** You are in the wrong folder. Do not type the build command until `main.c` appears.
- **The terminal says `gcc: command not found`:** Stop and ask the teacher to prepare GCC. This is not a source-code problem.
- **GCC prints an error:** Read the first error with your teacher. Compare `main.c` with the code shown above, including quotation marks and semicolons, then build again.
- **The build returns silently:** That is normal. Run `ls` and look for `hello-world`.
- **The terminal says `./hello-world: No such file or directory`:** The program was not built in this folder. Check `pwd` and `ls`, then run the GCC command again.
- **The words appear with `\n` printed at the end:** Inside the source code, check that the message contains one backslash followed by `n`, without an extra backslash.

When your observation and explanation are complete, continue to [lesson 02: Name Greeting](../02-name-greeting/).
