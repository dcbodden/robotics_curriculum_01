# Coding Fundamentals Lessons

This collection is a short introduction to code for a beginner robotics student. You will make three small programs run on a Linux laptop. Then you will use the same ideas to make an Arduino Uno's built-in light blink.

These lessons focus on a few useful ideas. They are preparation for the robotics lessons, not a complete computer science course.

## What You Need

For lessons 01–03:

- a Linux laptop prepared by the teacher;
- a terminal;
- the GCC C compiler; and
- a text editor such as VS Code.

Lesson 04 also requires:

- an Arduino Uno;
- a USB data cable; and
- VS Code with PlatformIO ready to use.

Lesson 04 uses the light already built into the Uno. It needs no breadboard, external LED, jumper wires, Snap Circuits parts, or external power supply.

## Where This Collection Fits

On the primary beginner route, first complete [Multimeter and Ohm's Law](../electronics-fundamentals-lessons/01-multimeter-ohms-law/) for safe measurement practice. Then complete all four lessons in this collection before starting [Uno lesson 01: GPIO Transistor Switch](../platformio-uno-lessons/01-gpio-transistor-switch/).

The complete route is listed in the [repository guide](../README.md#recommended-beginner-sequence). Other activities may be used as optional review, but check their stated prerequisites before changing the order.

## Follow the Lessons in Order

1. [Hello, World](01-hello-world/) — Build and run a C program that sends a greeting to the terminal.
2. [Name Greeting](02-name-greeting/) — Give the program input and see it use the name it remembers in its output.
3. [Repeating Counter](03-repeating-counter/) — Use a counter and loops to print 1 through 5, one number per second, over and over.
4. [Arduino LED Counting](04-arduino-led-counting/) — Build and upload a program that flashes the Uno's built-in LED in groups of one through five.

Each lesson builds on the one before it:

```text
print a message
      ↓
receive and use a name
      ↓
count and repeat
      ↓
use counting and repetition to control a light
```

Complete lessons 01–03 before lesson 04. Before beginning lesson 04, the teacher should also confirm that PlatformIO can find the Uno and that the USB cable carries data. After lesson 04, continue to [Uno lesson 01: GPIO Transistor Switch](../platformio-uno-lessons/01-gpio-transistor-switch/), where familiar build, upload, loop, and output ideas begin controlling and measuring an external circuit.

## Words We Will Use

| Word | Meaning in these lessons |
| --- | --- |
| **Program** | A group of instructions for a computer or Arduino. |
| **Instruction** | One action we ask the machine to perform. |
| **Source code** | The program text that a person can read and edit. |
| **Build** | Turn source code into a form the machine can run. |
| **Run** | Start a program and let it follow its instructions. |
| **Input** | Information that goes into a program, such as a name typed on the keyboard. |
| **Output** | Information or an action produced by a program, such as terminal words or a blinking light. |
| **Stored value** | Information the program remembers in a labeled place. |
| **Counter** | A stored number that changes as a program counts. |
| **Loop** | Instructions that repeat. |
| **Upload** | Send a built program from the laptop to an Arduino. |

You do not need to memorize all these words before starting. Each lesson introduces them when they become useful.

## Laptop Programs and Arduino Programs

Both kinds begin as source code and must be built. The important difference is where the built program runs.

| Laptop C program | Arduino program |
| --- | --- |
| GCC builds it on the laptop. | PlatformIO builds it on the laptop. |
| You run it from the laptop terminal. | You upload it through the USB cable. |
| It runs on the laptop. | It runs on the Uno. |
| Its output can be words in the terminal. | Its output can include a physical light. |
| You stop it from the laptop. | It keeps running on the Uno while the board has power. |

Building checks whether the source code can become a program. Running or uploading lets you observe what the program actually does. A successful build does not by itself show what the running program will output.

## How Each Lesson Works

The lessons use the same six steps:

```text
READ → PREDICT → BUILD → RUN OR UPLOAD → OBSERVE → EXPLAIN
```

First read the short program and predict what it will do. Next build it. Run a laptop program or upload an Arduino program, then record what actually happens and explain which instruction, input, counter, or loop helped produce it. A prediction can be different from the result; the observation is still useful evidence.

Laptop lessons show every terminal command in a box. Run one command at a time from that lesson's folder. Do not change a command unless the lesson asks you to. Your teacher will help prepare GCC and fix computer permissions. The [teacher guide](teacher-guide.md) explains the shared lesson routine and preparation.
