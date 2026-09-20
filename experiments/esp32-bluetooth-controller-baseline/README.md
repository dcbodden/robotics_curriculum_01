# ESP32 Bluetooth Controller Baseline

This standalone experiment checks whether a Mystiluck/Senze SZ-4015B game
controller in PS4-compatible mode can connect to an ESP32 and produce controller
input reports. It emits machine-readable `BTDIAG` records at 115200 baud while
leaving the underlying ESP-IDF, Bluepad32, and BTstack logs visible.

The firmware builds successfully for the supported board. A successful build is
not proof that this particular controller connects; that requires the later
physical first-pair and reconnection tests.

## Supported hardware

- An original ESP32-WROOM-32 development board in the 30-pin DOIT DevKit V1
  layout
- A USB data cable
- A Mystiluck/Senze SZ-4015B controller operating in PS4-compatible mode

The original ESP32 is required because PS4-compatible gamepads use Bluetooth
Classic BR/EDR. ESP32-C3 and ESP32-S3 boards do not provide Bluetooth Classic
and cannot substitute for the ESP32-WROOM in this experiment.

The PlatformIO environment is named `esp32doit-devkit-v1` and uses the board ID
`esp32doit-devkit-v1`.

## Important: use the experiment-local PlatformIO wrapper

Use `scripts/pio.sh` for every build, upload, device-list, and monitor command.
The wrapper installs and runs the pinned PlatformIO Core 6.1.19 in `.tooling/`
and gives it a project-local package cache. It does not modify or depend on the
user's global PlatformIO installation.

This matters because `platformio.ini` pins the pioarduino platform but cannot
pin the PlatformIO Core that launched it. The selected pioarduino/ESP-IDF stack
works with the wrapper's pinned SCons and Click versions. A global PlatformIO
Core 6.2 environment can instead select incompatible host-tool versions and fail
during configuration.

VS Code's PlatformIO toolbar normally uses the extension's global or bundled
Core and global package directory. Consequently, its Build and Upload buttons
are not the reproducible path for this experiment. VS Code is still a useful
editor: open its integrated terminal and run the same project-local commands
shown below.

## One-time dependency setup

From the repository root:

```bash
cd experiments/esp32-bluetooth-controller-baseline
./scripts/bootstrap-dependencies.sh
./scripts/pio.sh --version
```

The bootstrap script downloads checksum-verified, pinned Arduino, Bluepad32,
BTstack, and template components into `components/`. The first `pio.sh` command
creates the ignored `.tooling/` environment and may download the pinned
PlatformIO toolchain and framework packages. It should report:

```text
PlatformIO Core, version 6.1.19
```

See [DEPENDENCIES.md](DEPENDENCIES.md) for exact revisions, sources, checksum
handling, and license obligations.

Host prerequisites are Python 3 with `venv`, `curl`, `tar`, and `sha256sum`.
Internet access is required during the initial dependency setup.

## Confirm the active project before building or uploading

Open this directory itself as the VS Code folder:

```text
robotics_curriculum_01/experiments/esp32-bluetooth-controller-baseline/
```

Then check from the integrated terminal:

```bash
pwd
grep -E '^(default_envs|\[env:)' platformio.ini
./scripts/pio.sh --version
```

The expected environment lines are:

```text
default_envs = esp32doit-devkit-v1
[env:esp32doit-devkit-v1]
```

Every correct build starts with output similar to:

```text
Processing esp32doit-devkit-v1
PLATFORM: Espressif 32 ...
HARDWARE: ESP32 ...
```

Stop immediately if the output says `Processing uno`, invokes `avrdude`, or
uploads a `.hex` file. Those identify an Arduino Uno project, not this ESP32
experiment. A correct ESP32 upload uses `esptool` and a `firmware.bin` image.

## Build

From the experiment directory:

```bash
./scripts/pio.sh run --environment esp32doit-devkit-v1
```

The first build can take several minutes because ESP-IDF, Arduino, Bluepad32,
and BTstack are compiled together. Later incremental builds are much faster.

## Find the serial port

Connect the ESP32 with a USB data cable, then run:

```bash
./scripts/pio.sh device list
```

