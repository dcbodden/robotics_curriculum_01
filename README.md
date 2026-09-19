# robotics_curriculum_01
This project is an OpenSpec-driven exploratory robotics curriculum for beginners, which I am developing for my 10 year old child. Electronics, coding, and AI (from CV to LLMs) may be integrated.

## Repository layout

- `electronics-reference/` — general electronics reference materials such as Ohm's law cards, multimeter notes, and other foundational artifacts
- [`coding-fundamentals-lessons/`](coding-fundamentals-lessons/) — four beginner lessons that move from small laptop C programs to counting with the Arduino Uno's built-in LED
- [`electronics-fundamentals-lessons/`](electronics-fundamentals-lessons/) — numbered, standalone, non-code lessons for hands-on electronics measurement and concepts
- `experiments/` — standalone experiments such as Snap Circuits activities, breadboard diagrams, and lesson-specific builds
- `syllabus-and-teacher-materials/` — lesson plans, pedagogy notes, and teacher-training/reference material
- `platformio-uno-template/` — a VS Code-friendly PlatformIO Arduino Uno starter project for digital/physical integration lessons
- [`platformio-uno-lessons/`](platformio-uno-lessons/) — numbered, independently buildable Arduino Uno projects, beginning with [`01-gpio-transistor-switch`](platformio-uno-lessons/01-gpio-transistor-switch/) for controlling and measuring a transistor-switched LED with digital pin 8
- [`platformio-esp32-lessons/`](platformio-esp32-lessons/) — numbered, independently buildable ESP32 DevKit projects, beginning with `01-hello-world` for verifying the onboard LED and Serial Monitor
- `platformio-esp32-robotics-template/` — an ESP32-WROOM-32 advanced robotics template organized around OV7670 camera work, DRV8833 motor control, an FPV Wi-Fi RC car project, and a follow-on semi-autonomous robotics project
- `openspec/` — lightweight OpenSpec planning artifacts for repository structure and future curriculum changes

## Recommended Beginner Sequence

Follow this primary route so each activity builds on ideas and equipment used earlier:

1. Start with [Multimeter and Ohm's Law](electronics-fundamentals-lessons/01-multimeter-ohms-law/) to practice safe measurement and recognize open and closed circuit paths.
2. Complete the four [Coding Fundamentals Lessons](coding-fundamentals-lessons/) to explore output, input, stored values, counters, loops, building, running, and uploading.
3. Use [Uno lesson 01: GPIO Transistor Switch](platformio-uno-lessons/01-gpio-transistor-switch/) to connect familiar Arduino output to voltage measurements and an external light circuit.
4. Continue with [Uno lesson 02: ADC Variable Delay](platformio-uno-lessons/02-adc-variable-delay/) to turn a changing voltage into a digital value and a visible change in blink speed.
5. Reinforce that ADC idea with [Manual Voltage Mapping](electronics-fundamentals-lessons/02-manual-voltage-mapping/) by sorting measured voltages into eight digital levels without building new firmware.
6. Introduce PWM and motor behavior with [Uno lesson 03: PWM Motor Ramp](platformio-uno-lessons/03-pwm-motor-ramp/).
7. Control motor PWM directly with [Uno lesson 04: ADC PWM Motor Control](platformio-uno-lessons/04-adc-pwm-motor-control/).
8. Compare slow and frequent control checks with [Uno lesson 05: Interrupt-Driven Motor Control](platformio-uno-lessons/05-interrupt-driven-motor-control/) and its robot-attention investigation.
9. Combine two joystick inputs with [Uno lesson 06: Joystick Motor and Servo Control](platformio-uno-lessons/06-joystick-motor-servo-control/).
10. Measure ultrasonic round-trip time and distance with [Uno lesson 07: Blocking Ultrasonic Distance](platformio-uno-lessons/07-ultrasonic-blocking-distance/).
11. Keep joystick control running while Echo edges are captured with [Uno lesson 08: Interrupt Ultrasonic and Joystick Control](platformio-uno-lessons/08-interrupt-ultrasonic-joystick-control/).
12. Use measured distance to modify an actuator command with [Uno lesson 09: Distance-Aware Motor Control](platformio-uno-lessons/09-distance-aware-motor-control/).

The lesson collections also contain optional references and reinforcement activities. Follow each lesson's stated prerequisites before taking an optional side path.

## OpenSpec workflow

This repository keeps lightweight OpenSpec artifacts in `openspec/`:

- durable requirements in `openspec/specs/`
- in-flight or recorded changes in `openspec/changes/`

Use these documents to capture curriculum changes before implementation whenever the change is larger than a small edit.
