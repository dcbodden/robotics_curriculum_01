# Lesson 02 Dependency Record

This lesson follows the official Bluepad32 ESP-IDF + Arduino template, but
keeps generated third-party component source out of this repository. Run
`scripts/bootstrap-dependencies.sh` to materialize the exact components below.

## Derivation record

Lesson 02 was scaffolded from the tracked
`experiments/esp32-bluetooth-controller-baseline/` tree at repository commit
`463dd061bb77d4065c8e79673026f32792fddff6` (baseline tree
`4bf49e1acf66f3b483e9e57247cb9616626efbad`). It retains that baseline's exact
tooling and dependency pins so Bluetooth behavior can be compared directly,
while remaining a separate project with its own firmware identity. The source
baseline is not a build-time dependency and remains unchanged.

## Pinned inputs

| Input | Selected revision | Source | Archive SHA-256 |
| --- | --- | --- | --- |
| PlatformIO Core | `6.1.19` | <https://github.com/platformio/platformio-core/releases/tag/v6.1.19> | Installed from the exact version in `requirements-tooling.txt` |
| Click (host tool) | `8.1.7` | <https://github.com/pallets/click/releases/tag/8.1.7> | Installed from the exact version in `requirements-tooling.txt` |
| Bluepad32 ESP-IDF + Arduino template | tag `4.2.2`, commit `d07a9385f46f7215f51fc3eb5e40c5a484cfe102` | <https://github.com/ricardoquesada/esp-idf-arduino-bluepad32-template> | `6ced5c5b4c6af92f4412bff4d7bd8149f5073010d375f95b54482db1c406364a` |
| pioarduino PlatformIO Espressif32 platform | release `54.03.21` | <https://github.com/pioarduino/platform-espressif32/releases/tag/54.03.21> | `4a069f95ec901a375b4185000a7dd3b503f283fb9973a711a5beba0cad24e7d8` |
| Arduino Core for ESP32 | `3.2.1`, commit `ac961f671abd5ae1da0a15fd4bee71ed807c2cf3` | <https://github.com/espressif/arduino-esp32/tree/ac961f671abd5ae1da0a15fd4bee71ed807c2cf3> | `8d0156e0aa38976e2f5ed7ad31a9c7787a6465f09844834901bdff19af5fe23d` |
| ESP-IDF | `5.4.2`, supplied by pioarduino `54.03.21` | <https://github.com/espressif/esp-idf/releases/tag/v5.4.2> | Managed by the pinned PlatformIO platform |
| Bluepad32 | `4.2.0` snapshot carried by template commit `d07a9385...` | <https://github.com/ricardoquesada/bluepad32> | Covered by the checksum-pinned template archive |
| BTstack | `1.6.2` snapshot carried by template commit `d07a9385...` | <https://github.com/bluekitchen/btstack> | Covered by the checksum-pinned template archive |

The template release number is newer than its embedded Bluepad32 version: the
template's `4.2.2` release combines Bluepad32 `4.2.0`, Arduino Core `3.2.1`, and
ESP-IDF `5.4.2`. The bootstrap script verifies archive bytes before extracting
anything into `components/`.

PlatformIO Core is pinned because this pioarduino platform requires SCons
`4.8.1`. PlatformIO Core `6.2.0` installs SCons `4.11.1`, whose removed
`SCons.Tool.FortranCommon` module prevents this ESP-IDF release from configuring.
Use `scripts/pio.sh` to keep the compatible Core and its package cache local to
this lesson. Click is also pinned because the esptool version carried by the
platform predates Click's newer parameter API.

## License obligations

This is a dependency record, not legal advice. Review the linked license text
before redistributing firmware or using it commercially.

- The template project, Bluepad32, and this lesson's derived bootstrap
  source are Apache-2.0. Preserve copyright, license, and NOTICE material when
  redistributing covered source or binaries.
- `bluepad32_arduino` files identify Apache-2.0 or LGPL-2.1-or-later terms;
  preserve those notices and comply with the selected license.
- Arduino Core for ESP32 is primarily LGPL-2.1 and includes separately licensed
  bundled material. Its downloaded `LICENSE.md` and file-level notices are the
  authoritative terms.
- The pioarduino PlatformIO platform is Apache-2.0. Platform-installed toolchains
  and frameworks retain their own licenses.
- BTstack permits source and binary redistribution under its stated conditions
  only for personal, non-commercial use; commercial use requires a separate
  BlueKitchen license. Preserve its copyright, conditions, disclaimer, and do
  not use contributor names for endorsement. See
  <https://github.com/bluekitchen/btstack/blob/master/LICENSE>.

The generated `components/` tree retains upstream file headers and license files
that are present in the pinned archives. Do not commit a modified generated tree
without reviewing and preserving all applicable notices.
