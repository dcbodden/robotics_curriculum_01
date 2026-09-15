# PlatformIO Uno Lessons

This folder contains numbered, standalone Arduino Uno lessons.

Each lesson lives in its own folder (for example, `01-gpio-transistor-switch/`) and is a complete PlatformIO project that you can open, build, and upload independently.

Use the number prefix to keep lessons in learning order.

## Where These Lessons Fit

The primary beginner route starts with [Multimeter and Ohm's Law](../electronics-fundamentals-lessons/01-multimeter-ohms-law/), then all four [Coding Fundamentals Lessons](../coding-fundamentals-lessons/). Their final [Arduino LED Counting](../coding-fundamentals-lessons/04-arduino-led-counting/) activity practices building, uploading, loops, and output before lesson 01 adds an external circuit.

Complete Uno lessons 01 and 02 in order. Then use [Manual Voltage Mapping](../electronics-fundamentals-lessons/02-manual-voltage-mapping/) to reinforce the ADC idea before continuing with Uno lessons 03–06. The [repository guide](../README.md#recommended-beginner-sequence) shows the complete route.

## Lessons

1. [`01-gpio-transistor-switch`](01-gpio-transistor-switch/) - Use digital pin 8 to switch a transistor and blink an LED with fixed delays.
2. [`02-adc-variable-delay`](02-adc-variable-delay/) - Read an adjustable voltage on A0 and use it to change the pin-8 blink speed.
3. [`03-pwm-motor-ramp`](03-pwm-motor-ramp/) - Use PWM on digital pin 3 to ramp an externally powered motor command smoothly up and down.
4. [`04-adc-pwm-motor-control`](04-adc-pwm-motor-control/) - Map the adjustable A0 voltage to motor PWM and observe the response of a blocking 1,000-millisecond control loop.
5. [`05-interrupt-driven-motor-control`](05-interrupt-driven-motor-control/) - Use timer interrupts to update ADC-controlled motor PWM every 100 milliseconds while the main loop reports values every 1,000 milliseconds.
6. [`06-joystick-motor-servo-control`](06-joystick-motor-servo-control/) - Check two joystick axes every 20 milliseconds to coordinate center-off motor power and servo position. Use its [teacher guide](06-joystick-motor-servo-control/teacher-guide.md) to prepare regulated external 5 V actuator power and verify the actual hardware.

## Robot Attention Investigation: Lessons 04 and 05

Use the same B1 battery, RV control, and motor circuit to investigate how checking frequency affects response. The three-page activity includes a hypothesis, paired trials, and a conclusion supported by observations.

- [Printable student worksheet (PDF)](05-interrupt-driven-motor-control/robot-attention-worksheet.pdf)
- [Student worksheet source (HTML)](05-interrupt-driven-motor-control/robot-attention-worksheet.html)
- [Teacher guide](05-interrupt-driven-motor-control/robot-attention-teacher-guide.md)

## Learning Order and Prerequisites

- Before lesson 01, complete [Multimeter and Ohm's Law](../electronics-fundamentals-lessons/01-multimeter-ohms-law/) and the four [Coding Fundamentals Lessons](../coding-fundamentals-lessons/). Students bring safe measurement practice and familiar build, upload, counter, loop, and output ideas to the external pin-8 circuit.
- Before lesson 02, students should understand the lesson 01 transistor/LED circuit, shared ground, digital HIGH and LOW output on pin 8, and using `delay()` to control blink timing.
- Before lesson 03, students should complete lessons 01 and 02, then use [Manual Voltage Mapping](../electronics-fundamentals-lessons/02-manual-voltage-mapping/) to reinforce how changing voltage becomes a digital value. Lesson 03 builds on transistor switching, shared ground, numeric mapping, and the difference between Arduino logic power and an external circuit's power while introducing PWM and a teacher-verified external MOSFET motor circuit.
- Before lesson 04, students should understand lesson 03's PWM values from 0 to 255. Lesson 04 reuses the lesson 02 RV voltage divider to control those PWM values and intentionally updates only once every 1,000 milliseconds.
- Before lesson 05, students should observe lesson 04's blocking response. Lesson 05 keeps the same motor and RV controls but uses a timer interrupt so motor updates can continue while the main program waits between serial reports.
- Before lesson 06, students should understand lesson 05's frequent checks during foreground waits. Lesson 06 introduces a two-axis joystick, bounded servo position commands, and 50 checks per second. The teacher verifies its new regulated 5 V actuator supply, shared ground, and travel limits before use.

The sequence moves from a program-controlled PWM ramp, to direct but blocking ADC control, to interrupt-driven ADC control, and then to coordinated motor and servo commands. Open only one numbered lesson folder at a time so PlatformIO builds that lesson independently.
