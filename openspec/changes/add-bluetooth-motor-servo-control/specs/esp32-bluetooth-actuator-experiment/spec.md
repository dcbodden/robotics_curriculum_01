## Purpose

Defines a repeatable ESP32-WROOM bench lesson that converts input from a PS4-compatible Bluetooth controller into safe bidirectional motor and servo commands while preserving diagnostic and physical evidence.

## ADDED Requirements

### Requirement: Standalone Bluetooth actuator lesson
The repository SHALL provide numbered Lesson 02 as an independently buildable PlatformIO project under `platformio-esp32-lessons/02-bluetooth-motor-servo-control/` for the supported 30-pin ESP32-WROOM DevKit V1, an SZ-4015B controller in PS4-compatible mode, one HW-627 DRV8833 motor-driver module, one Elegoo Arduino Car V1-style 48:1 geared brushed motor, and one hobby servo. The lesson SHALL preserve the pinned Bluetooth Classic dependency and project-local tooling approach established by the standalone controller baseline experiment without changing that baseline project.

#### Scenario: Open the actuator lesson independently
- **WHEN** a contributor opens Lesson 02 as the active PlatformIO project
- **THEN** its configuration, source, dependency declarations, wrapper scripts, and instructions are sufficient to build and upload it without compiling another lesson or experiment

#### Scenario: Browse the ESP32 lesson collection
- **WHEN** a learner or teacher opens the ESP32 lesson collection index
- **THEN** Lesson 02 is listed after Lesson 01 with its Bluetooth controller, bidirectional motor, servo, and bench-only scope identified

#### Scenario: Keep the diagnostic baseline intact
- **WHEN** the new lesson is added
- **THEN** `experiments/esp32-bluetooth-controller-baseline/` remains independently buildable and retains its controller-only diagnostic behavior

### Requirement: Wireless motor and servo command mapping
The lesson SHALL map the controller's left-stick vertical axis to a signed motor command spanning reverse, zero, and forward, and its left-stick horizontal axis to a bounded servo-position command. Each axis SHALL have a documented center dead zone, the released stick SHALL produce zero motor command and a centered servo command, and the mappings SHALL clamp out-of-range inputs.

#### Scenario: Move the stick forward and backward
- **WHEN** an armed controller reports vertical-axis input beyond the center dead zone in either direction
- **THEN** the lesson produces a proportional motor magnitude with opposite signed directions for forward and reverse

#### Scenario: Steer left and right
- **WHEN** an armed controller reports horizontal-axis input beyond the center dead zone
- **THEN** the lesson produces a proportional servo command within the documented safe pulse limits

#### Scenario: Release the stick
- **WHEN** both left-stick axes return to their center dead zones
- **THEN** the motor command is zero and the servo command is centered

#### Scenario: Receive an extreme or out-of-range axis value
- **WHEN** a reported axis value reaches or exceeds the supported input boundary
- **THEN** the resulting motor and servo commands remain within their configured output limits

### Requirement: DRV8833 bidirectional output behavior
The lesson SHALL drive one HW-627 DRV8833 H-bridge channel so that a positive command applies PWM in one direction, a negative command applies PWM in the opposite direction, and zero drives both bridge inputs low for coast behavior. A requested direction reversal SHALL pass through a zero-output interval before the opposite direction is energized.

#### Scenario: Command forward motion
- **WHEN** the active signed motor command is positive
- **THEN** one bridge input receives the proportional PWM command while the opposite bridge input remains low

#### Scenario: Command reverse motion
- **WHEN** the active signed motor command is negative
- **THEN** the opposite bridge input receives the proportional PWM command while the first bridge input remains low

#### Scenario: Reverse direction
- **WHEN** a nonzero motor command changes sign
- **THEN** both bridge inputs remain low for at least one control update before PWM is applied in the new direction

#### Scenario: Command motor stop
- **WHEN** the active motor command becomes zero
- **THEN** both bridge inputs are low and the documentation explains that the motor can coast mechanically after electrical drive is removed

### Requirement: Explicit arming and loss-of-control failsafe
The lesson SHALL initialize with motor output disabled, require a connected controller to report neutral controls before accepting an explicit arm action, and require fresh controller reports while armed. Disarming, controller disconnection, invalid controller state, or absence of a fresh report for no more than 500 milliseconds SHALL force motor coast and the configured safe servo position. Reconnection SHALL require a new neutral report and arm action rather than restoring a prior command.

#### Scenario: Start or connect with a displaced stick
- **WHEN** firmware starts or a controller connects while a mapped control is outside its neutral dead zone
- **THEN** the lesson remains disarmed and keeps the motor output at zero

#### Scenario: Arm from neutral
- **WHEN** a connected controller has supplied a fresh neutral report and the user performs the documented arm action
- **THEN** subsequent valid stick reports may control the motor and servo

#### Scenario: Lose fresh controller input
- **WHEN** no fresh valid controller report is received within the documented watchdog interval while armed
- **THEN** the lesson enters its failsafe state within 500 milliseconds, drives the motor command to zero, and commands the configured safe servo position

