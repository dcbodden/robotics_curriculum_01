## Purpose

Defines a repeatable ESP32-WROOM experiment and agent-assisted diagnostic process for establishing and explaining Bluetooth game-controller connectivity with the Mystiluck/Senze SZ-4015B in PS4-compatible mode.

## ADDED Requirements

### Requirement: Standalone ESP32 Bluetooth controller experiment
The repository SHALL provide an independently buildable PlatformIO project under `experiments/esp32-bluetooth-controller-baseline/` that targets the supported 30-pin ESP32-WROOM DevKit V1 and attempts to operate as a Bluetooth host for an SZ-4015B controller in PS4-compatible mode.

#### Scenario: Open the experiment by itself
- **WHEN** a contributor opens `experiments/esp32-bluetooth-controller-baseline/` as the active PlatformIO project
- **THEN** the project contains the configuration, source, dependency declarations, and instructions needed to build and upload without compiling an ESP32 lesson or robotics template

#### Scenario: Check the Bluetooth transport
- **WHEN** a contributor reviews the supported hardware and project configuration
- **THEN** the documentation identifies the original ESP32-WROOM as required because the PS4-compatible connection uses Bluetooth Classic BR/EDR

### Requirement: Structured serial diagnostic evidence
The experiment SHALL send versioned, machine-readable `BTDIAG` records at 115200 baud without suppressing useful underlying Bluetooth stack logs. The records SHALL identify firmware startup, Bluetooth readiness, availability for controller connections, controller connection and disconnection, receipt of the first valid controller input report, elapsed waiting status, and a bounded connection timeout.

#### Scenario: Observe a successful connection
- **WHEN** the controller connects and sends a valid input report
- **THEN** the serial stream contains ordered `BTDIAG` evidence that the firmware started, Bluetooth became ready, a controller connected, and controller input was received

#### Scenario: Observe no connection
- **WHEN** no controller becomes usable during the documented connection window
- **THEN** the serial stream emits waiting evidence followed by a timeout record instead of claiming either success or a specific unobserved Bluetooth failure

#### Scenario: Preserve lower-level evidence
- **WHEN** the Bluetooth dependency emits diagnostic output during discovery, authentication, connection, or disconnection
- **THEN** that output remains visible beside the distinguishable `BTDIAG` records for later investigation

### Requirement: First-pair and reconnection procedures
The experiment documentation SHALL provide separate procedures for first-pair testing and bonded reconnection. The first-pair procedure SHALL start with the controller powered off, direct the user to hold `SHARE` and `HOME` for approximately three seconds until the white channel light flashes, and treat receipt of a valid input report as the positive success criterion. The reconnection procedure SHALL test a subsequent connection initiated with `HOME` without repeating first-pair mode.

#### Scenario: Complete a first-pair test
- **WHEN** the user follows the first-pair procedure and presses the requested controller button after connection
- **THEN** the result is considered successful only if the serial evidence includes both controller connection and a corresponding valid input report

#### Scenario: Complete a bonded-reconnection test
- **WHEN** a previously paired controller is switched off or disconnected and subsequently restarted with `HOME`
- **THEN** the procedure checks for a new connection event and a new input report without requiring `SHARE` and `HOME`

### Requirement: Confirmation-gated bond clearing
The experiment SHALL expose a serial operation for clearing Bluetooth bonding information, but SHALL require an explicit confirmation step before changing the stored bonding state.

#### Scenario: Request bond clearing without confirmation
- **WHEN** a user or diagnostic agent requests that bonds be cleared but does not complete the documented confirmation step
- **THEN** the firmware leaves stored bonds unchanged and reports that confirmation is required or expired

#### Scenario: Confirm bond clearing
- **WHEN** the user explicitly authorizes the operation and the documented confirmation is received within its allowed interval
- **THEN** the firmware clears the stored Bluetooth bonds and reports completion so a repeatable first-pair test can begin

### Requirement: Repository-local serial diagnostic skill
The repository SHALL provide an agent skill for conducting a bounded diagnostic session against this experiment. The skill SHALL verify the selected project, PlatformIO environment, serial port, and expected boot evidence; prompt the user for controller actions at the appropriate time; monitor the serial stream; and classify the result without treating silence as proof of a controller defect.

#### Scenario: Diagnose a successful session
- **WHEN** the skill observes Bluetooth readiness, controller connection, and valid input evidence
- **THEN** it reports a successful controller session and distinguishes first-pair success from bonded reconnection when the session type is known

#### Scenario: Diagnose an unsuccessful session
- **WHEN** the bounded session ends without valid input evidence
- **THEN** the skill reports the furthest evidenced stage and suggests checks appropriate to missing boot output, Bluetooth initialization, controller discovery or pairing, connection without input, or premature disconnection

#### Scenario: Avoid monitoring the wrong project or port
- **WHEN** the active PlatformIO project, configured environment, detected serial port, or expected firmware boot evidence does not match the experiment
- **THEN** the skill reports the mismatch and does not attribute the result to controller compatibility

### Requirement: Reproducible diagnostic records
Each diagnostic run SHALL be capable of producing a timestamped local transcript and summary that identify the relevant project, board, framework, Bluetooth dependency, controller model and mode, serial port, observed milestones, and final classification. Generated records SHALL remain outside tracked source by default and SHALL distinguish automated build evidence from user-confirmed physical hardware evidence.

#### Scenario: Save a diagnostic run
- **WHEN** the user requests or accepts transcript capture
- **THEN** the skill stores the raw serial session and summarized classification in the documented ignored diagnostics location

#### Scenario: Report verification boundaries
- **WHEN** the experiment builds but has not been physically exercised with the SZ-4015B
- **THEN** project records describe the build as verified and the Bluetooth connection and input behavior as awaiting physical verification
