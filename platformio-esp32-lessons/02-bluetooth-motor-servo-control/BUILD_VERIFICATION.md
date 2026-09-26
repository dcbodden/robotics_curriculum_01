# Scaffold Build Verification

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

This verifies only that the standalone scaffold bootstraps and builds for the
supported ESP32-WROOM board. It does not claim that Bluetooth has been exercised
on hardware or that motor/servo mapping, actuator outputs, bench wiring, or
physical motion have been implemented or verified.
