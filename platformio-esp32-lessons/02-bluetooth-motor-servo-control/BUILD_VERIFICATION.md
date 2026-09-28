# Build and Host Verification

## 2026-09-27 strict OpenSpec validation and scenario review

Strict validation was run from the repository root:

```bash
openspec validate add-bluetooth-motor-servo-control --strict
```

It exited 0 with `Change 'add-bluetooth-motor-servo-control' is valid`. The
current lesson host suite was also rerun with `./test/run-host-checks.sh`; all
mapping, policy, actuator-command, and telemetry checks passed with warnings
treated as errors. The controller baseline path still had no working-tree
changes.

Every delta-spec scenario was then reviewed against the finished lesson. In
the tables below, “conforms” means that the required code, documentation, test,
or recorded observation exists. It does not elevate host checks or logical
telemetry into physical evidence.

### Standalone Bluetooth actuator lesson

| Scenario | Review result |
| --- | --- |
| Open the actuator lesson independently | Conforms: the lesson-local configuration, pinned dependencies, wrapper, source tree, instructions, and recorded clean build are self-contained. |
| Browse the ESP32 lesson collection | Conforms: the collection index lists Lesson 02 after Lesson 01 and identifies Bluetooth Classic, bidirectional motor, servo, and bench-only scope. |
| Keep the diagnostic baseline intact | Conforms: the baseline independently built from its own tree, its tracked tree was unchanged, and it remains unchanged in the current worktree. |

### Wireless motor and servo command mapping

| Scenario | Review result |
| --- | --- |
| Move the stick forward and backward | Conforms: mapping checks cover both signed directions, and powered motion was user-observed in both directions. |
| Steer left and right | Conforms: mapping checks cover bounded pulses on both sides of center, and powered servo response was user-observed. |
| Release the stick | Conforms: mapping and policy checks require zero motor and a 1500-microsecond centered servo command inside both dead zones. |
| Receive an extreme or out-of-range axis value | Conforms: host checks cover clamping below -512 and above 511 plus bounded motor and servo outputs across the entire supported range. |

### DRV8833 bidirectional output behavior

| Scenario | Review result |
| --- | --- |
| Command forward motion | Conforms: bridge checks require AIN1 PWM with AIN2 low, and forward motor motion was user-observed. |
| Command reverse motion | Conforms: bridge checks require AIN1 low with AIN2 PWM, and reverse motor motion was user-observed. |
| Reverse direction | Conforms: policy and actuator checks prove immediate zero plus one complete zero update before opposite duty. Safe physical reversal was observed, but the exact GPIO zero interval was not instrumented and remains a stated evidence boundary. |
| Command motor stop | Conforms: bridge checks require both inputs low, and the missions explicitly distinguish electrical coast from continuing mechanical motion. |

### Explicit arming and loss-of-control failsafe

| Scenario | Review result |
| --- | --- |
| Start or connect with a displaced stick | Conforms: the policy check rejects neutral readiness and arming outside the dead zones, and Mission 1 requires this check with actuator power off. |
| Arm from neutral | Conforms: policy checks require a prior fresh neutral report and a later OPTIONS edge; powered arm/disarm operation was user-observed. |
| Lose fresh controller input | Conforms in implementation: the 300 ms watchdog host check proves zero motor and centered servo inside the 500 ms limit. A powered physical report-loss observation remains pending. |
| Disconnect and reconnect | Conforms in implementation: host checks require safe output, fresh neutral, and a new arm edge; bonded reconnection was observed with actuator power off. Powered physical disconnect behavior remains pending. |
| Disarm explicitly | Conforms: host checks prove immediate safe output and the user observed working disarm behavior with actuator power enabled. |

### Bench-only lesson guidance

| Scenario | Review result |
| --- | --- |
| Prepare the bench circuit | Conforms: `BENCH_WIRING.md` specifies USB-only ESP32 power, separate regulated actuator-positive branches, a common signal reference, and no actuator-positive connection to ESP32 power pins. |
| Begin physical testing | Conforms: the wiring checklist and gated missions require neutral evidence, secured unloaded actuators, clear motion areas, polarity checks, and conservative current limiting before power. |
| Encounter abnormal operation | Conforms: the guide requires immediate external-power removal followed by USB removal, inspection, and no reconnection until the cause is corrected. |
| Preserve later rover scope | Conforms: the lesson, collection index, and root navigation reserve batteries, conversion, wheels, chassis, linkage, and complete rover wiring for later work. |

### Power-integrity and inductive-load guidance

