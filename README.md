# robotics_curriculum_01
This project is an OpenSpec-driven exploratory robotics curriculum for beginners, which I am developing for my 10 year old child. Electronics, coding, and AI (from CV to LLMs) may be integrated.

## Repository layout

- `electronics-reference/` — general electronics reference materials such as Ohm's law cards, multimeter notes, and other foundational artifacts
- `experiments/` — standalone experiments such as Snap Circuits activities, breadboard diagrams, and lesson-specific builds
- `syllabus-and-teacher-materials/` — lesson plans, pedagogy notes, and teacher-training/reference material
- `platformio-uno-template/` — a VS Code-friendly PlatformIO Arduino Uno starter project for digital/physical integration lessons
- `platformio-esp32-robotics-template/` — an ESP32-WROOM-32 advanced robotics template organized around OV7670 camera work, DRV8833 motor control, an FPV Wi-Fi RC car project, and a follow-on semi-autonomous robotics project
- `openspec/` — lightweight OpenSpec planning artifacts for repository structure and future curriculum changes

## OpenSpec workflow

This repository keeps lightweight OpenSpec artifacts in `openspec/`:

- durable requirements in `openspec/specs/`
- in-flight or recorded changes in `openspec/changes/`

Use these documents to capture curriculum changes before implementation whenever the change is larger than a small edit.
