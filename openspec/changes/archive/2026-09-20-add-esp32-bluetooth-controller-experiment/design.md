## Context

The existing ESP32 lesson uses PlatformIO with the Arduino framework and the 30-pin DOIT ESP32 DevKit V1. This experiment is intentionally outside the numbered lesson sequence, but it should reuse the same physical board and 115200-baud serial convention.

PS4-compatible gamepads use Bluetooth Classic BR/EDR, which the original ESP32 supports but ESP32-S3/C3-family boards do not. Bluepad32 documents DualShock 4 support and limited clone compatibility, but the SZ-4015B is not explicitly listed; discovering its actual compatibility is therefore an experiment outcome rather than an assumption. Bluepad32's supported PlatformIO integration combines ESP-IDF, Arduino as a component, BTstack, and Bluepad32 rather than adding Bluepad32 as an ordinary Arduino `lib_deps` entry.

The experiment and the diagnostic skill cross firmware, build configuration, serial-port ownership, physical user actions, and agent interpretation. See `proposal.md` for motivation and the capability spec for observable requirements.

## Goals / Non-Goals

**Goals:**

- Produce unambiguous evidence for each observable stage between firmware boot and the first valid controller input.
- Make the first-pair and bonded-reconnection tests repeatable on the supported ESP32-WROOM board.
- Let an agent conduct and interpret a bounded serial session while keeping the raw evidence available to the user.
- Pin and document the nonstandard Bluetooth build dependencies closely enough that another contributor can reproduce the build.

**Non-Goals:**

- Support PS3 mode, BLE-only ESP32 variants, every SZ-4015B host mode, or arbitrary gamepads.
- Turn the experiment into a numbered student lesson or integrate controller input with motors or other actuators.
- Promise a specific low-level error reason when the Bluetooth stack exposes only silence, disappearance, or a timeout.
- Run a permanent background listener after the invoking agent turn ends.

## Decisions

### Use Bluepad32 through its supported ESP-IDF/Arduino PlatformIO architecture

Start from the current upstream ESP-IDF + Arduino + Bluepad32 template structure, reduce it to the experiment, pin the platform and component revisions, and preserve required license notices. Keep the existing board identity where supported and document any unavoidable mapping between `esp32doit-devkit-v1` and the upstream template's generic `esp32dev` environment.

This is preferred to the PS4-only Arduino libraries because Bluepad32 has an actively documented ESP32 host architecture, explicitly supports DualShock 4 and controller properties, and provides lower-level diagnostic output. A plain `BluetoothSerial` sketch is unsuitable because SPP is not a gamepad HID host. A raw ESP-IDF HID host would offer maximum control but would make the baseline substantially larger and less comparable with the repository's Arduino-based ESP32 work.

### Treat clone compatibility as the experiment result

The firmware will request PS4-compatible controller behavior but will not hard-code an expected controller name, VID, PID, or exact descriptor. It will report properties that Bluepad32 actually supplies and declare application-level success only after a connection callback and a valid input report.

This avoids misclassifying clones that advertise as either `Wireless Controller` or `PLAYSTATION (4) Controller`. It also prevents a transient radio or HID-channel connection from being recorded as full success.

### Use prefixed JSON records as the stable diagnostic interface

Application evidence will use one record per line in the form `BTDIAG {json}` with a protocol version, monotonic elapsed time, event name, and event-specific fields. Raw ESP-IDF, Bluepad32, and BTstack logs may be interleaved, but only lines carrying the prefix are part of the stable machine-readable contract.

JSON makes optional controller properties and explicit values safer to parse than free-form prose. A prefix makes it possible for the skill to extract records without depending on unstable upstream log wording. The firmware will serialize records without adding a general JSON library unless implementation shows that correct escaping cannot remain small and auditable.

### Model the diagnostic session as evidenced stages

The skill will classify the furthest observed stage:

```text
serial unavailable
        |
        v
firmware boot -> Bluetooth ready -> accepting connections
                                      |
                                      v
                              controller connected
                                      |
                         +------------+------------+
                         v                         v
                  valid input received     disconnect/timeout
                         |
                         v
                       PASS
```

Absence of a later stage is not evidence for a particular root cause. Low-level logs may support a more specific explanation, but the skill will label such conclusions as interpretations rather than structured firmware facts.

### Separate listening from mutation

The skill's default path will validate context and monitor an already-flashed board. Uploading will be an explicit path, and the monitor will be closed before any upload because the serial port is exclusive. Clearing bonds will require both user authorization and a two-step, time-limited firmware confirmation exchange before nonvolatile pairing state changes.

This separation reduces accidental uploads and lost pairings while still supporting a repeatable first-pair test.

### Use bounded sessions and ignored evidence files

A diagnostic run will wait long enough to cover the controller's approximately 60-second pairing window, emit periodic waiting evidence, and then finish with a clear classification. The skill will poll an interactive PlatformIO monitor session often enough to keep the user informed and will always terminate or hand back the monitor cleanly.

Transcripts and summaries will be written beneath an experiment-local ignored diagnostics directory. Reports will record versions and observed milestones, but documentation examples will use redacted Bluetooth addresses and generated evidence will not be committed by default.

## Risks / Trade-offs

- **The SZ-4015B descriptor may not be accepted by Bluepad32 despite its PS4 compatibility claim** → Preserve raw stack logs, report the furthest evidenced stage, and treat unsupported-controller investigation as a truthful experiment result rather than weakening success criteria.
- **The current Bluepad32 PlatformIO route depends on a third-party PlatformIO platform fork and several pinned components** → Pin known-compatible revisions, document their origin and licenses, and verify from a clean dependency state without deleting the user's global PlatformIO installation.
- **Upstream debug wording can change** → Base automated classification only on versioned `BTDIAG` records; expose raw logs as supporting evidence.
- **Serial output can be noisy or lose early boot lines** → Open the monitor before a deliberate board reset when possible, emit a recognizable startup record, and allow the skill to ask for one retry before classifying missing firmware evidence.
- **A monitor can block upload or another application can own the port** → Detect port-open failures, explain exclusive ownership, and stop the monitor before an explicitly requested upload.
- **Device addresses and host paths can leak into committed artifacts** → Ignore generated diagnostic records by default and redact identifying values from maintained documentation examples.
- **Bond clearing changes nonvolatile state** → Require explicit user authorization plus a short-lived second confirmation and report whether the operation completed.

## Migration Plan

This is additive and requires no migration of existing lessons. Implementation will add the experiment and skill, verify a clean build, then leave physical pairing and input verification explicitly pending until performed with the SZ-4015B. Removing the experiment later consists of deleting its self-contained directory and diagnostic skill; it does not alter the existing ESP32 lesson project.
