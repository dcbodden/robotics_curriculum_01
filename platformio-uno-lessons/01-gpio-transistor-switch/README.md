# Lesson 01: GPIO Transistor Switch

The Arduino Uno changes digital pin 8 (`D8`) between **HIGH** and **LOW** every two seconds. First you will measure those states directly. Then the same pin will control the existing teacher-approved Snap Circuits transistor-and-LED assembly.

## Recommended Prior Learning

Complete [Multimeter and Ohm's Law](../../electronics-fundamentals-lessons/01-multimeter-ohms-law/) and the four [Coding Fundamentals Lessons](../../coding-fundamentals-lessons/) first. Their final [Arduino LED Counting](../../coding-fundamentals-lessons/04-arduino-led-counting/) lesson practices Build, Upload, loops, and output with the Uno's built-in LED before this lesson adds an external circuit.

## Learning Goals

By the end of the lesson, you will be able to:

- explain that a digital output has a HIGH state and a LOW state;
- measure D8 relative to Arduino GND while no external control lead is connected;
- compare the same D8 states with an approved transistor-controlled LED; and
- build and upload one PlatformIO project to an Uno.

## What You Need

- an Arduino Uno and USB **data** cable;
- a digital multimeter with insulated probes;
- the existing, teacher-approved Snap Circuits NPN-transistor-and-LED assembly and its approved power source; and
- two teacher-approved jumper connections for the later assembly stage.

Do not rebuild or move parts inside the existing assembly. The Uno adds only a shared-ground connection and D8 control lead after the direct measurement.

## Core Mission: Measure D8, Then Watch the LED

**Goal:** Observe D8 near the Uno's HIGH and LOW voltages with its external control lead disconnected, then compare those same pin states with the approved assembly's LED.

**Prepare and stay safe:** Ask the teacher to confirm the approved assembly, its protected NPN base-control point, ground node, power source, meter, and USB data cable. Keep the assembly's power off and its two leads **disconnected from the Uno** during Stage 1. Never use resistance or current mode on a powered circuit or put the red meter lead in a current jack. Disconnect **USB and assembly power before adding, moving, or removing a wire or probe**. If a part becomes hot, a lead is loose, or a reading seems unsafe, turn off assembly power, unplug USB, and ask the teacher to inspect it.

### Stage 1: Measure the program's output before adding a circuit

1. Leave **D8 and Arduino GND unconnected to the external assembly**. Connect only the USB data cable to the Uno and laptop.
2. In VS Code choose **File > Open Folder...** and open `platformio-uno-lessons/01-gpio-transistor-switch` by itself. Confirm that Explorer shows `platformio.ini` and `src/main.cpp`. If asked whether to trust the folder, ask your teacher; wait for PlatformIO to load.
3. Select the PlatformIO alien-head icon. Under **Project Tasks > uno > General**, choose **Build** and wait for `SUCCESS`. Then choose **Upload** and wait for its `SUCCESS`. A charge-only cable can light the Uno but cannot upload. The Uno restarts and begins alternating D8 HIGH and LOW automatically.
4. **Unplug USB before placing the meter probes.** With the meter off, put black in **COM** and red in **VΩ**. Set a teacher-approved **DC voltage** range that includes 5 V, not AC, resistance, or current. Place black on **Arduino GND** and red on **D8** without touching a neighboring pin.
5. **Teacher checkpoint:** The teacher confirms D8 has no external control lead, the assembly is unpowered and disconnected from the Uno, the probes touch only D8 and Arduino GND, and the meter jacks and dial are correct. Reconnect USB only after approval. Do not move the probes while powered.
6. Watch several two-second HIGH and LOW intervals. Record D8-to-GND voltage in the [recording activity](recording-activity.md): HIGH should be near the Uno's logic-high level (about 5 V), and LOW should be near 0 V. Record actual values, even if they are not exact.

**Notice 1:** What two voltage ranges did the program create at D8?

HIGH measured: ______ V. LOW measured: ______ V.

7. **Unplug USB before removing the probes or changing any connection.** Turn the meter off. Keep the external assembly unpowered.

**Predict from Stage 1:** When D8 is connected to the approved transistor control point, what do you think the LED will do during HIGH and LOW? Your prediction may differ from the actual assembly response.

HIGH LED prediction: ______. LOW LED prediction: ______.

### Stage 2: Connect the approved assembly only after power-off approval

8. With **USB unplugged and assembly power off**, ask the teacher to identify the assembly's ground node and **protected NPN base-control point**. Do not guess by wire color or rebuild its parts. Connect Arduino **GND** to the identified assembly ground node, then connect **D8** only to the identified protected base-control point. **Do not connect Arduino 5 V to the assembly.**
9. **Teacher checkpoint:** The teacher traces both connections, verifies shared ground and the protected D8 base-control path, confirms there is no short circuit, and approves the assembly's own power. Do not restore either power source until approved.
10. With power still off, check that the meter remains in DC voltage mode, black in COM, red in VΩ. Put black on **Arduino GND or the verified shared-ground point** and red on **D8**, keeping it clear of neighboring pins. Ask the teacher to check the probes again.
11. After approval, reconnect USB and the assembly's approved power. Do not move probes or wires while powered. Watch several HIGH and LOW two-second intervals. Record each D8 voltage and whether the assembly LED is on or off in the [recording activity](recording-activity.md). The LED response may depend on the existing transistor arrangement, so write what you actually see.

**Notice 2:** During HIGH and LOW, how did the LED state compare with the D8 voltage and your prediction?

HIGH LED state: ______. LOW LED state: ______.

**Stop safely:** Turn off or remove assembly power, then unplug USB. Only after both sources are off may you remove probes or change any connection. Leave the existing assembly intact.

## Name the Idea: One Program, Two Observations

The program made D8 alternate between HIGH and LOW in both stages. In the first stage, you saw that change **as voltage relative to Arduino GND**. In the second, the same output also reached a transistor control point through a protected connection, with a shared ground, and you saw the assembly LED respond.

The focus instructions in [`src/main.cpp`](src/main.cpp) are:

```cpp
digitalWrite(TRANSISTOR_PIN, HIGH);
delay(STATE_DURATION_MS);
digitalWrite(TRANSISTOR_PIN, LOW);
delay(STATE_DURATION_MS);
```

`TRANSISTOR_PIN` is D8 and `STATE_DURATION_MS` is 2,000 milliseconds, or two seconds. The `setup()` line that prepares D8 as an output and the PlatformIO project settings help the real program work; you do not need to memorize their syntax.

**Compare:** Were the D8 voltages approximately similar before and after the approved assembly was connected? What new output could you observe in Stage 2?

______________________________________________________________

## Troubleshooting and Teacher Reference

- **Build or Upload fails:** Confirm VS Code opened the folder containing this `platformio.ini`, read the first error with the teacher, and check that the cable carries data. A teacher can use PlatformIO's [device-list guide](https://docs.platformio.org/en/latest/core/userguide/device/cmd_list.html) if the Uno port is not found.
- **Upload port is busy:** Close serial monitors and other programs using the Uno, then try again.
- **Linux reports permission denied:** This is a teacher or system-administrator task. Follow PlatformIO's official [Linux udev-rules instructions](https://docs.platformio.org/en/latest/core/installation/udev-rules.html) or check the serial-device owner group; sign out and back in after a group change.
- **D8 voltage does not change in Stage 1:** Unplug USB before checking the probes. The teacher checks that black is on Arduino GND, red is on D8, the meter is in DC voltage mode, and the supplied source was uploaded. Do not connect the assembly to hide the missing direct result.
- **D8 voltage changes but the assembly LED does not in Stage 2:** Remove both power sources. Ask the teacher to recheck shared ground, the protected D8-to-base-control connection, and the assembly's own power. Do not rebuild the assembly.
- **Voltage is a little above 0 V or below 5 V:** Small differences are normal. Record the measured value instead of changing the circuit to force an exact number.

PlatformIO's [VS Code guide](https://docs.platformio.org/en/latest/integration/ide/vscode.html) explains Build and Upload. After completing this lesson, continue to [lesson 02: ADC Variable Delay](../02-adc-variable-delay/).
