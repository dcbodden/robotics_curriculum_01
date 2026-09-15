# Electronics Fundamentals Lessons

This collection contains numbered, standalone, non-code electronics lessons for beginner robotics students.

## Convention

- Folder names use a numeric prefix to show lesson order, for example `01-lesson-name/`, `02-lesson-name/`, and so on.
- Each lesson is self-contained and can be taught independently.
- Lessons in this collection do not require building or uploading firmware.
- Student-facing instructions and recording activities live inside each lesson folder.

## Recommended Curriculum Sequence

These two activities appear at different points on the primary beginner route:

1. Begin with [Multimeter and Ohm's Law](01-multimeter-ohms-law/). It gives students hands-on experience with voltage, resistance, current, and safe meter setup.
2. Complete the four [Coding Fundamentals Lessons](../coding-fundamentals-lessons/).
3. Complete [Uno lesson 01: GPIO Transistor Switch](../platformio-uno-lessons/01-gpio-transistor-switch/).
4. Complete [Uno lesson 02: ADC Variable Delay](../platformio-uno-lessons/02-adc-variable-delay/) so the student first sees an Arduino turn changing voltage into a digital value.
5. Return to this collection for [Manual Voltage Mapping](02-manual-voltage-mapping/). Sorting measured voltages into eight levels reinforces the ADC idea.
6. Continue with [Uno lessons 03–06](../platformio-uno-lessons/#lessons) for PWM, motor control, responsive checking, and coordinated joystick control.

The [repository guide](../README.md#recommended-beginner-sequence) shows the complete route with every lesson linked.

Both fundamentals lessons are non-code activities. Students do not build or upload firmware for either one. They can be revisited as optional review after their place in the primary route, but later activities still keep their own prerequisites.

## Lessons

- [Multimeter and Ohm's Law](01-multimeter-ohms-law/)
- [Manual Voltage Mapping](02-manual-voltage-mapping/)

Ownership and reuse permission for lesson photographs and illustrations are recorded in [Asset Provenance](ASSETS.md).

Use this collection for foundational electronics practice that supports coding lessons while remaining separate from PlatformIO project folders.
