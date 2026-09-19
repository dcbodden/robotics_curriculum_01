## 1. Establish the ESP32 Lesson Collection

- [x] 1.1 Create `platformio-esp32-lessons/README.md` as the collection index, identify the projects as standalone, and list `01-hello-world` as the first lesson for the supported 30-pin ESP32-WROOM DevKit.
- [ ] 1.2 Update the root repository guide to include `platformio-esp32-lessons/` in the documented layout without adding it to the existing canonical beginner route.

## 2. Build the Hello World Project

- [ ] 2.1 Add `platformio-esp32-lessons/01-hello-world/platformio.ini` with an `esp32doit-devkit-v1` Arduino environment and a 115200-baud Serial Monitor.
- [ ] 2.2 Add the lesson firmware using `LED_BUILTIN`, a blocking blink-group helper, and five explicit group calls for `1, 1, 2, 3, 5`; print `Blinking Fibonacci N` before each group, use 250-millisecond on/off phases, add a 1,000-millisecond group pause, and repeat forever.

## 3. Write the Student Lesson

- [ ] 3.1 Create the lesson README with learning goals, supported-board identification, USB-only materials, and a concise Core Mission covering Build, Upload, initial LED observation, and opening Monitor at 115200 baud.
- [ ] 3.2 Document the expected five-message and five-blink sequence, guide the student to match a serial value to its blink count, and explain the repeated one groups and the additional group pause without turning runtime Fibonacci calculation into a learning requirement.
- [ ] 3.3 Add focused source explanation and troubleshooting for data cables, upload/reset or BOOT behavior, serial speed, missed initial output, the controllable LED versus the power LED, and clones without a controllable GPIO 2 LED.

## 4. Verify the Lesson

- [ ] 4.1 Build `platformio-esp32-lessons/01-hello-world` with PlatformIO and resolve all compiler or configuration errors.
- [ ] 4.2 Review the project and documentation against the specification, including the exact serial spelling and capitalization, sequence, timings, standalone folder structure, and absence of external-circuit requirements.
- [ ] 4.3 Upload to the supported 30-pin ESP32-WROOM DevKit and record whether the 115200-baud messages, onboard LED groups, group boundaries, and restart after five blinks were physically observed; leave the hardware result explicitly unverified until that test is performed.
