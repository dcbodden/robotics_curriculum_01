# Lesson 02: Bluetooth Motor and Servo Control

This independently buildable ESP32-WROOM lesson is the starting point for
wireless bench control of one bidirectional motor and one servo with a
Mystiluck/Senze SZ-4015B game controller in PS4-compatible mode. This scaffold
retains the controller connection and machine-readable `BTDIAG` behavior from
the standalone baseline; the actuator mapping, safety policy, and output stages
are introduced by the following implementation tasks.

The lesson was derived from
`experiments/esp32-bluetooth-controller-baseline/` at repository commit
`463dd061bb77d4065c8e79673026f32792fddff6`, using baseline tree
`4bf49e1acf66f3b483e9e57247cb9616626efbad`. It keeps the same pinned
dependency revisions but has an independent build tree, local tooling directory,
and firmware identity. Building this directory never compiles the baseline or
another lesson. See [DEPENDENCIES.md](DEPENDENCIES.md) for the full revision
record.

The scaffold builds successfully for the supported board. A successful build is
not proof that this particular controller connects or that actuators operate;
those claims require separate physical verification.

See [BUILD_VERIFICATION.md](BUILD_VERIFICATION.md) for the dated clean-build
record, resolved versions, memory usage, warnings, and verification boundary.

## Supported hardware

- An original ESP32-WROOM-32 development board in the 30-pin DOIT DevKit V1
  layout
- A USB data cable
- A Mystiluck/Senze SZ-4015B controller operating in PS4-compatible mode

The original ESP32 is required because PS4-compatible gamepads use Bluetooth
Classic BR/EDR. ESP32-C3 and ESP32-S3 boards do not provide Bluetooth Classic
and cannot substitute for the ESP32-WROOM in this lesson.

The PlatformIO environment is named `esp32doit-devkit-v1` and uses the board ID
`esp32doit-devkit-v1`.

## Important: use the lesson-local PlatformIO wrapper

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
are not the reproducible path for this lesson. VS Code is still a useful
editor: open its integrated terminal and run the same project-local commands
shown below.

## One-time dependency setup

From the repository root:

