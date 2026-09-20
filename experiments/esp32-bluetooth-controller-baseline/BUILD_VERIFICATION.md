# Clean Build Verification

## 2026-09-20 baseline

The experiment was cleaned and rebuilt from its own directory with its pinned,
project-local PlatformIO installation:

```bash
./scripts/pio.sh --version
./scripts/pio.sh run --environment esp32doit-devkit-v1 --target clean
./scripts/pio.sh run --environment esp32doit-devkit-v1
```

All three commands succeeded. The clean command removed
`.pio/build/esp32doit-devkit-v1`, so the subsequent 278.97-second build compiled
and linked the complete dependency graph rather than reusing object files from
an incremental build.

## Resolved versions

| Layer | Version or revision observed by the build |
| --- | --- |
| PlatformIO Core | `6.1.19` |
| Click host dependency | `8.1.7` |
| PlatformIO environment / board | `esp32doit-devkit-v1` / `esp32doit-devkit-v1` |
| pioarduino Espressif32 platform | `54.3.21`, selected by the pinned `54.03.21` release URL |
| ESP-IDF framework package | `3.50402.0` (`ESP-IDF 5.4.2`) |
| Arduino Core for ESP32 | `3.2.1`, commit `ac961f671abd5ae1da0a15fd4bee71ed807c2cf3` |
| Bluepad32 template | tag `4.2.2`, commit `d07a9385f46f7215f51fc3eb5e40c5a484cfe102` |
| Bluepad32 | `4.2.0` snapshot from the pinned template |
| BTstack | `1.6.2` snapshot from the pinned template |
| Xtensa toolchain | `14.2.0+20241119` |

PlatformIO also resolved these build tools and platform packages:

| Package | Version |
| --- | --- |
| `tl-install` | `5.0.0` |
| `tool-cmake` | `3.30.2` |
| `tool-dfuutil-arduino` | `1.11.0` |
| `tool-esp-rom-elfs` | `2024.10.11` |
| `tool-esptoolpy` | `5.0.0` (reports `esptool.py v5.0.0-dev1` at runtime) |
| `tool-mkfatfs` | `2.0.1` |
| `tool-mklittlefs` | `3.2.0` |
| `tool-mklittlefs-4.0.0` | `4.0.0` |
| `tool-mkspiffs` | `2.230.0` |
| `tool-ninja` | `1.10.2` |
| `tool-scons` | `4.40801.0` (`SCons 4.8.1`) |

The tracked [`dependencies.lock`](dependencies.lock) records the exact ESP-IDF
Component Manager dependency versions and hashes. At verification time its
SHA-256 was
`a2d16ca8b5b0c9c262172e821aacec393b2f3787e3148086eaa39d0b5728b4ca`.
The pinned source and license details remain in
[`DEPENDENCIES.md`](DEPENDENCIES.md).

## Memory and artifacts

PlatformIO reported:

```text
RAM:   [===       ]  25.5% (used 83648 bytes from 327680 bytes)
Flash: [==        ]  17.0% (used 714468 bytes from 4194304 bytes)
```

The resulting `firmware.elf` was 7,384,816 bytes on the build host, and the
flashable `firmware.bin` was 714,864 bytes. The PlatformIO flash-usage figure
describes linked program usage; it is not expected to equal the container file
size byte-for-byte.

## Warnings observed

The build completed successfully with these warnings:

1. pip disabled its host cache because `/home/dbodden/.cache/pip` was not
   writable. This affects caching only, not the selected dependencies or
   firmware output.
2. Bluepad32's `uni_mouse_quadrature.c` includes ESP-IDF's deprecated legacy
   timer-group header. The warning recommends migration to `driver/gptimer.h`.
3. The managed `esp_diagnostics` component includes the deprecated
   `freertos/task_snapshot.h` header.
4. PlatformIO invoked `esp-idf-size` with the unsupported `--ng` option. That
   optional report exited with code 2; PlatformIO then used its fallback size
   calculation, printed the RAM and flash figures above, generated
   `firmware.bin`, and finished with `SUCCESS`.

No warning originated from this experiment's `main/` sources.

## Verification boundary

This clean build verifies that the pinned project resolves, compiles, links,
fits the configured ESP32-WROOM target, and produces a firmware image. It does
not prove that the image was uploaded or executed, that Bluetooth initialized
on hardware, or that the SZ-4015B paired, reconnected, or delivered controller
input. Those behaviors require the separate structured serial sessions in
OpenSpec tasks 5.3 and 5.4.