| Scenario | Review result |
| --- | --- |
| Add initial local decoupling | Conforms: the inspected-module guide specifies 100 nF at the motor, 100 nF at driver VCC/GND, and 100–220 uF local bulk with placement, polarity, and rating checks. |
| Address servo or shared-supply transients | Conforms: the guide gives measurement-driven 470 uF driver, 100–470 uF servo, and 470–1000 uF distribution options without treating capacitance as a supply substitute. |
| Consider a motor flywheel diode | Conforms: the guide prohibits a single diode across the reversible motor and explains DRV8833 H-bridge recirculation. |
| Size the supply and driver | Conforms: the 300 mA value is labeled an unverified running estimate; startup/stall demand, thermal limits, and the prohibition on deliberate stall testing are explicit. |

### Observable control and verification evidence

| Scenario | Review result |
| --- | --- |
| Observe normal wireless control | Conforms: bounded `CONTROL` records contain axes, target/applied motor values, bridge direction/duty, servo pulse, state, freshness, and transition reason; connected-controller operation was observed. |
| Share the initialized Bluetooth console safely | Conforms: the firmware reuses BTstack's UART without restarting its driver, constrained-capacity telemetry checks pass, and startup through scanning plus continuing records was observed. |
| Observe a safety transition | Conforms: safety transitions enqueue immediate records containing their reason and safe output fields; policy and telemetry checks cover the values. Powered disconnect observation remains pending as noted above. |
| Verify pure control behavior | Conforms: the warning-as-error host suite covers mapping boundaries, clamping, inversion, arming, watchdog, disconnect/reconnect, slew, reversal, safe bridge commands, servo bounds, and telemetry cadence. |
| Record verification boundaries | Conforms: this record separates host, build, Bluetooth, user-observed motion, uninstrumented electrical behavior, and the still-pending powered disconnect/failsafe check. |

Review result: all 29 scenarios are represented by conforming implementation,
documentation, automated checks, or recorded observations. The powered
disconnect/report-loss test and instrumented reversal/power measurements remain
clearly identified evidence follow-ups; they are not represented as completed
physical tests.

## 2026-09-27 powered actuator bench check

### Automated evidence

No new automated run was used as physical evidence for this check. The
previously recorded host suite covers servo and motor command mapping,
forward/reverse bridge commands, the zero-before-reverse policy, explicit
disarming, and disconnect/watchdog failsafe logic. Those deterministic checks
do not prove motion, GPIO timing, supply integrity, or failsafe behavior with
connected actuators.

### User-observed evidence

With the external actuator supply enabled, the user reported that arming and
disarming worked and that both left-stick control axes operated their intended
actuators. The horizontal axis moved the servo, and the vertical axis drove the
secured motor in both forward and reverse. Direct direction reversal behaved
safely, and simultaneous motor-and-servo operation worked without an observed
reset or erratic actuator response.

The controller-disconnect/loss test was not performed directly. Physical
confirmation that disconnect or report loss forces motor coast and servo center
within the required interval therefore remains pending. No oscilloscope capture
or independent supply-voltage/current measurement was supplied, so the observed
reversal supports the behavior but does not measure the exact zero interval or
electrical transients.

## 2026-09-27 shared-console correction and actuator-power-off reconnection

### Automated evidence

The UART regression was reproduced before the correction as repeated stale
record fragments followed by an `IDLE1` task-watchdog report. The corrected
firmware leaves UART0 under the ownership established by
`btstack_stdio_init()`: it does not call `Serial.begin()`, resize Arduino
serial buffers, or install/delete the UART driver. Control records are queued
only when the active UART TX ring reports enough room for a complete record,
and the Arduino loop includes the baseline's cooperative 10 ms yield.

The host suite was rerun from the Lesson 02 directory:

```bash
./test/run-host-checks.sh
```

It exited 0. In addition to the existing mapping, policy, and actuator checks,
the telemetry check covered capacities below, equal to, and above a complete
record and reported:

```text
Telemetry checks passed: transitions, five-per-second limit, and constrained console capacity.
```

The firmware was then rebuilt from an explicitly empty build tree:

```bash
./scripts/pio.sh run --target clean
./scripts/pio.sh run
```

The clean build exited 0 with `SUCCESS` in 358.31 seconds. PlatformIO reported
85,720 of 327,680 RAM bytes (26.2%) and 722,040 of 4,194,304 flash bytes
(17.2%). The only warnings were the already-recorded dependency deprecations
and PlatformIO's nonfatal unsupported `esp-idf-size --ng` display attempt.

