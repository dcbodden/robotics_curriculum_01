## Why

The curriculum needs a repeatable way to establish whether the supported ESP32-WROOM board can discover, connect to, and receive input from the Mystiluck/Senze SZ-4015B controller in PS4-compatible mode. A structured serial diagnostic process will make connection attempts and failure stages observable instead of reducing hardware testing to an unexplained pass or fail.

## What Changes

- Add a self-contained PlatformIO experiment under `experiments/` for connecting the 30-pin ESP32-WROOM DevKit V1 to an SZ-4015B in PS4-compatible mode using Bluepad32.
- Emit versioned, machine-readable serial diagnostic events for firmware startup, Bluetooth readiness, connection availability, controller connection and disconnection, input reception, and bounded timeouts while retaining useful underlying stack logs.
- Document first-pair and bonded-reconnection procedures, including the controller's `SHARE` + `HOME` pairing action and positive success criteria based on receiving an input report.
- Add a repository-local agent skill that conducts a bounded serial-monitor session, prompts the user for physical actions, classifies the observed result, and saves a diagnostic transcript and summary.
- Provide an explicit, confirmation-gated way to clear stored Bluetooth bonds for repeatable first-pair testing.

## Capabilities

### New Capabilities

- `esp32-bluetooth-controller-experiment`: Defines the standalone ESP32 Bluetooth-controller experiment, observable diagnostic protocol, pairing and reconnection tests, and agent-assisted serial diagnosis.

### Modified Capabilities

None.

## Impact

- Adds a new project below `experiments/` and a new repository-local skill below `.agents/skills/`.
- Introduces pinned Bluepad32, BTstack, ESP-IDF/Arduino, and PlatformIO platform dependencies based on the supported upstream integration route.
- Targets the original ESP32-WROOM/DOIT DevKit V1 because PS4-compatible controllers use Bluetooth Classic BR/EDR.
- Adds serial-port monitoring and local diagnostic transcript handling; generated transcripts containing device addresses or host details will remain untracked.
