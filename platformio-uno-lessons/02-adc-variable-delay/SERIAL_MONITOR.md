# Using the Serial Monitor

The serial monitor shows the ADC reading from A0 and the delay used for each HIGH or LOW interval. The Arduino Uno uses the same USB connection for program uploads and serial messages, so only one operation should use the port at a time.

## What You Need

- An Arduino Uno connected to the computer with a USB data cable. Some USB cables provide power but do not carry data.
- This lesson folder open in VS Code as the PlatformIO project.
- The PlatformIO IDE extension installed and ready.
- No other serial-monitor program using the Arduino's USB port.

The sketch and `platformio.ini` both use 9,600 baud. PlatformIO reads `monitor_speed = 9600` from the project configuration, so you should not need to enter the speed manually.

## Connect and Upload

1. Make sure the serial monitor is closed before connecting or changing lesson wiring.
2. With the USB cable disconnected, check the potentiometer and transistor/LED wiring.
3. Connect the Arduino Uno to the computer with the USB cable.
4. In VS Code, open the PlatformIO project in this `02-adc-variable-delay` folder.
5. If more than one serial device is connected, open the Command Palette with **Ctrl+Shift+P**, run **PlatformIO: Set Project Port**, and select the Arduino Uno's port.
6. Select **PlatformIO: Upload** from the Command Palette, or click PlatformIO's Upload arrow in the status bar.
7. Wait until the upload terminal reports `SUCCESS`. Do not open the serial monitor while an upload is still running.

## Open and Read the Serial Monitor

1. Open the Command Palette with **Ctrl+Shift+P**.
2. Run **PlatformIO: Serial Monitor**. You can also use PlatformIO's serial-monitor plug icon when it is visible in the status bar.
3. If PlatformIO asks for a port, select the same Arduino Uno port used for the upload.
4. Turn the potentiometer and watch for lines similar to this:

   ```text
   ADC reading: 512 | Delay: 1050 ms
   ```

Opening the monitor may reset the Uno. A brief pause before the first line appears is normal. Lower ADC readings should show shorter delays, and higher readings should show longer delays.

## Upload Again While the Monitor Is Open

The upload tool and serial monitor share the Uno's USB serial port. Before another upload:

1. Click inside the serial-monitor terminal and press **Ctrl+C**, or use the terminal trash-can button to close that monitor terminal.
2. Start **PlatformIO: Upload** and wait for `SUCCESS`.
3. Run **PlatformIO: Serial Monitor** again after the upload finishes.

If an upload reports that the port is busy or cannot be opened, close every serial-monitor terminal and any other application that may be using the Arduino port, then retry the upload.

## Disconnect Cleanly

1. Stop the serial monitor with **Ctrl+C** or close its terminal with the trash-can button.
2. Wait until the terminal says the monitor stopped or returns to a command prompt.
3. Close any upload terminal that is still open and confirm that no upload is running.
4. Unplug the USB cable from the computer or Arduino.
5. Make wiring changes only after USB power is disconnected.

The serial monitor only observes messages; it does not save unsent data on the Uno. Closing the monitor before unplugging cleanly releases the shared USB port and prevents a later upload from finding it busy.

## Troubleshooting

- **No port appears:** Try another USB data cable or USB socket. Confirm that the Uno's power light turns on.
- **Permission denied on Linux:** Add your user to the serial-port access group with `sudo usermod -aG dialout "$USER"`, then sign out and back in. Ask a teacher or administrator before changing computer permissions.
- **Unreadable characters:** Stop the monitor and confirm that this project's `monitor_speed` and `Serial.begin` are both set to `9600`.
- **No text, but upload succeeded:** Close and reopen the monitor, verify the selected port, and wait through the Uno's brief reset.
- **Port busy during upload:** Stop the serial monitor and close other programs that use the Arduino port before uploading again.