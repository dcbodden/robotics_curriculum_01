# Lesson 02: Name Greeting

Lesson 01 produced the same output every time. This program asks for **input**, remembers it, and uses it in a new greeting.

## Learning Goals

By the end of this lesson, you will be able to:

- identify a question from the program as output;
- enter text as input;
- describe a variable as a labeled place that stores a value; and
- find where `printf` combines fixed words with a stored name.

## Before You Begin

Complete [lesson 01: Hello, World](../01-hello-world/). Use the same teacher-prepared Linux laptop, VS Code, terminal, and GCC compiler. You do not need an Arduino.

## Read the Important Steps

Open `main.c`. The complete program has a few helper instructions that safely receive a line of text. Follow its four important actions:

```text
1. Make a labeled place named name.
2. Ask, "What is your name?"
3. Read and remember the keyboard input.
4. Print, "Nice to meet you," followed by the remembered name.
```

This line creates the labeled storage place:

```c
char name[50];
```

This line prints the final greeting:

```c
printf("Nice to meet you, %s!\n", name);
```

`%s` marks the place where `printf` puts the stored name. It is like a blank in the sentence. The program limits how much input it accepts and removes the Enter-key newline with helper instructions. You do not need to memorize those mechanics or the punctuation.

## Predict

Suppose you type `Ada Lovelace` and press Enter.

What greeting do you predict? _________________________________________

Which part comes from the program? ___________________________________

Which part comes from your input? ____________________________________

## Open, Build, and Run

1. In VS Code, choose **File > Open Folder...** and select `coding-fundamentals-lessons/02-name-greeting`.
2. Choose **Terminal > New Terminal**.
3. Check the current folder:

   ```bash
   pwd
   ```

   The path must end with `coding-fundamentals-lessons/02-name-greeting`.

4. List the files:

   ```bash
   ls
   ```

   Confirm that you see `main.c`.

5. Build the program with this exact command:

   ```bash
   gcc -std=c11 -Wall -Wextra -Werror main.c -o name-greeting
   ```

6. Run it:

   ```bash
   ./name-greeting
   ```

7. When the program asks `What is your name?`, type your name and press Enter. A name containing a space is okay.

Expected pattern:

```text
What is your name? Ada Lovelace
Nice to meet you, Ada Lovelace!
```

The letters you type normally appear after the question as you type them. They are keyboard input, not another message chosen by the program.

## Observe and Explain

Name entered: ______________________________________________

Greeting printed: ___________________________________________

Run the program again and enter a different nonempty name. Did the fixed words stay the same? **Yes / No**

What changed? _______________________________________________

Complete the sentence: The variable named `name` stored ____________________, and `printf` used that stored value as ____________________.

If you press Enter without typing a name, the program asks you to run it again and type at least one character. If input ends before it receives a name, it prints `No name was entered.` and stops. These results prevent the program from pretending that it received a name.

## Optional Cleanup

Remove only the generated executable with:

```bash
rm name-greeting
```

The source file `main.c` remains and can build the executable again.

## Troubleshooting

- **`main.c` does not appear after `ls`:** Reopen the `02-name-greeting` folder before building.
- **`gcc` is not found:** Ask the teacher to prepare the compiler.
- **GCC reports an error:** Read the first error with the teacher and compare `main.c` with the supplied source.
- **`./name-greeting` is missing:** Check that the build returned without an error and that `ls` shows `name-greeting`.
- **Only part of a very long name appears:** The storage has a safe size limit. Use a shorter name for this activity.
- **The program asks for at least one character:** Run it again and type a name before pressing Enter.

Next, continue to [lesson 03: Repeating Counter](../03-repeating-counter/).