```bash
cd platformio-esp32-lessons/02-bluetooth-motor-servo-control
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
robotics_curriculum_01/platformio-esp32-lessons/02-bluetooth-motor-servo-control/
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
lesson. A correct ESP32 upload uses `esptool` and a `firmware.bin` image.

## Build

From the lesson directory:

```bash
./scripts/pio.sh run --environment esp32doit-devkit-v1
```

The first build can take several minutes because ESP-IDF, Arduino, Bluepad32,
and BTstack are compiled together. Later incremental builds are much faster.

## Run the hardware-free control checks

The left-stick mapping and safety policy are standard C++ and can be checked
without PlatformIO or connected hardware:

```bash
./test/run-host-checks.sh
```

See [test/README.md](test/README.md) for the behavior covered by each check.

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
2. Upload the lesson firmware, open the serial monitor, and reset the ESP32
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

## Bonded-reconnection test

Run this separately after a successful first-pair test. Leave the ESP32's stored
bond intact; this test checks whether the controller can reconnect without
entering first-pair mode again.

1. Power off or disconnect the controller, then reset or power-cycle the ESP32.
2. Open the serial monitor before resetting when possible, and wait for
   `firmware_started`, `bluetooth_ready`, and `accepting_connections`.
3. Press `HOME` once to start the controller. Do not hold `SHARE` and `HOME`.
4. Wait for a new `controller_connected` record.
5. Press and hold Cross for about half a second, then release it.
6. Require a subsequent `controller_first_input` or `controller_input` record
   with `"valid_report":true` and `pressed_buttons` containing `cross`.

The reconnection test passes only when the new session contains both the new
connection record and the requested input evidence. A channel light or an old
record left in the terminal does not demonstrate a fresh reconnection.

## Clear stored Bluetooth bonds

Bond clearing changes nonvolatile ESP32 state. Use it only when a repeatable
first-pair test is intended and the user has explicitly authorized the change.
The firmware requires two exact, uppercase serial commands and limits the
confirmation to 10 seconds.

With the serial monitor connected:

1. Enter `CLEAR_BONDS` and press Enter.
2. Require `bond_clear_confirmation_required` with `"bonds_changed":false`.
3. Within 10 seconds, enter `CONFIRM_CLEAR_BONDS` and press Enter.
4. Require `bond_clear_completed` with `"bonds_changed":true` and
   `"restart_required":true`.
5. Reset the ESP32, power off the controller, and use the first-pair procedure
   with `SHARE` + `HOME`.

To abandon a pending request, enter `CANCEL_CLEAR_BONDS`. The resulting
`bond_clear_cancelled` record reports `"bonds_changed":false`. Doing nothing
for 10 seconds produces `bond_clear_expired`, also with
`"bonds_changed":false`. A late `CONFIRM_CLEAR_BONDS` is rejected and cannot
serve as evidence that bonds were cleared.

## Expected `BTDIAG` sequences

Raw ESP-IDF, Bluepad32, and BTstack lines can appear between these records. Use
the prefixed `BTDIAG` records as the stable evidence and compare their
`elapsed_ms` values to establish order.

Successful first pair or bonded reconnection:

```text
firmware_started
dependency_identity
bluetooth_ready
accepting_connections
waiting_for_controller (zero or more)
controller_connected
controller_first_input
controller_input (when the requested button changes the state)
```

The requested Cross press may be the first report, in which case
`controller_first_input` itself contains `pressed_buttons:"cross"`. Otherwise,
a later `controller_input` record must contain it.

No observed connection:

```text
accepting_connections
waiting_for_controller (periodically)
connection_timeout
```

Confirmed bond clearing:

```text
bond_clear_confirmation_required
bond_clear_completed
```

Cancelled or unconfirmed bond clearing ends in `bond_clear_cancelled` or
`bond_clear_expired` instead of `bond_clear_completed`.

## Timeout interpretation

- `waiting_for_controller` is periodic progress evidence, not a failure.
- `connection_timeout` means no controller was usable during the 75-second
  firmware window. New Bluetooth connections remain disabled after this event
  until the ESP32 is reset. It does not identify whether the cause was mode,
  timing, radio conditions, stored bonds, or controller compatibility.
- `bond_clear_expired` means the 10-second confirmation window ended. Stored
  bonds were not changed; issue a new `CLEAR_BONDS` request if clearing is still
  explicitly authorized.
- `bond_clear_confirmation_rejected` means confirmation arrived without a live
  request. Stored bonds were not changed.

## Troubleshooting by furthest evidenced stage

| Furthest evidence | What is established | Next checks |
| --- | --- | --- |
| Serial port will not open | No firmware conclusion is possible | Close every other monitor, verify the port with `./scripts/pio.sh device list`, reconnect the USB data cable, and install the PlatformIO udev rules if needed. |
| Serial opens, but no `firmware_started` | The selected port has not shown this lesson booting | Open the monitor before pressing `EN`/`RST`; confirm the active project and port; upload again if the expected boot record remains absent. |
| `firmware_started`, but no `bluetooth_ready` | This firmware started, but Bluetooth readiness was not evidenced | Preserve the raw stack output, reset once, and check power and board identity. Do not blame the controller yet. |
| `bluetooth_ready`, but no `accepting_connections` | Bluetooth reached the ready stage, but the connection window was not evidenced | Reset once and confirm the uploaded firmware is current; retain intervening raw logs for diagnosis. |
| `accepting_connections` or `waiting_for_controller` | The ESP32 is listening, but no usable controller connection has appeared | For first pair, start powered off and hold `SHARE` + `HOME` until the white light flashes. For reconnection, press `HOME` only. Keep the controller close and begin before the 75-second timeout. |
| `connection_timeout` | The bounded connection window ended without a usable connection | Reset the ESP32 before retrying. Verify the intended first-pair versus bonded-reconnection procedure; consider confirmation-gated bond clearing only with explicit authorization. |
| `controller_connected`, but no valid input | A controller connection occurred, but usable HID input is not yet proven | Press and hold Cross, then move an axis. Check for a quick `controller_disconnected` and preserve raw HID/stack logs. Connection alone is not a pass. |
| `controller_disconnected` before input | A connection was lost before success evidence | Charge the controller, reduce distance, reset both devices, and repeat the correct pairing mode. Treat protocol compatibility as unresolved. |
| Valid input with the requested Cross press | Connection and usable controller reports are proven | Mark that first-pair or reconnection session successful according to the procedure used. |
| `bond_clear_confirmation_required` only | A destructive operation is pending, but bonds remain unchanged | Confirm within 10 seconds only if still authorized, cancel explicitly, or allow expiration. |
| `bond_clear_cancelled`, `bond_clear_expired`, or confirmation rejected | The bond-clear operation did not run | Bonds remain unchanged. Begin a new two-step request only if clearing is still authorized. |
| `bond_clear_completed` | The firmware accepted the confirmed clear operation | Reset the ESP32 before beginning a fresh first-pair test. |

## Command summary

Run all commands from `platformio-esp32-lessons/02-bluetooth-motor-servo-control/`:

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
