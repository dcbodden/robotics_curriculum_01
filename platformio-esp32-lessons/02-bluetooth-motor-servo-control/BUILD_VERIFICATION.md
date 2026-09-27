# Build and Host Verification

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
GPIO waveforms or actuator motion. Servo, motor, reversal, failsafe-motion, and
simultaneous-motion checks remain pending under task 7.5.

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
