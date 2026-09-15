# Lesson 04: Arduino LED Counting

In the last lesson, a laptop program printed groups of numbers. This program uses the same counting idea to make a physical output: the Arduino Uno's built-in light blinks in groups of one through five.

## Learning Goals

By the end of this lesson, you will be able to:

- explain the difference between **run** and **upload**;
- identify one loop that chooses a blink group from 1 through 5;
- identify another loop that makes the matching number of blinks;
- match Serial Monitor output with the Uno's built-in LED; and
- describe how a program keeps running on an Arduino.

## What You Need

- an Arduino Uno;
- a USB **data** cable that fits the Uno;
- a Linux laptop with VS Code and the PlatformIO extension; and
- [lesson 03: Repeating Counter](../03-repeating-counter/) completed.

This lesson uses only the USB cable and the light already built into the Uno. Do not connect a breadboard, external LED, Snap Circuits parts, jumper wires, or an external power supply.

## From Laptop Output to Arduino Output

In lessons 01 through 03, you built a program and ran it on the laptop. This time, the laptop builds the program and **uploads** it through the USB cable. Upload means sending the built program to the Arduino. The program then runs on the Arduino.

| Laptop lessons | This Arduino lesson |
| --- | --- |
| Build the source code on the laptop. | Build the source code on the laptop. |
| Run the program on the laptop. | Upload the program to the Uno. |
| Read output in the terminal. | Watch the built-in LED and read output in the Serial Monitor. |

The USB cable carries the program to the board. It also supplies power and carries Serial Monitor messages back to the laptop.

## Find the Two Loops

Open `src/main.cpp`. Look for these two instructions:

```cpp
for (int groupNumber = FIRST_GROUP; groupNumber <= LAST_GROUP;
     groupNumber++) {
```

This outside loop chooses group 1, then group 2, and continues through group 5.

Inside it, find:

```cpp
for (int blinkNumber = 1; blinkNumber <= groupNumber; blinkNumber++) {
```

This inside loop makes one blink for every number in the current group. When `groupNumber` stores 3, the inside loop makes 3 blinks.

The program uses `LED_BUILTIN`, so PlatformIO uses the correct built-in LED pin for the Uno. The timing values have names such as `LED_ON_TIME_MS` and `TIME_BETWEEN_GROUPS_MS`. You do not need to memorize their numbers. Notice that the pause between groups is longer than the short pause between blinks. That makes each group easier to see.

After group 5, Arduino's `loop()` starts again. The next group is group 1.

## Predict

Before connecting the Uno, complete these predictions:

1. The Serial Monitor will show these first six group numbers:

   ____  ____  ____  ____  ____  ____

2. When the Serial Monitor says `Blink group: 4`, the built-in LED will blink ____ times.

3. After group 5, I predict _______________________________________.

4. I think the longer pause will happen **between blinks / between groups**. Circle one.

## Prepare the Uno

1. Make sure nothing is connected to the Uno except the USB cable used in this lesson.
2. Check that the cable is a USB **data** cable. A charge-only cable can power the board but cannot upload the program.
3. Plug the cable into the Uno and the laptop.
4. Find the small built-in LED marked **L** on the Uno. Its exact position may vary slightly between boards.

## Open the Correct Project

This lesson directory is a complete PlatformIO project. Open it by itself so PlatformIO uses the correct board and program.

1. Open VS Code.
2. Choose **File > Open Folder...**.
3. Select `coding-fundamentals-lessons/04-arduino-led-counting`.
4. Confirm that the Explorer shows `platformio.ini` and the `src` folder.
5. If VS Code asks whether you trust the folder, ask your teacher before continuing.
6. Wait for the PlatformIO extension to finish loading.

## Build and Upload

1. Select the PlatformIO alien-head icon in the Activity Bar.
2. Under **Project Tasks**, expand **uno**, then expand **General**.
3. Select **Build**. Building checks the source code and prepares a program for the Uno. It does not send the program to the board yet.
4. Watch the terminal. A successful build ends with `SUCCESS` and no error message.
5. Select **Upload**. PlatformIO may build again before sending the program through USB.
6. Wait for the upload terminal to end with `SUCCESS`.

After a successful upload, the Uno restarts and runs the program automatically. You should see the built-in LED blink even before opening the Serial Monitor.

## Open the Serial Monitor

1. In **Project Tasks > uno > General**, select **Monitor**.
2. The monitor uses 9600 bits per second because `monitor_speed = 9600` appears in `platformio.ini`. The source code stores the same number as `SERIAL_SPEED` and uses it to start Serial.
3. Watch the messages while you also watch the built-in LED.

The messages should repeat like this:

```text
Blink group: 1
Blink group: 2
Blink group: 3
Blink group: 4
Blink group: 5
Blink group: 1
```

Each message appears before its matching group of blinks. The LED should blink once for group 1, twice for group 2, and so on. A longer dark pause separates one group from the next.

## Observe and Explain

Record what you actually observe, even if it differs from your prediction.

| Group shown in the monitor | Number of blinks observed |
| --- | --- |
| 1 | ____ |
| 2 | ____ |
| 3 | ____ |
| 4 | ____ |
| 5 | ____ |

Did the monitor return to group 1 after group 5? **Yes / No**

Could you tell where one group ended and the next began? **Yes / No**

Complete the explanations:

- The outside loop ______________________________________________.
- The inside loop _______________________________________________.
- The longer pause ______________________________________________.
- Upload means _________________________________________________.

## Stop and Disconnect

The program keeps repeating while the Uno has power. Closing the Serial Monitor only stops showing its messages; it does not stop the program on the board.

1. Close the Serial Monitor with its trash-can or close-terminal button.
2. When you are finished observing, unplug the USB cable to turn off the Uno.
3. Remove power before swapping boards or cables. Hold a plug by its connector instead of pulling on the cable.

## Troubleshooting

- **`platformio.ini` is missing:** VS Code opened the wrong folder. Reopen `coding-fundamentals-lessons/04-arduino-led-counting` by itself.
- **Build reports an error:** Read the first error with your teacher. Confirm that `board = uno` appears in `platformio.ini` and that `src/main.cpp` has not been moved.
- **The Uno lights up, but Upload cannot find it:** Try a known USB data cable and a direct laptop USB port. A charge-only cable cannot upload.
- **Upload reports that the port is busy:** Close the Serial Monitor and any other program using the Uno, then try Upload again.
- **Linux reports permission denied:** Stop and ask the teacher to fix the workstation's serial-device permissions.
- **Upload succeeds, but you do not see blinking:** Look for the small LED marked **L**, not the power LED marked **ON**. Press the Uno's reset button once and watch again.
- **The blinks do not match the messages:** Start watching at `Blink group: 1`, count only the flashes before the longer pause, and compare again.
- **No text appears in the Serial Monitor:** Confirm that the task is **Monitor** under the `uno` environment and that the speed is 9600. Close and reopen Monitor once.
- **The first message is not group 1:** You may have opened the monitor during a running cycle. Press the Uno's reset button and watch from the beginning.

You have now used output in two places: text appeared in a monitor, and light appeared on the Arduino. Both outputs followed counters and loops in the program.