#### Scenario: Disconnect and reconnect
- **WHEN** an armed controller disconnects and later reconnects
- **THEN** the motor remains at zero until the reconnected controller reports neutral and the user explicitly arms it again

#### Scenario: Disarm explicitly
- **WHEN** the user performs the documented disarm action
- **THEN** the motor immediately receives a zero command and further stick motion cannot drive it until the arming conditions are satisfied again

### Requirement: Bench-only lesson guidance
The lesson SHALL provide student-facing staged missions and teacher guidance using USB power for the ESP32 and an appropriately rated regulated bench supply for the motor and servo, with the positive supplies kept separate and all signal grounds sharing a defined reference. It SHALL require secured unloaded actuators, power-off wiring changes, initial command checks before actuator power is enabled, bounded low-power first motion, and external actuator power removal before teardown.

#### Scenario: Prepare the bench circuit
- **WHEN** a user follows the wiring procedure
- **THEN** the ESP32 is USB-powered, the HW-627 and servo receive externally regulated actuator power through separate branches, grounds are common, and external positive is not connected to the ESP32 5 V or 3.3 V rail

#### Scenario: Begin physical testing
- **WHEN** the user is ready to enable actuator power
- **THEN** the procedure first requires verified neutral commands, a secured bare geared motor without a wheel or propeller, an unloaded secured servo, clear motion areas, checked polarity, and a suitable supply current limit

#### Scenario: Encounter abnormal operation
- **WHEN** the setup exhibits unexpected movement, a persistent stall, excessive heat, smoke, odor, resets, severe servo chatter, or unstable supply voltage
- **THEN** the procedure directs the user to remove external actuator power and USB and not reconnect until the setup has been inspected

#### Scenario: Preserve later rover scope
- **WHEN** a learner or teacher reviews the lesson boundary
- **THEN** batteries, DC/DC regulators, chassis construction, wheels, steering linkage, and complete rover wiring are identified as later work rather than requirements of this bench lesson

### Requirement: Power-integrity and inductive-load guidance
The lesson documentation SHALL distinguish chip-level bypassing, local bulk energy storage, motor brush-noise suppression, and H-bridge inductive-current recirculation. It SHALL direct the user to verify the components actually populated on the HW-627 rather than assume an unverified module schematic, and SHALL treat capacitor values as an initial configuration subject to measurement under simultaneous motor and servo activity.

#### Scenario: Add initial local decoupling
- **WHEN** the bench circuit is prepared
- **THEN** the documented starting point includes a non-polarized 100 nF ceramic capacitor directly across the motor terminals, a 100 nF ceramic capacitor near the driver's supply input, and approximately 100 to 220 microfarads of suitably voltage-rated bulk capacitance near the HW-627 supply input, while accounting for capacitors already fitted to the module

#### Scenario: Address servo or shared-supply transients
- **WHEN** measured supply behavior or actuator symptoms show transient disturbance
- **THEN** the documentation permits measurement-driven increases such as approximately 470 microfarads near the driver, 100 to 470 microfarads near the servo branch, or 470 to 1000 microfarads at the actuator distribution point without presenting capacitance as a substitute for adequate supply capacity and wiring

#### Scenario: Consider a motor flywheel diode
- **WHEN** a user considers adding a conventional single diode across the reversible motor
- **THEN** the documentation prohibits that connection because motor polarity reverses and explains that the DRV8833 H-bridge provides the required recirculation paths

#### Scenario: Size the supply and driver
- **WHEN** the motor is described as drawing approximately 300 milliamperes while running
- **THEN** the documentation treats that value as provisional running current, requires startup and stall demand to be considered without deliberately stalling the motor, and avoids claiming suitability solely from the driver's headline peak-current rating

### Requirement: Observable control and verification evidence
The lesson SHALL retain distinguishable Bluetooth lifecycle diagnostics and add bounded-rate actuator-control records that identify connection, armed or failsafe state, controller inputs, mapped motor direction and magnitude, servo command, and the cause of any safety transition. It SHALL provide host checks for pure mapping and safety behavior, a reproducible firmware build record, and a staged physical verification record that does not equate successful compilation with successful actuator operation.

#### Scenario: Observe normal wireless control
- **WHEN** an armed controller changes the mapped stick axes
- **THEN** serial output reports the corresponding bounded motor and servo commands at a rate that remains readable without blocking control processing

#### Scenario: Observe a safety transition
- **WHEN** the lesson disarms or enters failsafe
- **THEN** serial evidence identifies the reason and shows a zero motor command and the configured safe servo command

#### Scenario: Verify pure control behavior
- **WHEN** host checks run without ESP32, Bluetooth, or actuator hardware
- **THEN** they cover dead-zone boundaries, endpoints, clamping, forward and reverse mapping, zero-before-reverse behavior, arming prerequisites, watchdog expiry, disconnect handling, and reconnection behavior

#### Scenario: Record verification boundaries
- **WHEN** the project builds but has not completed the physical bench procedure
- **THEN** its verification record marks the build and host checks separately from Bluetooth connection, electrical measurements, and observed motor and servo motion