Firmware `0.2.2` was uploaded to `/dev/ttyUSB1`. Esptool identified an
ESP32-D0WD-V3 revision 3.1, wrote the 722,448-byte application image, verified
all hashes, performed a hard reset, and exited with `SUCCESS`.

After releasing the serial monitor's DTR and RTS reset lines, the fresh boot
produced complete records in this order: `firmware_started` for version
`0.2.2`, `dependency_identity`, `bluetooth_ready`, and
`accepting_connections` at 764 ms, followed by `BR/EDR scan -> 1` and
`BLE scan -> 1`. Complete `CONTROL` records continued at approximately 200 ms
intervals without a watchdog reset or repeated stale fragments.

The retained-bond controller check then produced valid reports for controller
index 0. The control policy selected it, observed neutral, and continuously
reported `state:"disarmed"`, fresh report ages, motor coast/duty 0, and a
1500-microsecond centered servo command. Cross presses were recorded at
82,192 ms and 85,104 ms as `BTDIAG controller_input` events with
`valid_report:true` and `pressed_buttons:"cross"`. No bond-clear command was
sent. This is serial evidence for Bluetooth discovery, bonded controller input,
policy processing, and safe logical commands; it is not evidence of electrical
isolation or physical actuator behavior.

An additional, non-required reset made while the controller was already active
rediscovered `Wireless Controller` but its SDP query timed out. That attempt
was stopped without clearing bonds and does not replace the preceding complete
successful check; future diagnostics should put the controller back into its
normal HOME-button reconnect state after resetting the ESP32.

### User-observed evidence

The user confirmed that the external actuator supply was off throughout the
successful controller check. This confirms the test boundary but does not prove
GPIO waveforms or actuator motion. At the end of this actuator-power-off check,
servo, motor, reversal, failsafe-motion, and simultaneous-motion checks were
still pending; the later powered check above records the subsequently observed
motion and the remaining disconnect/failsafe boundary.

## 2026-09-26 complete host checks

Run from the Lesson 02 directory:

```bash
./test/run-host-checks.sh
```

The script compiled all four host executables as C++11 with `-Wall -Wextra
-Werror`; any compiler warning would therefore fail the run. It exited 0 with:

```text
Mapping checks passed: inversion, dead zones, endpoints, clamps, and full axis range.
Policy checks passed: selection, arming, failsafe, watchdog, slew, and reversal.
Actuator checks passed: bridge truth table, safe-zero states, and servo duty bounds.
Telemetry checks passed: immediate transitions and at most five periodic records per second.
```

This is deterministic host evidence for the pure mapping, policy, bridge-command,
servo-duty, and telemetry scheduling code. It is not Bluetooth, GPIO waveform,
electrical, or physical-motion evidence.

## 2026-09-26 clean complete-firmware build

Run from the Lesson 02 directory:

```bash
./scripts/bootstrap-dependencies.sh
./scripts/pio.sh --version
./scripts/pio.sh project config --json-output
./scripts/pio.sh run --environment esp32doit-devkit-v1 --target clean
./scripts/pio.sh run --environment esp32doit-devkit-v1
```

The bootstrap exited 0 and reported that the pinned ESP-IDF components were
already present; the earlier scaffold run below records the checksum-verified
installation. The wrapper reported PlatformIO Core `6.1.19`. Resolved build
inputs were:

| Layer | Resolved version |
| --- | --- |
| pioarduino Espressif32 platform | `54.3.21` |
| ESP-IDF framework | `3.50402.0` (`5.4.2`) |
| SCons | `4.40801.0` (`4.8.1`) |
| Xtensa toolchain | `14.2.0+20241119` |

The configuration contained only environment `esp32doit-devkit-v1`, board
`esp32doit-devkit-v1`, framework `espidf`, and this lesson's `main/` source
directory. After the explicit clean, the complete build exited 0 with
`SUCCESS` in 401.32 seconds. PlatformIO reported:

| Output | Result |
| --- | --- |
| RAM | 86,040 of 327,680 bytes (26.3%) |
| Flash | 735,428 of 4,194,304 bytes (17.5%) |
| `firmware.elf` file size | 7,703,684 bytes |
| `firmware.bin` file size | 735,824 bytes |

