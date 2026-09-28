# PlatformIO ESP32 Lessons

This folder contains numbered, standalone ESP32 lessons. Each lesson lives in its own folder and is a complete PlatformIO project that you can open, build, upload, and monitor independently.

Open only one numbered lesson folder at a time so PlatformIO builds that lesson without compiling another lesson or the advanced ESP32 robotics template.

## Supported Board

These lessons begin with the 30-pin DOIT ESP32 DevKit V1 carrying an ESP32-WROOM module. Use a USB data cable so the computer can both upload programs and receive Serial Monitor messages.

Individual lessons identify any additional hardware they require. Lesson 01 uses only the board, its controllable onboard LED, and the USB data cable.

## Lessons

1. [`01-hello-world`](01-hello-world/) - Verify the ESP32, upload path, onboard LED, and Serial Monitor by repeating blink groups for the Fibonacci sequence 1, 1, 2, 3, 5.
2. [`02-bluetooth-motor-servo-control`](02-bluetooth-motor-servo-control/) - Use a PS4-compatible Bluetooth Classic controller to command one servo and one forward/reverse geared motor through an HW-627 DRV8833 on a current-limited bench setup. This lesson verifies wireless actuator control; batteries, DC/DC conversion, wheels, chassis work, steering linkage, and complete rover integration remain later work.

The separate [`platformio-esp32-robotics-template`](../platformio-esp32-robotics-template/) contains advanced starter projects for camera, motor-control, and Wi-Fi robotics work.
