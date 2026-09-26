## Context

See `proposal.md` for motivation and scope. The existing controller baseline is an ESP-IDF project with Arduino running under the Bluepad32 bootstrap, pinned pioarduino/Arduino/Bluepad32/BTstack inputs, a project-local PlatformIO Core wrapper, and structured `BTDIAG` output. It already produces normalized gamepad axes in the nominal Bluepad32 range of -512 through 511 and establishes connection, input, timeout, and bond-management behavior.

Lesson 06 supplies useful teaching patterns—pure command mapping, bounded servo pulses, command telemetry, actuator-power gates, and explicit separation between a command and observed motion—but its AVR timers, ADC scale, and one-direction MOSFET motor stage do not carry over directly. The HW-627 module uses a DRV8833 dual H-bridge, so one motor channel can be driven bidirectionally from two logic inputs. The exact passive population and exposed sleep/fault pins of the user's HW-627 board are not backed by an authoritative module schematic and must be inspected rather than assumed.

## Goals / Non-Goals

**Goals:**

- Keep Bluetooth transport, control policy, output generation, and telemetry separable enough to test control behavior on the host.
- Ensure every reset, connection transition, stale-input condition, and direction reversal has a deterministic safe motor output.
- Produce smooth, bounded bench commands for one reversible geared motor and one servo while retaining readable diagnostic evidence.
- Make the staged bench procedure capable of finding supply, grounding, brush-noise, and module-pinout problems before simultaneous motion.
- Organize the physical workflow as student-facing missions with teacher gates and deeper implementation detail separated from the learner focus.

**Non-Goals:**

- Abstracting the firmware for arbitrary gamepads, motor drivers, multiple drive motors, or multiple servos.
- Implementing rover mixing, differential drive, steering geometry, obstacle response, autonomous behavior, or battery management.
- Characterizing the motor's loaded performance or intentionally performing a locked-rotor stall test.
- Certifying an undocumented HW-627 clone solely from the DRV8833 IC ratings.

## Decisions

### 1. Create numbered Lesson 02 from the baseline

Add `platformio-esp32-lessons/02-bluetooth-motor-servo-control/` and reproduce the baseline's pinned dependency declarations, bootstrap process, local PlatformIO wrapper, ESP-IDF/Arduino entry structure, and relevant diagnostics. Downloaded dependency trees and generated tooling remain ignored and reproducible from the new project's manifests.

This keeps the controller-only baseline useful as a known diagnostic checkpoint and makes the actuator lesson independently openable. Modifying the baseline in place would erase that checkpoint; making the new lesson depend on files in the baseline experiment would violate the repository's standalone-project convention.

### 2. Separate input, policy, and hardware output

Use four conceptual layers:

1. A controller adapter converts the selected Bluepad32 controller report into a small snapshot containing sequence/time, left-stick axes, and arm/disarm button edges.
2. Pure mapping functions clamp the nominal -512 through 511 inputs, apply dead zones, and calculate target motor and servo commands.
3. A pure control policy owns connection, neutral-ready, armed, disarmed, and failsafe transitions plus the stale-input watchdog and reversal interlock.
4. A hardware adapter applies the policy's output command to ESP32 PWM pins.

The main loop continues calling `BP32.update()` without blocking, advances the control policy on a regular cadence, applies outputs, processes bounded serial commands, and emits rate-limited telemetry. This is preferred to producing actuator output directly inside Bluetooth callbacks because policy transitions and host verification remain deterministic.

### 3. Use one controller and one-stick driving

The first connected valid gamepad becomes the active controller. Other controllers may still be diagnosed but cannot influence outputs. Loss of the active controller immediately invalidates the control source; control does not silently transfer to another connected device.

Use the left stick for both commands:

- `axisY`, inverted so physical stick-up is positive, becomes signed drive.
- `axisX` becomes servo position, with a named configuration flag available if the physical steering direction needs inversion.

Start with a dead-zone half-width of 40 Bluepad32 counts on each axis. Outside the dead zone, remap the remaining negative and positive spans independently so both endpoints are reachable despite the asymmetric -512/511 range. Map drive to -255 through 255 and servo position to a default 1000 through 2000 microsecond pulse with 1500 microseconds at center. Keep these calibration values centralized and document narrowing the servo endpoints when the actual servo binds or buzzes.

