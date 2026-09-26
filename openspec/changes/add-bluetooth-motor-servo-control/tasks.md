## 1. Standalone Experiment Scaffold

- [ ] 1.1 Create `experiments/esp32-bluetooth-motor-servo-control/` as an independently openable project derived from the controller baseline while leaving the baseline unchanged.
- [ ] 1.2 Copy and rename the pinned dependency declarations, checksum bootstrap process, project-local PlatformIO wrapper, ESP-IDF configuration, build entry files, and ignore rules needed to reproduce the baseline toolchain.
- [ ] 1.3 Update firmware and dependency identity metadata for the new experiment and document its derivation from the known baseline revisions.
- [ ] 1.4 Confirm the new project selects only `esp32doit-devkit-v1`, builds through its local wrapper, and does not compile files from a lesson or sibling experiment.

## 2. Pure Mapping and Safety Policy

- [ ] 2.1 Implement host-compatible left-stick mapping with -512-through-511 input clamping, independent 40-count dead zones, inverted vertical drive, signed -255-through-255 motor output, and bounded 1000-through-2000 microsecond servo output.
- [ ] 2.2 Implement explicit controller selection and connection state so only the first selected valid gamepad can affect actuator commands and control never transfers silently.
- [ ] 2.3 Implement neutral readiness plus edge-triggered OPTIONS arming and CIRCLE disarming, clearing readiness after reset, disconnect, invalid state, or failsafe.
- [ ] 2.4 Implement the 300 ms fresh-report watchdog and immediate failsafe output of motor coast plus centered servo, with explicit transition reasons.
- [ ] 2.5 Implement the 20 ms applied-command update, 16-count motor slew limit, immediate zero on safety transitions, and one-full-update zero interlock before a direction reversal.
- [ ] 2.6 Add host checks covering mapping endpoints and clamps, both dead-zone boundaries, input inversion, servo centering, arm prerequisites and button edges, disarm, watchdog expiry, disconnect/reconnect, slew behavior, and zero-before-reverse behavior.

## 3. ESP32 Actuator Output

- [ ] 3.1 Add an output adapter that establishes safe low states before PWM setup and assigns GPIO 26/27 to DRV8833 inputs and GPIO 25 to the servo signal without resource conflicts.
- [ ] 3.2 Configure approximately 20 kHz 8-bit forward-coast motor PWM and a separate 50 Hz servo PWM resource using the API supplied by the pinned Arduino core.
- [ ] 3.3 Translate positive, zero, and negative applied motor commands into the specified AIN1/AIN2 truth table and verify that both inputs are low during initialization, disarm, failsafe, and reversal interlock.
- [ ] 3.4 Add centralized motor direction, dead-zone, PWM, servo endpoint, servo inversion, watchdog, slew, and telemetry configuration with compile-time consistency checks where practical.
- [ ] 3.5 Inspect and record the actual HW-627 labels and populated parts; add GPIO 13 `nSLEEP` control and optional fault telemetry only if those signals are verified as exposed on the user's module.

## 4. Bluetooth Integration and Diagnostics

- [ ] 4.1 Integrate the selected-controller snapshots and control-policy cadence with nonblocking `BP32.update()` processing while retaining the baseline connection, first-input, timeout, and confirmation-gated bond-clearing behavior.
- [ ] 4.2 Preserve versioned `BTDIAG` lifecycle records and lower-level Bluetooth logs while updating firmware identity for the actuator experiment.
- [ ] 4.3 Add versioned, bounded-rate control telemetry for state, freshness, axes, target/applied motor commands, bridge direction and duty, servo pulse, and safety transition reason.
- [ ] 4.4 Verify that significant safety transitions emit immediate evidence while periodic telemetry remains at or below the designed five-records-per-second rate and cannot block control processing.

## 5. Bench Wiring and Safety Documentation

- [ ] 5.1 Write an HW-627 inspection and wiring table that uses the physical module labels, assigns one bridge to the secured bare 48:1 geared motor, and records whether sleep, fault, and expected onboard capacitors are available.
- [ ] 5.2 Document USB-only ESP32 power, an appropriately rated regulated 5 V actuator supply, separate positive branches for driver and servo, a low-impedance common ground, and the prohibition on joining actuator positive to ESP32 5 V or 3.3 V.
- [ ] 5.3 Document the initial 100 nF motor-terminal capacitor, local 100 nF driver bypass, 100–220 microfarad driver bulk capacitor, optional servo/distribution bulk ranges, voltage-rating and polarity checks, short twisted motor leads, and measurement-driven adjustment.
- [ ] 5.4 Explain DRV8833 current recirculation and explicitly prohibit a single flywheel diode across the bidirectional motor; distinguish the provisional 300 mA running estimate from startup and stall demand.
- [ ] 5.5 Add power-off wiring rules, secured unloaded actuator requirements, clear motion areas, supply-current limiting, abnormal-operation stop conditions, and external-power-first shutdown instructions.
- [ ] 5.6 Keep batteries, DC/DC conversion, wheels, chassis, steering linkage, and complete rover integration visibly out of scope and point to them as later curriculum work.

## 6. Guided Bench Procedure

- [ ] 6.1 Add an actuator-power-off procedure for build/upload, Bluetooth pairing or reconnection, neutral telemetry, displaced-stick arm rejection, successful neutral arming, explicit disarming, and watchdog/disconnect failsafe checks.
- [ ] 6.2 Add a teacher- or experimenter-gated servo-only procedure that starts near center, verifies direction and pulse bounds, and narrows endpoints on binding or persistent buzzing.
- [ ] 6.3 Add a current-limited, conservative-duty motor procedure for forward motion, zero/coast, reverse motion, and the observable zero interval during reversal without deliberate stalling.
- [ ] 6.4 Add a final simultaneous motor-and-servo check that records supply voltage/current behavior, resets, Bluetooth continuity, actuator response, and whether additional bulk capacitance is justified.
- [ ] 6.5 Add readable troubleshooting paths for wrong project/port, pairing failure, failure to arm, immediate failsafe, reversed controls, no motor motion, unstable servo, driver fault/heat, and power-integrity symptoms.

## 7. Verification and Repository Integration

- [ ] 7.1 Run the complete host mapping and policy check suite with warnings treated as errors and record the command and result.
- [ ] 7.2 Bootstrap pinned dependencies and perform a clean firmware build with the project-local PlatformIO wrapper; record resolved versions, memory use, warnings, and the physical-verification boundary.
- [ ] 7.3 Confirm the existing Bluetooth controller baseline still builds independently and its tracked source and documented behavior were not modified.
- [ ] 7.4 Conduct or explicitly mark pending the controller connection, actuator-power-off safety, servo motion, forward/reverse motor motion, reversal, failsafe, and simultaneous-motion checks, keeping automated and user-observed evidence separate.
- [ ] 7.5 Update the repository experiment index or root navigation so contributors can find the new experiment without presenting it as a completed rover or numbered lesson.
- [ ] 7.6 Run strict OpenSpec validation and review the finished experiment against every scenario in the delta specification.
