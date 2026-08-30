# PlatformIO Uno Lessons

This folder contains numbered, standalone Arduino Uno lessons.

Each lesson lives in its own folder (for example, `01-gpio-transistor-switch/`) and is a complete PlatformIO project that you can open, build, and upload independently.

Use the number prefix to keep lessons in learning order.

## Lessons

1. [`01-gpio-transistor-switch`](01-gpio-transistor-switch/) - Use digital pin 8 to switch a transistor and blink an LED with fixed delays.
2. [`02-adc-variable-delay`](02-adc-variable-delay/) - Read an adjustable voltage on A0 and use it to change the pin-8 blink speed.
3. [`03-pwm-motor-ramp`](03-pwm-motor-ramp/) - Use PWM on digital pin 3 to ramp an externally powered motor command smoothly up and down.
4. [`04-adc-pwm-motor-control`](04-adc-pwm-motor-control/) - Map the adjustable A0 voltage to motor PWM and observe the response of a blocking 500-millisecond control loop.
5. [`05-interrupt-driven-motor-control`](05-interrupt-driven-motor-control/) - Use timer interrupts to update ADC-controlled motor PWM every 100 milliseconds while the main loop reports values every 500 milliseconds.

## Learning Order and Prerequisites

- Before lesson 02, students should understand the lesson 01 transistor/LED circuit, shared ground, digital HIGH and LOW output on pin 8, and using `delay()` to control blink timing.
- Before lesson 03, students should complete lessons 01 and 02 so they recognize transistor switching, shared ground, numeric mapping, and the difference between Arduino logic power and an external circuit's power. Lesson 03 introduces PWM and a teacher-verified external MOSFET motor circuit.
- Before lesson 04, students should understand lesson 03's PWM values from 0 to 255. Lesson 04 reuses the lesson 02 RV voltage divider to control those PWM values and intentionally updates only once every 500 milliseconds.
- Before lesson 05, students should observe lesson 04's blocking response. Lesson 05 keeps the same motor and RV controls but uses a timer interrupt so motor updates can continue while the main program waits between serial reports.

The sequence moves from a program-controlled PWM ramp, to direct but blocking ADC control, and finally to interrupt-driven ADC control. Open only one numbered lesson folder at a time so PlatformIO builds that lesson independently.
