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

### Match the Fibonacci Messages and Blink Groups

Wait for a complete five-group cycle. Each message appears at the start of its group, and its number tells you how many blinks to count before the longer dark pause.

| Group order | Serial Monitor message | Expected blinks | Blinks you counted |
| --- | --- | ---: | ---: |
| 1 | `Blinking Fibonacci 1` | 1 | ____ |
| 2 | `Blinking Fibonacci 1` | 1 | ____ |
| 3 | `Blinking Fibonacci 2` | 2 | ____ |
| 4 | `Blinking Fibonacci 3` | 3 | ____ |
| 5 | `Blinking Fibonacci 5` | 5 | ____ |

The first two groups both contain one blink because the five visible Fibonacci values in this lesson are **1, 1, 2, 3, 5**. The message number is the blink count, not the group's position in the table. After the five-blink group, the sequence starts again with one blink and continues for as long as the board has power.

Each blink keeps the LED on for 250 milliseconds and off for 250 milliseconds. After that normal off phase finishes for the final blink in a group, the program adds a 1,000-millisecond group pause. This means the LED remains dark for about 1,250 milliseconds between the last falling edge of one group and the first rising edge of the next.

This program contains the five values directly so you can focus on confirming the ESP32's output. You do not need to calculate Fibonacci numbers in this lesson.

**Compare:** Did every Serial Monitor number match the number of blinks in the group that followed it? **Yes / No**

**Stop safely:** Close the Serial Monitor with its close-terminal or trash-can button. When you are finished, unplug the USB cable by holding its connector.

## Focus Code: Announce, Then Blink

The complete program is in [`src/main.cpp`](src/main.cpp). The main idea is the `blinkFibonacciGroup` helper: it announces one number, uses a loop to blink that many times, and then waits before the next group.

```cpp
Serial.print("Blinking Fibonacci ");
Serial.println(blinkCount);

for (int blinkNumber = 0; blinkNumber < blinkCount; blinkNumber++) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(LED_ON_TIME_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(LED_OFF_TIME_MS);
}
```

The five lines in `loop()` call that helper with the supplied values `1`, `1`, `2`, `3`, and `5`. When the fifth call finishes, Arduino runs `loop()` again, so the sequence repeats forever.

`setup()` prepares the onboard LED as an output and starts Serial at 115200 baud. The named timing values and other setup lines make the program work, but the focus of this lesson is matching each announced value to its visible blink group. The calls to `delay()` intentionally pause the program during this simple first lesson.

## Troubleshooting

- **PlatformIO builds an Uno project:** Check the upload log. `.pio/build/uno`, `--environment uno`, `firmware.hex`, or `avrdude` means the Uno project is active. Open `platformio-esp32-lessons/01-hello-world` by itself and run tasks under `esp32doit-devkit-v1`. An ESP32 upload uses that environment, creates a `.bin` file, and uses `esptool`.
- **The board powers on but Upload cannot find it:** Try a known USB data cable and a direct computer USB port. A charge-only cable can light the power LED but cannot upload a program.
- **Upload remains at `Connecting...`:** First close the Serial Monitor and try Upload again. If automatic reset does not work, hold the board's **BOOT** button when `Connecting...` appears, release it when writing begins, and press **EN** or **RESET** once after upload if the program does not start. Ask your teacher before forcing buttons or changing system settings.
- **Linux reports permission denied or requests udev rules:** Stop and ask the teacher or system administrator to follow PlatformIO's [official udev-rules instructions](https://docs.platformio.org/en/latest/core/installation/udev-rules.html). This is a workstation setup issue, not a source-code change.
- **Monitor is empty:** Confirm that `esp32doit-devkit-v1` is the active project environment and that Monitor is using **115200 baud**. Close and reopen Monitor, then press **EN** or **RESET** once if needed.
- **The first message is not `Blinking Fibonacci 1`:** Monitor may have opened in the middle of a running cycle. Wait for the sequence to repeat, or press **EN** or **RESET** and watch from the beginning.
- **Monitor text is unreadable:** The monitor speed and `Serial.begin` speed must both be 115200. Stop Monitor, confirm the active ESP32 project, and reopen it.
- **A light stays on continuously:** That is probably the power indicator, which only proves that the board has power. Watch for the separate program-controlled LED connected to GPIO 2 on the supported board.
- **Serial messages work but no onboard LED blinks:** Verify that the board is the supported 30-pin DOIT ESP32 DevKit V1 and that its controllable LED is connected to GPIO 2. Some boards sold under similar ESP32 DevKit names omit that LED or wire their onboard LED differently. Successful serial messages still prove that the uploaded program is running; do not add an external circuit as part of this USB-only lesson.
