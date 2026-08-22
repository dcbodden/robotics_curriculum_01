# PlatformIO Uno Lessons

This folder contains numbered, standalone Arduino Uno lessons.

Each lesson lives in its own folder (for example, `01-gpio-transistor-switch/`) and is a complete PlatformIO project that you can open, build, and upload independently.

Use the number prefix to keep lessons in learning order.

## Lessons

1. [`01-gpio-transistor-switch`](01-gpio-transistor-switch/) - Use digital pin 8 to switch a transistor and blink an LED with fixed delays.
2. [`02-adc-variable-delay`](02-adc-variable-delay/) - Read an adjustable voltage on A0 and use it to change the pin-8 blink speed.

Before lesson 02, students should understand the lesson 01 transistor/LED circuit, shared ground, digital HIGH and LOW output on pin 8, and using `delay()` to control blink timing.
