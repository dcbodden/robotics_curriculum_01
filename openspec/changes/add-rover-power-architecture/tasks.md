## 1. Hardware Record and Architecture Documentation

- [ ] 1.1 Record the exact protected 3S pack, cell, BMS, charger, main fuse, cutoff switch, connectors, wire, meter, three LM2596 modules, HW-627, two motor specimens, and ESP32 board variants, including the ratings that bound the activity.
- [x] 1.2 Expand `syllabus-and-teacher-materials/Explorations/PowerArchitecture.md` into the follow-on guide with the three-branch block diagram, phase boundaries, power budget, expansion boundary, and explicit preservation of Lesson 02's bench-only contract.
- [x] 1.3 Add a point-to-point wiring table covering fused 3S distribution, the two phase-1 converters, the disconnected phase-2 branch, star returns, ESP32 5 V inputs, mirrored DRV8833 input signals, both bridge outputs, wire sizing, connector polarity, and USB/external-power exclusion.
- [x] 1.4 Add suppression and energy-storage guidance for both motor-terminal ceramics, driver ceramic and bulk capacitance, ESP32-local bulk capacitance, distribution bulk capacitance, polarity, voltage rating, placement, twisted motor pairs, and the prohibition on a single diode across a reversible motor.
- [x] 1.5 Add authoritative component-source references and clearly label the motor currents, converter efficiency, and camera demand as planning estimates until replaced by measurements.

## 2. Measurement and Safety Procedure

- [x] 2.1 Add a multimeter safety procedure showing voltage-mode parallel probing versus current-mode series insertion, fused-range selection, power-off rewiring, and a mandatory current-jack-to-voltage-jack restoration checkpoint.
- [x] 2.2 Add measurement tables that identify the measurement point, converter side, rail voltage, operating state, observed current, min/max-capture status, voltage droop, reset or connectivity evidence, driver fault evidence, and temperature observation.
- [ ] 2.3 Add normal and emergency shutdown sequences, teacher gates, secured-unloaded-motor rules, no-deliberate-stall rules, reachable cutoff requirements, and stop conditions for non-turning motors, unexpected motion, droop, resets, faults, odor, damaged insulation, or excessive heat.
- [ ] 2.4 Add an acceptance checklist that keeps phase 2 disconnected until every phase-1 source, rail, separate-load, integrated-load, and abnormal-observation item is resolved.

## 3. Phase-1 Source and Converter Qualification

- [ ] 3.1 With all loads disconnected, inspect the protected 3S assembly and record pack polarity and voltage through the fused cutoff path against the selected pack's documented operating range.
- [ ] 3.2 With loads still disconnected, adjust and record LM2596 A at 5.0 V and LM2596 B initially at 4.0 V, checking polarity directly at each converter output before attaching downstream wiring.
- [ ] 3.3 Inspect the completed phase-1 star distribution, branch returns, local capacitance, motor suppression, open `J2`, mirrored input-signal fan-out, and physical cutoff before authorizing load connection.

## 4. Phase-1 Separate-Load Bench Evidence

- [ ] 4.1 Power only the Bluetooth ESP32 branch, recheck voltage at the converter and board terminals, and record 5 V branch-output current during boot, Bluetooth connection, neutral/disarmed operation, and representative reporting.
- [ ] 4.2 With power removed, connect only motor A to its bridge; then record driver idle, observed motor-A startup, settled forward and reverse running, coast behavior, motor-rail voltage, faults, and heating without restraining the shaft.
- [ ] 4.3 Repeat the power-off wiring and complete the same isolated evidence for motor B on the second bridge while preserving motor specimen identity and lead orientation.
- [ ] 4.4 With power removed, connect both motors and record driver idle plus conservative simultaneous startup, settled operation, reversal, coast, motor-rail droop, observed current, faults, and heating.
- [ ] 4.5 If reliable unloaded startup requires more than 4.0 V, document the evidence, adjust LM2596 B upward with loads disconnected, stop at or below 4.5 V, and repeat every affected single- and dual-motor voltage/current check.

## 5. Phase-1 Integrated Evidence and Gate

- [ ] 5.1 Connect LM2596 A and B to the protected 3S distribution, confirm neutral/disarmed state before motor power, and record whole-pack voltage and current plus both load-terminal voltages at safe idle and Bluetooth-connected idle.
- [ ] 5.2 Record the same integrated measurements and observations during motor A operation, motor B operation, and conservative simultaneous motor operation, including resets, Bluetooth interruption, rail droop, driver faults, and heating.
- [ ] 5.3 Compare isolated branch measurements, integrated pack measurements, and the phase-1 budget without treating a handheld meter's observed startup maximum as an exact transient peak.
- [ ] 5.4 Complete and sign the phase-1 gate or record each failed item; keep the third converter and camera disconnected until all failures are corrected and the affected checks pass.

## 6. Phase-2 Camera Integration

- [ ] 6.1 After the phase-1 gate passes, adjust the unloaded third LM2596 to 5.0 V, verify output polarity, and document the exact camera-board 5 V and ground connection with USB power absent.
- [ ] 6.2 Power only the camera branch and record board-terminal voltage and branch-output current during boot, Wi-Fi connection, and the documented representative OV3660 streaming workload, along with reset, stream, and heating observations.
- [ ] 6.3 Connect all three branches and record whole-pack voltage and current plus both 5 V board-terminal voltages and the motor load-terminal voltage during Bluetooth connection, camera streaming, and bounded simultaneous motor operation.
- [ ] 6.4 Update the final power budget, converter margins, and conditional expansion discussion from the recorded phase-1 and phase-2 evidence without presenting unmeasured arithmetic headroom as available actuator capacity.

## 7. Curriculum Integration and Verification

- [ ] 7.1 Link the completed follow-on guide from the relevant ESP32 curriculum index or Lesson 02 boundary documentation while retaining all existing bench-only warnings and prerequisites.
- [ ] 7.2 Review the final guide against the capability scenarios, verify that all three LM2596 branches and both phases are represented consistently, and separate planning estimates from recorded observations.
- [ ] 7.3 Verify repository diffs contain only intended planning, curriculum, and evidence changes and do not alter the standalone Bluetooth controller baseline or silently add differential-drive behavior.
- [ ] 7.4 Run strict OpenSpec validation and record any physical checks that remain pending rather than representing documentation, compilation, or arithmetic as completed hardware verification.
