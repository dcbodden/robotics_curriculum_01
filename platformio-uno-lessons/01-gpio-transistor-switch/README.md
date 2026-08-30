# Lesson 01: GPIO Transistor Switch

In this lesson, an Arduino Uno changes digital pin 8 between **HIGH** and **LOW** every two seconds. Pin 8 controls an NPN transistor in an existing Snap Circuits LED assembly. You will watch the LED and use a digital multimeter to measure what HIGH and LOW mean in volts.

## Learning Goals

By the end of the lesson, you will be able to:

- explain that a digital output has a HIGH state and a LOW state;
- observe a transistor-controlled LED responding to pin 8;
- measure pin 8 relative to shared ground; and
- build and upload one PlatformIO project to an Arduino Uno.

## What You Need

- an Arduino Uno;
- a USB **data** cable that fits the Uno;
- the existing, teacher-approved Snap Circuits NPN-transistor-and-LED assembly;
- two teacher-approved jumper connections; and
- a digital multimeter with insulated probes.

Do not rebuild or move parts inside the transistor-and-LED assembly. This lesson adds only the two connections described below.

## Connect the Existing Snap Circuits Assembly

**Disconnect the Uno's USB cable and turn off or remove power from the Snap Circuits assembly before adding, removing, or moving any connection.**

The Arduino and the existing assembly need exactly two connections:

| Arduino Uno | Existing Snap Circuits connection | Why it is needed |
| --- | --- | --- |
| GND | The assembly's teacher-identified ground node | Gives both systems the same 0 V reference, called **shared ground**. |
| Digital pin 8 (`D8`) | The assembly's teacher-identified NPN base control point | Lets pin 8 control the existing transistor switch. |

Follow these steps:

1. Disconnect the USB cable from the Uno.
2. Turn off or remove power from the Snap Circuits assembly.
3. Ask the teacher to identify the assembly's ground node and NPN base control point. Do not guess from wire color or move any of the assembly's parts.
4. Connect an Arduino **GND** pin to the identified Snap Circuits ground node.
5. Connect Arduino digital pin **8** to the identified NPN base control point.
6. Check that no wire connects Arduino 5 V to the assembly. The Arduino supplies only shared ground and the pin-8 control signal in this lesson.
7. **Teacher checkpoint:** The teacher traces both connections, confirms that D8 reaches only the assembly's intended protected base-control point, and confirms that there is no short circuit.
8. Reconnect power only after the teacher approves the two connections.

## Connect, Build, and Upload

The lesson directory is a complete PlatformIO project. Open this directory by itself so PlatformIO builds only lesson 01.

1. Use a USB **data** cable to connect the Arduino Uno to the computer. A charge-only cable may light the board but cannot upload a program.
2. Open VS Code. Choose **File > Open Folder...**, then select the `platformio-uno-lessons/01-gpio-transistor-switch` folder containing `platformio.ini`.
3. If VS Code asks whether you trust the folder, ask the teacher before continuing. Wait for the PlatformIO IDE extension to finish loading.
4. Select the PlatformIO alien-head icon in the Activity Bar. Under **Project Tasks**, expand **uno**, then expand **General**.
5. Select **Build** (the checkmark action). Watch the terminal. A successful build ends with `SUCCESS` and no error message.
6. Select **Upload** (the right-arrow action). PlatformIO builds the program again, finds the connected Uno, and sends the program to it.
7. Wait for the upload terminal to end with `SUCCESS`. Messages from `avrdude` may appear during a normal Uno upload.
8. After a successful upload, the Uno restarts and runs the program automatically. Pin 8 stays HIGH for two seconds, then LOW for two seconds, and repeats.

## Measure HIGH and LOW with a Multimeter

Use voltage mode only. The meter measures pin 8 **relative to shared ground**, meaning it compares D8 with the ground connection used by both systems.

> **Never use resistance or current mode on the powered circuit. Never put the red lead in a current jack for this activity.**

1. With circuit power removed, turn the meter off and check its leads. Put the black lead in **COM** and the red lead in the voltage/resistance jack, usually marked **VΩ**.
2. Set the meter to DC voltage on a range that can safely include 5 V. Ask the teacher to verify the jacks and dial.
3. Connect or hold the black probe on Arduino GND or the teacher-identified shared-ground point.
4. Connect or hold the red probe on Arduino D8. Keep the probe from touching a neighboring pin.
5. Ask the teacher to check the probes and the two assembly connections before power is restored.
6. Reconnect the Uno USB data cable and the teacher-approved assembly power. Do not move the probes while the circuit is powered.
7. Watch several cycles. A reading near 5 V is the **HIGH** interval; a reading near 0 V is the **LOW** interval. Each interval lasts about two seconds.
8. During each interval, look at the LED and record whether it is on or off in [`recording-activity.md`](recording-activity.md). Record what you actually observe because the assembly's LED response may depend on how its transistor circuit is arranged.
9. When both rows are complete, disconnect USB and assembly power before removing the probes or changing any wiring.

## Troubleshooting

- **The Uno lights up, but Upload cannot find it:** The cable may carry power only. Try a known USB data cable and a direct computer USB port instead of an unpowered hub.
- **The board does not appear:** Unplug and reconnect the Uno, try another USB port, and confirm that `board = uno` appears in this lesson's `platformio.ini`. A teacher can open a PlatformIO terminal and run `pio device list` to list detected serial devices.
- **The upload port is busy or cannot be opened:** Close Arduino IDE, serial monitors, and other programs that may be using the Uno's port. Disconnect and reconnect the board, then try **Upload** again. PlatformIO normally detects the upload port automatically.
- **Linux reports permission denied:** This is a teacher or system-administrator task. Follow PlatformIO's official [Linux udev-rules instructions](https://docs.platformio.org/en/latest/core/installation/udev-rules.html), or identify the group that owns the detected serial device and add the classroom user to that group. Group names vary by system and may include `dialout`, `plugdev`, or `uucp`; sign out and back in after a group change.
- **Build fails before upload starts:** Make sure VS Code opened the lesson directory that contains this `platformio.ini`, then choose **Build** again and read the first error in the terminal.
- **Upload says `SUCCESS`, but the LED does not change:** Disconnect all power. Ask the teacher to recheck shared ground, the D8-to-base-control connection, and the existing assembly's own power. Do not rebuild the assembly.
- **The voltage changes but seems a little above 0 V or below 5 V:** Small differences are normal. Record the measured value instead of changing the circuit to force an exact number.

PlatformIO's official [VS Code guide](https://docs.platformio.org/en/latest/integration/ide/vscode.html) describes the Build and Upload actions, and its [`pio device list` guide](https://docs.platformio.org/en/latest/core/userguide/device/cmd_list.html) explains the teacher-facing device check.