On Linux the port is commonly `/dev/ttyUSB0` or `/dev/ttyACM0`. If PlatformIO
warns that its udev rules are missing, install the rules using the
[official PlatformIO instructions](https://docs.platformio.org/en/latest/core/installation/udev-rules.html),
then reconnect the board. Do not work around serial permissions by running the
project as root.

## Upload

Close any serial monitor that owns the port, then run:

```bash
./scripts/pio.sh run --environment esp32doit-devkit-v1 --target upload
```

PlatformIO normally detects the only connected ESP32. If more than one serial
device is present, provide the selected port explicitly:

```bash
./scripts/pio.sh run --environment esp32doit-devkit-v1 --target upload \
  --upload-port /dev/ttyUSB0
```

Most DOIT DevKit V1 boards enter the bootloader automatically. If upload remains
at `Connecting...`, hold `BOOT`, briefly press and release `EN`/`RST`, and release
`BOOT` when writing begins.

## Monitor and reset

After upload, open the 115200-baud monitor using the same local environment:

```bash
./scripts/pio.sh device monitor --environment esp32doit-devkit-v1
```

Specify the port when automatic selection is ambiguous:

```bash
./scripts/pio.sh device monitor --environment esp32doit-devkit-v1 \
  --port /dev/ttyUSB0
```

With the monitor open, press and release the board's `EN` or `RST` button. This
deliberate reset lets the monitor capture the earliest firmware evidence. Look
for the following ordered `BTDIAG` events among the lower-level stack logs:

```text
firmware_started
dependency_identity
bluetooth_ready
accepting_connections
```

If those events do not appear, reset the board once more before diagnosing the
controller. Exit the monitor with `Ctrl+C`; always do this before another upload
because only one process can own the serial port.

## First-pair test with the SZ-4015B

Use this procedure when the controller and ESP32 do not already share a stored
Bluetooth bond. Starting with a previously bonded reconnection does not provide
first-pair evidence.

1. Start with the SZ-4015B powered off. Its channel lights should not be lit or
   flashing.
2. Upload the experiment firmware, open the serial monitor, and reset the ESP32
   as described above.
3. Wait for the ordered `firmware_started`, `bluetooth_ready`, and
   `accepting_connections` records. Begin pairing promptly: the firmware accepts
   new connections for 75 seconds after startup.
4. Hold the controller's `SHARE` and `HOME` buttons together for approximately
   three seconds, until the white channel light flashes. Release both buttons.
5. Watch the serial stream for a `BTDIAG` record whose `event` is
   `controller_connected`. The reported model name, vendor ID, and product ID
   are observations; they are not required to match a hard-coded value for this
   clone controller.
6. After `controller_connected` appears, press and hold the controller's Cross
   button for about half a second, then release it.
7. Find the subsequent `BTDIAG` input record. Its `event` will be either
   `controller_first_input` or `controller_input`; it must contain
   `"valid_report":true` and its `pressed_buttons` value must include `cross`.

The first-pair test passes only when the same session contains both of these, in
order:

```text
controller_connected
controller_first_input or controller_input with valid_report=true and cross pressed
```

The channel light, a connection record by itself, a neutral input record, or
unstructured Bluetooth stack output is not sufficient to mark the test as a
success. The requested Cross-button record demonstrates that the connected HID
device is delivering usable controller input to this firmware.

If `connection_timeout` appears before `controller_connected`, reset the ESP32
to open a new 75-second connection window and repeat from the powered-off
controller state. A timeout records only that no usable connection was observed
within the window; it does not by itself prove that the controller is defective
or incompatible.

## Command summary

Run all commands from `experiments/esp32-bluetooth-controller-baseline/`:

```bash
# Prepare pinned source dependencies once
./scripts/bootstrap-dependencies.sh

# Verify the local Core
./scripts/pio.sh --version

# Build
./scripts/pio.sh run -e esp32doit-devkit-v1

# List ports
./scripts/pio.sh device list

# Upload
./scripts/pio.sh run -e esp32doit-devkit-v1 -t upload

# Monitor at the configured 115200 baud
./scripts/pio.sh device monitor -e esp32doit-devkit-v1
```