The built lesson objects were `actuator_output.cpp.o`, `btdiag.cpp.o`,
`control_telemetry.cpp.o`, `main.c.o`, and `sketch.cpp.o`. No warning originated
from those sources. The pinned dependency stack emitted two known deprecation
warnings: Bluepad32 includes ESP-IDF's legacy timer-group header, and
`esp_diagnostics` includes the deprecated FreeRTOS task-snapshot header.
PlatformIO also attempted the unsupported optional `esp-idf-size --ng` form,
reported exit code 2 for that size-display step, fell back to its normal size
check, generated both firmware images, and ended with `SUCCESS`.

## 2026-09-26 controller-baseline independence

Run from `experiments/esp32-bluetooth-controller-baseline/`:

```bash
./scripts/bootstrap-dependencies.sh
./scripts/pio.sh --version
./scripts/pio.sh project config --json-output
./scripts/pio.sh run --environment esp32doit-devkit-v1
```

The bootstrap exited 0 with its pinned components already present, and the
baseline's own wrapper reported PlatformIO Core `6.1.19`. Its resolved
configuration contained one environment, `esp32doit-devkit-v1`, and used the
baseline's own `main/` source and `.tooling/` directory. The independent build
exited 0 with `SUCCESS` in 264.65 seconds and compiled exactly these baseline
project objects: `btdiag.cpp.o`, `main.c.o`, and `sketch.cpp.o`.

| Output | Result |
| --- | --- |
| RAM | 83,648 of 327,680 bytes (25.5%) |
| Flash | 714,468 of 4,194,304 bytes (17.0%) |
| `firmware.elf` file size | 7,384,816 bytes |
| `firmware.bin` file size | 714,864 bytes |

The dependency stack produced the same Bluepad32 timer-group and
`esp_diagnostics` task-snapshot deprecation warnings described above. The
unsupported optional `esp-idf-size --ng` display also fell back successfully;
no warning originated from the baseline's `main/` sources.

After the build, repository-root checks of the baseline path found no working
tree diff, staged diff, or non-ignored untracked files. Its tracked tree at
`HEAD` was `4bf49e1acf66f3b483e9e57247cb9616626efbad`. This confirms that Lesson 02
work did not modify the baseline's tracked source or documented behavior;
ignored build products remain local to the baseline. This is independent-build
evidence, not a new controller-on-hardware runtime test.

## 2026-09-26 Lesson 02 scaffold

The lesson's checksum bootstrap and project-local PlatformIO wrapper were run
from `platformio-esp32-lessons/02-bluetooth-motor-servo-control/`:

```bash
./scripts/bootstrap-dependencies.sh
./scripts/pio.sh --version
./scripts/pio.sh project config --json-output
./scripts/pio.sh run --environment esp32doit-devkit-v1
```

The bootstrap verified both pinned archive checksums. The wrapper reported
PlatformIO Core `6.1.19`, and the build finished successfully in 508.74 seconds.

## Project isolation

The resolved configuration contained one environment only:
`esp32doit-devkit-v1`, using board `esp32doit-devkit-v1`, framework `espidf`,
and source directory `main/` inside this lesson. The lesson's main build output
contained exactly these project object files:

```text
btdiag.cpp.o
main.c.o
sketch.cpp.o
```

No source path from another lesson or from
`experiments/esp32-bluetooth-controller-baseline/` appeared in the generated
compile commands. The baseline project had no tracked diff after scaffolding and
building Lesson 02.

## Resolved versions and output

| Layer | Resolved version |
| --- | --- |
| PlatformIO Core | `6.1.19` |
| pioarduino Espressif32 platform | `54.3.21` |
| ESP-IDF framework | `3.50402.0` (`5.4.2`) |
| SCons | `4.8.1` |
| Xtensa toolchain | `14.2.0+20241119` |

PlatformIO reported 83,648 bytes of RAM usage (25.5%) and 714,616 bytes of
flash usage (17.0%). It produced a 7,395,268-byte `firmware.elf` and a
715,024-byte `firmware.bin`.

The build retained the baseline's known dependency warnings: Bluepad32 uses a
deprecated legacy timer header, `esp_diagnostics` uses a deprecated FreeRTOS
snapshot header, and PlatformIO's optional `esp-idf-size --ng` invocation falls
back after that unsupported option. No warning originated from Lesson 02's
`main/` sources, and firmware image generation completed with `SUCCESS`.

## Verification boundary

The current host checks verify deterministic software behavior, and the clean
firmware build verifies compilation and image generation for the supported
ESP32-WROOM board. Neither result proves upload, Bluetooth connection, GPIO
waveforms, HW-627 wiring, supply integrity, servo motion, motor motion, reversal
on physical outputs, or failsafe behavior on connected hardware. Those checks
remain separate and pending until the staged bench procedure is performed.
