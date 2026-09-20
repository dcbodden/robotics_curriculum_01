## 1. Reproducible Project Foundation

- [x] 1.1 Record the selected upstream Bluepad32 template, PlatformIO platform, Arduino, ESP-IDF, BTstack, and Bluepad32 revisions together with their source and license obligations.
- [x] 1.2 Create `experiments/esp32-bluetooth-controller-baseline/` with the minimal ESP-IDF/Arduino/Bluepad32 component structure, pinned dependencies, and a PlatformIO environment for the supported 30-pin ESP32-WROOM DevKit V1 at 115200 baud.
- [x] 1.3 Add project-local ignore rules for build products and generated diagnostic transcripts, then confirm the minimal project resolves dependencies and builds without compiling another repository project.

## 2. Observable Bluetooth Firmware

- [x] 2.1 Add a small, versioned `BTDIAG` JSON-line emitter and report firmware startup, build/dependency identity, monotonic elapsed time, and Bluetooth readiness.
- [x] 2.2 Initialize Bluepad32 for incoming PS4-compatible controller connections and emit accepting-connections plus periodic waiting and bounded-timeout evidence while preserving upstream stack logs.
- [x] 2.3 Report controller connection, available identity properties, and disconnection through structured events without hard-coding an expected SZ-4015B name, VID, or PID.
- [x] 2.4 Process controller reports and emit definitive first-input evidence plus readable, rate-limited button and axis observations suitable for interactive testing.
- [x] 2.5 Implement the two-step, time-limited serial confirmation exchange for clearing stored Bluetooth bonds and report cancellation, expiration, and successful completion.

## 3. Experiment Instructions

- [x] 3.1 Write the experiment README with supported hardware, Bluetooth Classic constraint, dependency setup, active-project check, build, upload, monitor, and reset instructions.
- [x] 3.2 Document a first-pair procedure for the SZ-4015B using `SHARE` + `HOME`, a requested button press, and connection-plus-input success criteria.
- [x] 3.3 Document the bonded-reconnection procedure, confirmation-gated bond clearing, expected `BTDIAG` sequence, timeout interpretation, and a troubleshooting matrix based on the furthest evidenced stage.
- [x] 3.4 Add the standalone Bluetooth controller experiment to `experiments/README.md` without presenting it as a numbered curriculum lesson.

## 4. Agent-Assisted Serial Diagnosis

- [x] 4.1 Create the repository-local `esp32-controller-diagnostics` skill using the repository's skill conventions, with separate default listen and explicitly requested flash-and-listen paths.
- [x] 4.2 Add a bounded monitor helper that validates the experiment path, PlatformIO environment, and serial port; captures raw output; extracts valid `BTDIAG` records; and closes the monitor cleanly.
- [x] 4.3 Implement stage-based session classification for missing serial access, missing experiment boot evidence, Bluetooth initialization, waiting or pairing timeout, connected-without-input, premature disconnection, and successful input.
- [x] 4.4 Add interactive skill guidance for board reset, `SHARE` + `HOME` first pairing, `HOME` reconnection, button confirmation, serial-port conflicts, and explicitly authorized bond clearing.
- [x] 4.5 Generate timestamped ignored transcripts and summaries containing reproducibility metadata, observed milestones, final classification, and clear separation between automated evidence and user-confirmed physical observations.

## 5. Verification

- [ ] 5.1 Add deterministic parser or fixture checks covering every diagnostic classification and malformed or interleaved non-`BTDIAG` serial lines.
- [ ] 5.2 Perform a clean PlatformIO build of the experiment and record exact dependency versions, memory usage, warnings, and the fact that compilation alone does not verify Bluetooth behavior.
- [ ] 5.3 Conduct and record an SZ-4015B first-pair diagnostic session on the supported ESP32-WROOM, requiring connection and valid button-input evidence before marking the physical test successful.
- [ ] 5.4 Conduct and record a bonded-reconnection session after a disconnect or power cycle, and verify that the diagnostic skill correctly summarizes the captured evidence.