Using one stick preserves Lesson 06's simultaneous two-axis observation. A split-stick mapping was considered but would change the learning model without providing a bench-test advantage.

### 4. Require neutral plus explicit edge-triggered arming

Use the controller's OPTIONS control as the arm action and CIRCLE as the disarm action. Arming requires all of the following in the same control session:

- the selected controller is connected and reporting valid gamepad data;
- both mapped axes are inside their dead zones;
- at least one fresh neutral report has been observed after connection or failsafe; and
- a new OPTIONS press edge occurs after that neutral report.

CIRCLE disarms regardless of stick position. Button holds do not repeatedly toggle state. Reconnection, firmware reset, watchdog expiry, invalid controller state, and active-controller disconnection clear neutral readiness and the arm latch. This is preferred to automatic control immediately after pairing because a displaced or stale stick could otherwise start the motor.

### 5. Use a 300 ms freshness watchdog with immediate safety output

Every fresh valid active-controller report updates a monotonic timestamp. While armed, reaching 300 ms without another fresh report enters failsafe, comfortably inside the specification's 500 ms maximum. Disconnect and explicit disarm take the same immediate safety-output path.

The safety output bypasses normal slew limiting: both motor bridge inputs go low immediately and the servo target becomes center. Recovery requires fresh neutral and a new arm edge. A longer timeout was rejected because it permits unnecessary travel after radio or controller failure; using only Bluepad32's disconnect callback was rejected because report delivery can stop before disconnection is declared.

### 6. Generate independent motor and servo PWM

Reserve the following output-capable ESP32-WROOM pins:

| Function | GPIO | Initial behavior |
| --- | ---: | --- |
| DRV8833 AIN1 | 26 | Low |
| DRV8833 AIN2 | 27 | Low |
| Servo signal | 25 | Centered 50 Hz pulse after output setup |
| Optional verified `nSLEEP` connection | 13 | Low until safe initialization completes |

Configure separate ESP32 PWM resources for approximately 20 kHz, 8-bit motor duty and 50 Hz servo pulses with sufficient resolution to represent microsecond-scale pulse commands. Pin-to-channel allocation is explicit so motor and servo configuration cannot overwrite one another. The implementation will use the pinned Arduino core's supported LEDC API rather than assume an API from another core version.

GPIO 13 is used only if inspection confirms that the particular HW-627 exposes an active-low sleep input. If no controllable sleep pin is exposed, safe startup relies on the DRV8833 input pulldowns plus firmware driving GPIO 26 and 27 low before enabling PWM. An exposed `nFAULT` may be reported as supplemental evidence, but lack of that pin is not a blocker.

### 7. Use forward-coast PWM, slew limiting, and a zero reversal interval

Translate the applied signed drive command as follows:

| Applied command | AIN1 | AIN2 |
| --- | --- | --- |
| Positive | PWM magnitude | Low |
| Zero | Low | Low |
| Negative | Low | PWM magnitude |

Normal commands move toward the mapped target by at most 16 duty counts per 20 ms control update, giving a full-scale transition of roughly one third of a second. If the requested sign differs from the applied sign, drive both inputs low immediately, hold zero for at least one complete control update, and then ramp in the new direction. Failsafe and disarm always jump directly to zero rather than ramping down.

Forward-coast PWM matches the specified zero behavior and makes mechanical coasting visible. Active braking and forward-brake PWM were rejected for this first lesson because they create sharper current and mechanical transients and complicate the comparison with Lesson 06.

### 8. Preserve lifecycle diagnostics and add control telemetry

Retain versioned `BTDIAG` records for firmware identity, dependency identity, Bluetooth readiness, connection lifecycle, first input, waiting, timeout, and confirmation-gated bond clearing. Give the new firmware its own identity and keep lower-level stack logs visible.

Add a separately identifiable, versioned control record containing active controller index, state, freshness age, raw mapped axes, target and applied signed motor commands, bridge direction/duty, servo pulse command, and transition reason. Emit immediately on significant state transitions and otherwise no faster than a documented bounded interval, initially 200 ms. Serial output must never delay the control cadence.

### 9. Treat the HW-627 as an inspected module and add external power-integrity parts

