## Purpose

Defines a staged, measurable, and safely bounded path from the ESP32 Bluetooth actuator bench lesson to a protected 3S-powered two-motor electrical assembly with a later independently regulated camera load.

## ADDED Requirements

### Requirement: Three-branch protected power architecture
The follow-on activity SHALL document a protected 3S 18650 source with cell balancing, overcharge, undervoltage, overcurrent, and short-circuit protection; a pack-side fuse and reachable physical cutoff; and a star distribution point feeding exactly three LM2596 branches. The first branch SHALL provide 5.0 V to the Bluetooth ESP32-WROOM, the second SHALL provide an adjustable 4.0-to-4.5 V motor rail to one HW-627/DRV8833, and the third SHALL remain disconnected until phase 2 and then provide 5.0 V to the ESP32-S3 camera board. Positive outputs of the converters SHALL remain separate and all control circuits SHALL share a defined low-impedance ground reference.

#### Scenario: Inspect the complete architecture
- **WHEN** a learner or teacher reviews the block diagram and wiring table
- **THEN** the protected 3S source, fuse, cutoff, star distribution, three non-paralleled LM2596 outputs, two phase-1 loads, phase-2 camera load, and common ground reference are unambiguous

#### Scenario: Prepare only phase 1
- **WHEN** the two-motor and Bluetooth integration is prepared before camera qualification
- **THEN** only the Bluetooth and motor converters are connected to loads and the third converter and camera branch remain electrically disconnected

### Requirement: Phase-1 Bluetooth and two-motor integration
Phase 1 SHALL power the supported ESP32-WROOM DevKit through its 5 V input and SHALL power both secured, unloaded TT motors from one inspected HW-627/DRV8833 at an initial measured motor-rail target between 4.0 and 4.5 V, with one motor on each H-bridge. It SHALL preserve the existing neutral-before-arm, explicit-disarm, report-freshness, disconnect, reversal, and coast safeguards while providing only the additional control behavior needed to exercise both channels for power validation. It SHALL NOT characterize this test as independent differential-drive control or authorize wheels, chassis motion, deliberate stalls, or restrained-shaft tests.

#### Scenario: Exercise each driver channel separately
- **WHEN** the phase-1 bench procedure commands the secured motors one at a time
- **THEN** each motor starts, runs, reverses, and coasts through its own H-bridge without the other motor being energized and without bypassing the existing arming and failsafe policy

#### Scenario: Exercise both motors together
- **WHEN** the individual-channel checks have passed and the procedure commands both secured motors simultaneously at the documented conservative command
- **THEN** both motors operate without an ESP32 reset, driver fault indication, persistent non-turning energized motor, severe rail droop, or excessive heating

#### Scenario: Encounter an abnormal motor condition
- **WHEN** either motor fails to turn while energized, motion is unexpected, a rail becomes unstable, a part overheats, or a fault indication appears
- **THEN** the procedure requires immediate disarm and physical power removal and prohibits increasing the command or repeating the test until the cause is understood

### Requirement: Voltage verification before load connection
The activity SHALL require DC-voltage and polarity checks with a multimeter before any load is connected. The protected 3S output SHALL be checked against the documented safe pack range, each enabled LM2596 SHALL be adjusted and recorded at its output terminals, and each output SHALL be rechecked at the load terminals after connection. The 5 V rails SHALL remain within the documented board-input tolerance and the motor rail SHALL remain between 4.0 and 4.5 V during the applicable acceptance checks.

#### Scenario: Verify the battery source
- **WHEN** the protected pack is connected to the fused distribution input with all LM2596 load outputs disconnected
- **THEN** the recorded pack voltage and polarity fall within the documented safe range before the cutoff is allowed to energize the converters

#### Scenario: Adjust a converter before connecting its load
- **WHEN** an LM2596 is commissioned or its adjustment could have been disturbed
- **THEN** its unloaded output voltage and polarity are measured directly at the converter output and confirmed for the intended 5.0 V or 4.0-to-4.5 V branch before that load is connected

#### Scenario: Recheck voltage under load
- **WHEN** a phase-1 load is energized separately or as part of the integrated assembly
- **THEN** the procedure records voltage at both the relevant converter and load terminals and treats out-of-range voltage or material wiring drop as a failed gate

### Requirement: Safe and attributable current measurements
The activity SHALL provide a current-measurement procedure that requires a suitably fused meter, correct lead jack and range, an in-series connection made only while power is removed, and restoration of the meter lead after each measurement. It SHALL prohibit placing a meter configured for current directly across a battery, converter output, or load. Every recorded current SHALL identify the measurement point, rail voltage, operating state, and whether it is converter-input, converter-output, or whole-pack current.

#### Scenario: Prepare a current measurement
- **WHEN** the learner changes from voltage measurement to current measurement
- **THEN** all power is removed before the circuit is opened and the meter is inserted in series using a fused range that can tolerate the expected startup transient

#### Scenario: Avoid a current-range short circuit
- **WHEN** the meter lead is in a current jack or the selector is on a current range
- **THEN** the instructions explicitly prohibit probing across the 3S source or any voltage rail and require the lead and selector to be restored before later voltage checks

