# Lesson 01: ESP32 Hello World

In this lesson, you will upload a small program to an ESP32 and observe its output in two places: the controllable onboard LED and the Serial Monitor. The repeating blink groups provide clear evidence that the program is running on the board.

## Learning Goals

By the end of the lesson, you will be able to:

- identify the supported ESP32 development board;
- build and upload a standalone PlatformIO project;
- observe program-controlled output on the onboard LED;
- open the Serial Monitor at the project's configured speed; and
- use visible and text output as evidence that the uploaded program is running.

## Supported Board

This project targets a **30-pin DOIT ESP32 DevKit V1** carrying an **ESP32-WROOM** module. PlatformIO identifies it as `esp32doit-devkit-v1`.

The program uses the board definition's `LED_BUILTIN` output. On the supported board, that controllable onboard LED is connected to GPIO 2.

## What You Need

- a 30-pin DOIT ESP32 DevKit V1 with an ESP32-WROOM module;
- a USB **data** cable that fits the board; and
- a computer with VS Code and the PlatformIO extension ready.

This is a USB-only lesson. Do not connect a breadboard, jumper wires, an external LED, or an external power supply.

## Core Mission: Confirm That the ESP32 Is Running Your Program

**Goal:** Build and upload the supplied project, observe its onboard LED, and open the Serial Monitor to see the program's text output.

1. Make sure no external circuit or power supply is connected to the ESP32.
2. In VS Code, choose **File > Open Folder...** and open `platformio-esp32-lessons/01-hello-world` by itself. Confirm that Explorer shows `platformio.ini` and `src/main.cpp`.
3. Connect the ESP32 directly to the computer with the USB data cable. If VS Code asks whether you trust the folder, ask your teacher before continuing.
4. Select the PlatformIO alien-head icon. Under **Project Tasks > esp32doit-devkit-v1 > General**, select **Build**. Wait for the terminal to report `SUCCESS`.
5. Select **Upload** and wait for `SUCCESS`. After the upload, the ESP32 restarts and runs the program automatically.
6. Watch the controllable onboard LED. It should blink in groups separated by a longer dark pause.
7. Under **Project Tasks > esp32doit-devkit-v1 > General**, select **Monitor**. This project uses **115200 baud**. Watch the text and the onboard LED together.

**Observe:** Did the onboard LED blink in groups? Did new text appear in the Serial Monitor before each group?

LED groups observed: **Yes / No**

Serial messages observed: **Yes / No**

**Stop safely:** Close the Serial Monitor with its close-terminal or trash-can button. When you are finished, unplug the USB cable by holding its connector.