The procedure begins with a photographed or recorded pin-label and component inspection of the actual module. It identifies motor supply, ground, both inputs and outputs for the selected bridge, and any exposed sleep/fault signals. It does not infer the board from its `HW-627` marking alone.

Use an externally regulated 5 V bench actuator supply if that voltage is permitted by the actual motor and servo. USB powers the ESP32. Motor-driver and servo positive branches originate separately at the actuator distribution point; ESP32 5 V and 3.3 V are not connected to the actuator-positive rail. ESP32, driver, servo, and supply grounds meet through a low-impedance common reference without routing motor current through the ESP32 ground jumper or a marginal breadboard rail.

The initial suppression and energy-storage arrangement is:

- a non-polarized 100 nF ceramic directly across the brushed-motor terminals;
- a 100 nF ceramic close to HW-627 motor supply and ground;
- 100 to 220 microfarads of suitably voltage-rated electrolytic bulk capacitance close to that supply input, reduced only when verified equivalent parts are already populated;
- optional 100 to 470 microfarads at the servo branch and measurement-driven increases up to roughly 470 microfarads at the driver or 470 to 1000 microfarads at the distribution point.

Motor wires are short and twisted where practical. A single flywheel diode is never installed across the motor because its polarity reverses; the full bridge provides inductive-current recirculation. Additional clamps, filters, or capacitance are added only from measured voltage/noise behavior and with component ratings checked.

### 10. Verify progressively and keep evidence boundaries explicit

Host checks cover mapping and policy without the embedded dependencies. The reproducible wrapper then builds the complete pinned firmware. Physical testing proceeds with actuator power off for connection, neutral, arming, disarming, and telemetry checks; then servo-only motion; low-duty forward motor motion; release/coast; low-duty reverse; reversal interlock; and finally simultaneous motor/servo motion.

The geared motor remains secured, bare, and unloaded. The approximate 300 mA figure is recorded as unverified running current, not used as a stall or startup rating. Supply voltage and current behavior are observed without deliberately stalling the motor. Build, host checks, Bluetooth operation, electrical measurements, and physical motion receive separate verification statuses.

## Risks / Trade-offs

- **[Undocumented HW-627 variants differ in pinout or passive population]** → Require inspection against the physical labels and continuity/component evidence; keep optional sleep/fault support conditional.
- **[The motor's startup or stalled demand may exceed the assumed 300 mA running current or the module's practical thermal capacity]** → Begin at limited duty with a current-limited supply, observe current and voltage, avoid deliberate stalls, and stop on fault or heating rather than relying on headline IC ratings.
- **[Motor and servo transients can reset the ESP32 or disturb Bluetooth]** → Use separate actuator branches, short returns, local bypass/bulk capacitance, motor-terminal suppression, staged testing, and measurement-driven capacitor adjustment.
- **[Servo endpoints vary and 1000/2000 microseconds may bind a particular unit]** → Centralize limits, start near center, test incrementally, and narrow endpoints before wider travel.
- **[A 300 ms watchdog may trip if a controller produces unexpectedly sparse reports]** → Log freshness age during actuator-power-off testing and adjust only with evidence while retaining the 500 ms requirement.
- **[Slew limiting can prevent a low-inertia motor from overcoming starting friction at small commands]** → Preserve the requested target in telemetry and test increasing bounded commands; do not hide the physical starting threshold with an automatic minimum duty in the initial design.
- **[Duplicating the baseline scaffold creates maintenance work]** → Record exact upstream revisions and keep the derivation explicit; independence is favored over a shared build tree for reproducibility.

## Migration Plan

This is an additive numbered lesson with no data or firmware migration. Implement it as Lesson 02 under `platformio-esp32-lessons/`, validate it independently, and leave the controller baseline and Lesson 01 unchanged. If the integration proves unsafe or unreliable, remove or quarantine only the new lesson; the baseline remains available to isolate Bluetooth behavior.

## Open Questions

- Which sleep/fault pins and which bypass or bulk capacitors are actually exposed or populated on the user's HW-627 specimen? Record this during the required module inspection.
- What neutral jitter, usable servo pulse endpoints, running current, and supply transient behavior are measured on the specific controller, servo, and motor? Use actuator-power-off calibration and staged bench evidence to finalize configuration values without changing the architecture.