### Requirement: Phase-1 separate-load and integrated evidence
Phase 1 SHALL record the Bluetooth ESP32 branch current separately with Bluetooth active and no motor branch energized. It SHALL then record the DRV8833 motor-branch current with the ESP32 supplied independently for control, including driver idle, each motor starting and running separately, and both motors starting and running together without deliberate stall. After separate-load checks pass, it SHALL record whole-pack current and relevant rail voltages for the integrated battery-powered assembly at safe idle, Bluetooth-connected idle, individual-motor operation, and simultaneous-motor operation.

#### Scenario: Measure the Bluetooth ESP32 separately
- **WHEN** only the 5 V Bluetooth branch is powered from its LM2596 and the controller is connected and reporting
- **THEN** the record contains branch-output voltage, branch-output current, operating state, and any reset or connection instability

#### Scenario: Measure the motor branch separately
- **WHEN** the driver and secured motors are exercised with the motor LM2596 as the only actuator load
- **THEN** the record distinguishes driver idle, motor A startup and running, motor B startup and running, and simultaneous startup and running current at the measured motor-rail voltage

#### Scenario: Measure the integrated phase-1 assembly
- **WHEN** both phase-1 LM2596 branches are supplied by the protected 3S pack
- **THEN** the record identifies whole-pack voltage and current plus both load-terminal voltages for idle and motor-operation states and notes observed resets, Bluetooth interruption, driver faults, voltage droop, and component heating

### Requirement: Phase gate before camera integration
Phase 2 SHALL remain blocked until phase 1 has a completed wiring inspection, in-range source and rail voltages, attributable current records, successful separate and integrated motor checks, and no unexplained ESP32 resets, Bluetooth loss, driver fault, excessive heating, or severe droop. A failed or incomplete phase-1 gate SHALL be corrected and repeated without the camera connected.

#### Scenario: Approve phase 2
- **WHEN** every documented phase-1 acceptance item has passed and the assembly has been shut down normally
- **THEN** the teacher may authorize connection and separate qualification of the third LM2596 and ESP32-S3 camera branch

#### Scenario: Reject phase advancement
- **WHEN** any required phase-1 measurement is missing or an abnormal observation remains unexplained
- **THEN** the third converter and camera remain disconnected and the record identifies the unresolved gate

### Requirement: Phase-2 camera branch and full-load evidence
Phase 2 SHALL adjust and verify the third LM2596 at 5.0 V before connecting the ESP32-S3 N16R8 camera board through its 5 V input. It SHALL first record the camera branch voltage and current during boot, Wi-Fi connection, and representative camera streaming without either motor energized, and SHALL then record whole-pack current and all three load-terminal voltages during a bounded full-system test. It SHALL prohibit simultaneous external 5 V and USB 5 V connection unless isolation for the exact board has been verified.

#### Scenario: Qualify the camera branch separately
- **WHEN** the camera converter has passed its unloaded voltage check and the camera is powered without motor activity
- **THEN** the record captures 5 V load-terminal voltage, current during boot and representative Wi-Fi camera operation, and any reset, stream interruption, or excessive heating

#### Scenario: Exercise all three branches
- **WHEN** the camera-only check passes and the bounded full-system test is authorized
- **THEN** the record captures pack voltage and current, both 5 V load-terminal voltages, motor load-terminal voltage, and observations during Bluetooth connection, camera streaming, and conservative simultaneous motor operation

### Requirement: Power budget and expansion boundary
The documentation SHALL provide a power budget that distinguishes expected current from measured startup or transient current, converter-output power from battery-input power, and reserved margin from demonstrated spare capacity. It SHALL update the phase-2 budget from recorded measurements and SHALL discuss additional motors, steppers, or servos only as future loads requiring their own driver, converter, protection, and new validation rather than treating unused arithmetic capacity as authorization to connect them.

#### Scenario: Review the baseline budget
- **WHEN** the learner compares phase-1 and phase-2 measurements with the planned budget
- **THEN** the tables show rail voltage, expected and measured current, output power, estimated conversion loss, and remaining margin without claiming unmeasured LM2596 or DRV8833 capacity

#### Scenario: Consider another actuator
- **WHEN** a future motor, stepper, or servo is proposed
- **THEN** the documentation requires its startup or stall demand and driver requirements to be added to a new budget and validation plan before connection

### Requirement: Distribution, suppression, and shutdown guidance
The activity SHALL include a detailed wiring table and physical layout guidance that keeps motor current out of ESP32 headers and signal returns, uses short star-connected power branches, twists each motor pair where practical, and provides local brush-noise suppression, driver bypassing, and bulk capacitance sized as a measured starting configuration. It SHALL retain the prohibition on a single flyback diode across a reversible motor and SHALL define normal and emergency shutdown sequences that remove battery power before wiring changes.

#### Scenario: Inspect distribution and capacitance
- **WHEN** the phase-1 assembly is reviewed before power is applied
- **THEN** motor and logic currents have separate positive branches, returns meet at the defined distribution ground, both motors have non-polarized terminal suppression, the driver and ESP32 loads have documented local bypass and bulk capacitance, and capacitor polarity and voltage ratings are verified

#### Scenario: Shut down after normal testing
- **WHEN** a measurement sequence finishes normally
- **THEN** the operator disarms, opens the physical battery cutoff, verifies motion has stopped and rails have decayed, and only then changes wiring or meter placement

#### Scenario: Shut down for an abnormal condition
- **WHEN** there is smoke, odor, unexpected motion, a persistent stall, severe voltage droop, repeated reset, excessive heat, or damaged insulation
- **THEN** the operator uses the reachable physical cutoff without delaying for further measurements and does not re-energize the assembly until it has been inspected
