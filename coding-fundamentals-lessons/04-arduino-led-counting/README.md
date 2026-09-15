# Lesson 04: Arduino LED Counting

In lesson 03, a laptop program printed 1 through 5, then started again at 1. Here you will use the same counting idea to control a physical output: the Arduino Uno's built-in light.

## Learning Goals

By the end of this lesson, you will be able to:

- explain the difference between **run** and **upload**;
- observe the Uno's built-in LED blinking in groups;
- match a Serial Monitor group message with the same number of blinks;
- identify one loop that chooses a group and another that counts its blinks; and
- describe how the program keeps running while the Uno has power.

## What You Need

- [lesson 03: Repeating Counter](../03-repeating-counter/) completed;
- an Arduino Uno;
- a USB **data** cable that fits the Uno; and
- a Linux laptop with VS Code and the PlatformIO extension ready.

This lesson uses only USB and the light already built into the Uno. Do not connect a breadboard, external LED, Snap Circuits parts, jumper wires, or an external power supply.

## Core Mission: Watch, Match, and Name the Blink Groups

**Goal:** Upload the supplied program, watch the built-in LED blink, then match at least one blink group with a Serial Monitor message.

**Prepare:** Ask your teacher to confirm that the Uno, USB data cable, and PlatformIO are ready. Keep all external circuits and power supplies disconnected. Find the small built-in LED marked **L**; it is different from the power LED marked **ON**. Do not change `src/main.cpp`.

### Stage 1: Upload and watch the light

1. Plug only the USB data cable into the Uno and laptop.
2. In VS Code, choose **File > Open Folder...** and select `coding-fundamentals-lessons/04-arduino-led-counting`. Confirm that Explorer shows `platformio.ini` and the `src` folder. If VS Code asks whether you trust the folder, ask your teacher before continuing.
3. Wait for PlatformIO to load. Select its alien-head icon in the Activity Bar. Under **Project Tasks**, expand **uno**, then **General**.
4. Select **Build**. Wait for `SUCCESS` in the terminal. Build prepares and checks the program; it does not send it to the board.
5. Select **Upload**. Wait for the upload terminal to end with `SUCCESS`. The Uno restarts and runs the program automatically.
6. Watch the LED marked **L** before opening the Serial Monitor. Look for flashes followed by a longer dark pause.

**Notice 1:** What did the built-in light do?

______________________________________________________________

Could you see a longer pause between groups of flashes? **Yes / No**

### Stage 2: Match messages with flashes

7. In **Project Tasks > uno > General**, select **Monitor**. This project uses 9600 bits per second. Watch the messages and the LED together.
8. Wait until you see `Blink group: 1`. Count its flashes before the longer pause. Then choose one other group message and count its flashes.

**Notice 2:**

| Message seen | Blinks counted before the longer pause |
| --- | --- |
| `Blink group: 1` | ____ |
| `Blink group: ____` | ____ |

Did the message number match the number of flashes? **Yes / No**

### Stage 3: Name the two jobs

9. Open `src/main.cpp`. Find the comment `The outer loop chooses a group from 1 through 5.` and the comment `The inner loop flashes the LED once for each number in the group.`
10. Use what you saw to finish two short ideas:

- The **outer loop** chooses group number ____ through ____.
- The **inner loop** makes the number of blinks chosen by the current ____________.

**Stop safely:** Close Monitor with its close-terminal or trash-can button. When you are done watching the light, unplug USB to turn off the Uno. Remove power before swapping a board or cable; hold the plug by its connector.

## Name What Build, Upload, and Output Mean

In lessons 01–03, GCC built a program and you ran it on the laptop. In this lesson, PlatformIO builds on the laptop and **uploads** the program through USB. The program then **runs on the Uno**.

| Laptop lessons | This Arduino lesson |
| --- | --- |
| Build the source on the laptop. | Build the source on the laptop. |
| Run the program on the laptop. | Upload the program to the Uno. |
| Read words in the laptop terminal. | Watch the built-in LED and read Serial Monitor messages. |

The USB cable carries the program to the board. It also supplies power and carries Serial messages back to the laptop. **Output** can be printed words or a physical light. A successful Build does not prove physical blinking; the observation happens after Upload.

Closing the Serial Monitor only stops showing messages. It does not stop the program on the board. The program keeps repeating while the Uno has power.

## Find the Focus Loops

The supplied source is [`src/main.cpp`](src/main.cpp). The excerpt below keeps the two focus loops and the group message; the complete source also has setup and named timing values.

```cpp
for (int groupNumber = FIRST_GROUP; groupNumber <= LAST_GROUP;
     groupNumber++) {
    Serial.print("Blink group: ");
    Serial.println(groupNumber);

    // The inner loop flashes the LED once for each number in the group.
    for (int blinkNumber = 1; blinkNumber <= groupNumber; blinkNumber++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(LED_ON_TIME_MS);
        digitalWrite(LED_BUILTIN, LOW);
        delay(TIME_BETWEEN_BLINKS_MS);
    }
    delay(TIME_BETWEEN_GROUPS_MS);
}
```

The outer `for` loop chooses group 1, then 2, through 5. The inner `for` loop makes as many flashes as the current group number. After group 5, Arduino's `loop()` starts the group sequence at 1 again.

The timing names tell the program how long to keep the LED on and how long to wait. The longer pause between groups makes their boundaries easier to see. The `setup()` lines, `Serial` setup, and exact timing numbers help the real program work; you do not need to memorize those helper details.

## Try One Comparison Without Changing the Code

Use your first observation to make a prediction. You can reconnect USB to restart the same program if you unplugged it; do not attach an external circuit.

1. **Predict:** How many flashes do you expect after `Blink group: 2`? How many after `Blink group: 4`?

   Group 2: ____ flashes. Group 4: ____ flashes.

2. With the Uno running, open **Monitor** again and watch those two messages and their matching light groups. If you start watching in the middle of a cycle, wait for the next group 1.

   Group 2 observed: ____ flashes. Group 4 observed: ____ flashes.

3. **Explain:** The group 4 message led to ____ more flashes than group 2 because the inner loop uses the current ____________ as its blink count.

4. Watch after group 5 if you can. Did the next message and light group return to 1? **Yes / No**

When finished, close Monitor and unplug USB as described in the Core Mission.

## Troubleshooting

- **`platformio.ini` is missing:** VS Code opened the wrong folder. Reopen `coding-fundamentals-lessons/04-arduino-led-counting` by itself.
- **Build reports an error:** Read the first error with your teacher. Confirm `board = uno` in `platformio.ini` and that `src/main.cpp` has not been moved.
- **The Uno lights up, but Upload cannot find it:** Try a known USB data cable and a direct laptop USB port. A charge-only cable can power the board but cannot upload.
- **Upload reports that the port is busy:** Close Monitor and any other program using the Uno, then try Upload again.
- **Linux reports permission denied:** Stop and ask the teacher to fix the workstation's serial-device permissions.
- **Upload succeeds, but you do not see blinking:** Look for LED **L**, not power LED **ON**. Press the Uno's reset button once and watch again.
- **The blinks do not match the messages:** Start watching at `Blink group: 1` and count only the flashes before the longer pause.
- **No text appears in Monitor:** Confirm you selected **Monitor** under **uno** and the speed is 9600. Close and reopen Monitor once.
- **The first message is not group 1:** You may have opened Monitor during a running cycle. Wait for the next group 1 or press reset to start over.

You have now used output in two places: text appeared in a monitor, and light appeared on the Arduino. Both outputs followed counters and loops in the program.
