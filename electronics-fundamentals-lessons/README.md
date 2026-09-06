# Electronics Fundamentals Lessons

This collection contains numbered, standalone, non-code electronics lessons for beginner robotics students.

## Convention

- Folder names use a numeric prefix to show lesson order, for example `01-lesson-name/`, `02-lesson-name/`, and so on.
- Each lesson is self-contained and can be taught independently.
- Lessons in this collection do not require building or uploading firmware.
- Student-facing instructions and recording activities live inside each lesson folder.

## Recommended Curriculum Sequence

Use these activities in this order:

1. Begin the curriculum with [Multimeter and Ohm's Law](01-multimeter-ohms-law/). It gives students hands-on experience with voltage, resistance, and current before they begin the Arduino lessons.
2. Teach [ADC Variable Delay](../platformio-uno-lessons/02-adc-variable-delay/) before manual voltage mapping. Students first see an Arduino turn a changing voltage into a digital value.
3. Teach [Manual Voltage Mapping](02-manual-voltage-mapping/) after ADC Variable Delay. Sorting measured voltages into eight levels gives students another way to practice the ADC idea and strengthen retention and comprehension.

Both fundamentals lessons are non-code activities. Students do not build or upload firmware for either one.

## Lessons

- [Multimeter and Ohm's Law](01-multimeter-ohms-law/)
- [Manual Voltage Mapping](02-manual-voltage-mapping/)

Use this collection for foundational electronics practice that supports coding lessons while remaining separate from PlatformIO project folders.
