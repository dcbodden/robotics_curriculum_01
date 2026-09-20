---
name: esp32-controller-diagnostics
description: Conduct bounded serial diagnostics for the repository's ESP32-WROOM Bluetooth controller baseline, including SZ-4015B first pairing, bonded reconnection, and explicitly authorized bond clearing. Use for observing or explaining this experiment, not for generic ESP32 serial monitoring.
---

# ESP32 Controller Diagnostics

Diagnose `experiments/esp32-bluetooth-controller-baseline` from structured
`BTDIAG` evidence. Treat raw ESP-IDF, Bluepad32, and BTstack output as supporting
context only. Never infer controller failure from silence or a timeout.

The helper requires Python 3 with pyserial and the experiment-local PlatformIO
wrapper initialized by the experiment setup instructions.

## Choose the path

- **Listen (default):** inspect an already-flashed board. Do not build, upload,
  or clear bonds merely because the user asks for diagnosis.
- **Flash and listen:** use only when the user explicitly asks to flash/upload.
  Close any monitor first, build and upload through the experiment-local
  `scripts/pio.sh`, then listen.
- **Clear bonds:** use only after the user explicitly authorizes loss of the
  ESP32's stored controller bonds. This is separate from ordinary diagnosis.

Work from the repository root unless a command below changes directory. The
only supported PlatformIO environment is `esp32doit-devkit-v1`; do not substitute
a global PlatformIO installation or another repository project.

## Listen workflow

1. Ask whether this is a `first-pair`, `reconnection`, or `unknown` session if
   the request does not establish it. Ask for the serial port only if exactly
   one cannot be selected safely.
2. Start the bounded helper. It validates the exact project, environment, and
   port before opening serial, streams raw output, extracts valid records, and
   closes serial on success, timeout, interruption, or error:

   ```bash
   python3 .agents/skills/esp32-controller-diagnostics/scripts/monitor.py \
     --session-type first-pair \
     --port /dev/ttyUSB0
   ```

   Omit `--port` only when PlatformIO reports exactly one serial device. The
   default 90-second bound covers the firmware's 75-second connection window.
   Add one `--user-observation "..."` per fact the user reports; do not turn
   such reports into automated evidence.
3. Once the helper reports that the port is open, ask the user to press and
   release `EN`/`RST`. If no `firmware_started` evidence appears, allow one
   deliberate reset retry before accepting the bounded classification.
4. After `accepting_connections`:
   - First pair: start with the SZ-4015B off, then hold `SHARE` + `HOME` for
     about three seconds until the white channel light flashes.
   - Reconnection: preserve the existing bond and press `HOME` once. Do not use
     `SHARE` + `HOME`.
5. After `controller_connected`, ask the user to press and hold Cross for about
   half a second and release it. A pass requires a subsequent valid input record
   whose `pressed_buttons` includes `cross`.
6. Poll the running command often enough to relay milestones and prompts. Do
   not leave it running after the invoking turn. Report the summary path and
   explain the furthest evidenced stage.

The helper writes a raw transcript, extracted JSONL records, and Markdown
summary beneath the ignored
`experiments/esp32-bluetooth-controller-baseline/.diagnostics/` directory.

## Flash-and-listen workflow

Only enter this path after an explicit upload/flash request.

1. Stop any process using the serial port.
2. From `experiments/esp32-bluetooth-controller-baseline`, run:

   ```bash
   ./scripts/pio.sh run --environment esp32doit-devkit-v1
   ./scripts/pio.sh run --environment esp32doit-devkit-v1 \
     --target upload --upload-port /dev/ttyUSB0
   ```

3. Do not claim Bluetooth success from compilation or upload. Start the listen
   workflow and pass concise observed command results with repeatable
   `--build-evidence` options so the saved report distinguishes them from
   serial evidence and user-reported physical observations.

## Classification and next action

The helper classifies only structured records from this experiment:

- `serial_unavailable`: close other monitors, check the cable and udev access,
  then re-run `./scripts/pio.sh device list`.
- `missing_experiment_boot`: reset once while listening; verify the port and
  uploaded project. Do not attribute this to the controller.
- `bluetooth_initialization_incomplete`: preserve the raw logs, confirm the
  original ESP32-WROOM board, power, and current firmware.
- `waiting_for_controller`: repeat the correct controller action while the
  connection window remains open.
- `pairing_timeout`: reset the ESP32 to open a new 75-second window; the timeout
  does not identify mode, bond, radio, or compatibility as the cause.
- `connected_without_input`: request Cross, then axis movement; a connection
  alone is not a pass.
- `premature_disconnection`: charge and move the controller closer, then retry
  the correct session mode while preserving raw logs.
- `successful_input`: connection plus the requested valid Cross input is proven.
  Label it first-pair or reconnection success only when the session type is known.

## Explicitly authorized bond clearing

Bond clearing mutates nonvolatile state. Restate the effect and obtain explicit
authorization immediately before doing it. Use an interactive 115200-baud
monitor on the validated experiment port, then:

1. Send `CLEAR_BONDS` followed by Enter.
2. Require `bond_clear_confirmation_required` with `bonds_changed:false`.
3. While the authorization still applies, send `CONFIRM_CLEAR_BONDS` within ten
   seconds.
4. Require `bond_clear_completed` with `bonds_changed:true` and
   `restart_required:true`, then reset and follow the first-pair workflow.

Send `CANCEL_CLEAR_BONDS` to abandon a pending request. Expiration, cancellation,
or rejection means bonds did not change. Never claim completion without the
structured completion record.
