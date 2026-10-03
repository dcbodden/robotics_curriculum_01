# Rover power hardware record

This record applies to the hardware used for the `add-rover-power-architecture`
change. It is an identification and rating gate, not permission to energize the
3S system. A blank or `not recorded` field blocks powered work that depends on
that rating.

## Record status

**Incomplete — task 1.1 remains open.** Repository evidence identifies the
existing ESP32-WROOM lesson target and photographed HW-627 specimen, and the
exploration identifies the intended camera configuration. The battery system,
protection components, wiring, meter, LM2596 specimens, and second motor are not
identified well enough to establish safe operating limits.

Record information from the actual part, its packaging, and manufacturer or
supplier documentation. Do not copy a rating from a visually similar listing.
Photograph markings and connector orientation before installation. Preserve a
source link or local datasheet for every claimed limit.

## Battery source, protection, and charging

| Item | Exact identification | Ratings needed before use | Current record |
| --- | --- | --- | --- |
| Complete protected pack | Manufacturer, model, chemistry, series/parallel arrangement, serial or lot if present | Nominal/full/cutoff voltage, capacity, continuous and peak discharge current, permitted charge current, temperature limits | Not recorded; the design requires a protected 3S pack |
| Cell type inside pack | Cell manufacturer and exact model; quantity and arrangement | Nominal/full/minimum voltage, capacity, continuous discharge rating, permitted charge current | Not recorded; do not infer from `18650` dimensions |
| BMS/protection assembly | Manufacturer, exact model or board marking, connection diagram | Cell count and chemistry, continuous/peak current, overcharge, undervoltage, overcurrent, short-circuit and balancing behavior | Not recorded |
| Charger | Manufacturer, exact model and connector polarity | 3S chemistry compatibility, CC/CV termination voltage, maximum charge current, input rating | Not recorded |
| Main fuse and holder | Fuse family, part number, type and holder part number | DC voltage interrupt rating, current rating, time-current characteristic, holder rating | Not recorded |
| Physical cutoff | Manufacturer/model or legible body markings | DC voltage rating, continuous current, inrush/break rating where specified | Not recorded |

Do not substitute a bare three-cell holder, an unverified protection board, or
a nominally 12 V charger for the documented protected-pack system. The selected
charger must match the exact pack chemistry and series count.

## Distribution hardware

| Item | Exact identification | Ratings needed before use | Current record |
| --- | --- | --- | --- |
| Pack connector pair | Manufacturer/family, contact size, keyed polarity | DC voltage and continuous/peak current; mating-cycle condition | Not recorded |
| Main distribution connector or block | Manufacturer/model | DC voltage and per-position/total current | Not recorded |
| Branch connectors | Manufacturer/family and branch assignment | DC voltage and continuous/peak current | Not recorded |
| Main positive/negative wire | Insulation type, conductor material, stranded/solid, AWG or cross-section | Temperature and current rating for the installed length and bundling | Not recorded |
| Logic-branch wire | Same fields as above | Current rating and acceptable voltage drop | Not recorded |
| Motor-branch and motor-lead wire | Same fields as above | Startup-current rating, acceptable voltage drop, flex/strain suitability | Not recorded |

Connector and wire ratings must exceed the final protected-circuit limits. A
breadboard power rail or lightweight signal jumper is not a motor-current
distribution component.

## Measurement equipment

| Item | Exact identification | Ratings needed before use | Current record |
| --- | --- | --- | --- |
| Digital multimeter | Manufacturer, model, serial or asset identifier | DC-voltage ranges, DC-current ranges, current-jack time limit, input-fuse part numbers and interrupt ratings, lead category/rating, min/max capability | Not recorded |
| Test leads and adapters | Manufacturer/model or marked rating | Voltage/current/category rating and condition | Not recorded |
| Optional temperature instrument | Manufacturer/model | Measurement range and stated accuracy | Not recorded |

The meter manual and installed fuse markings must be checked before any series
current measurement. A current range described only as `10 A` is incomplete
without its fuse, time-limit, and lead/jack information.

## LM2596 converter specimens

The three modules must be tracked individually; a single marketplace listing is
not evidence that all three have the same IC, diode, inductor, capacitors, PCB,
or thermal behavior.

| Assignment | Specimen ID and photographs | Observed markings and population | Documented limits to record | Current record |
| --- | --- | --- | --- | --- |
| LM2596 A — Bluetooth ESP32, 5.0 V | Assign a durable ID | IC top mark, inductor mark/size, diode mark, input/output capacitor value and voltage, terminal labels | Input range, adjustable-output range, continuous/peak output current for the complete module, thermal conditions, polarity | Not recorded |
| LM2596 B — HW-627 and two motors, initially 4.0 V | Assign a durable ID | Same inspection fields | Same fields, with particular attention to sustained current and thermal derating | Not recorded |
| LM2596 C — camera ESP32, 5.0 V, phase 2 | Assign a durable ID | Same inspection fields | Same fields | Not recorded |

Until the complete module is identified and tested, the LM2596 IC's nominal
rating is not treated as the module's continuous-current rating.

## Motor driver specimen

| Field | Record |
| --- | --- |
| Module identity | HW-627 specimen photographed in `platformio-esp32-lessons/02-bluetooth-motor-servo-control/photos/hw-627-front.jpeg` and `hw-627-back.jpeg` |
| Driver marking | Texas Instruments logo and `DRV8833` observed on the 16-pin device |
| Edge labels | `IN4`, `IN3`, `GND`, `VCC`, `IN2`, `IN1`, `EEP`, `OUT1`, `OUT2`, `OUT3`, `OUT4`, `ULT` |
| Configuration jumper | `J2` present and photographed open; function remains unverified |
| Visible passives | `472` and `473` resistors; footprints marked `2.2uF`, `0.01uF`, and `10uF` |
| Package/current-limit status | Not resolved. Photographs do not establish the DRV8833 package option, exposed-pad connection, PCB thermal capability, or configured bridge-current regulation |
| Binding evidence | `platformio-esp32-lessons/02-bluetooth-motor-servo-control/HW-627-INSPECTION.md` |

Powered two-motor work remains bounded by the unresolved module thermal and
current-limit details plus measured behavior. The DRV8833 headline peak rating
is not a complete-module continuous rating.

## Motor specimens

Label the physical motors `MOTOR-A` and `MOTOR-B`; do not exchange them after
individual results are recorded.

| Field | MOTOR-A | MOTOR-B |
| --- | --- | --- |
| Manufacturer and exact model/SKU | Not recorded; existing lesson describes an Elegoo Arduino Car V1-style 48:1 geared motor | Not recorded |
| Body, gearbox and label photographs | Not recorded | Not recorded |
| Marked or documented voltage range | Existing curriculum describes the style as 3–6 V; exact specimen evidence not recorded | Not recorded |
| Gear ratio | Existing lesson describes its specimen as approximately 48:1; exact specimen evidence not recorded | Not recorded |
| No-load current and conditions | Not recorded | Not recorded |
| Startup/stall specification and conditions | Not recorded; do not deliberately stall to obtain it | Not recorded; do not deliberately stall to obtain it |
| Shaft/gearbox condition before test | Not recorded | Not recorded |

## ESP32 boards

| Assignment | Confirmed identity | Ratings and identity still needed |
| --- | --- | --- |
| Bluetooth controller | Repository target is an original ESP32-WROOM-32 development board in the 30-pin DOIT DevKit V1 layout; PlatformIO board ID `esp32doit-devkit-v1` | Photograph and transcribe the actual module and regulator markings; identify the exact board revision and the documented 5 V input range/current path |
| Camera controller | Exploration and supplied image identify an `ESP32-S3-CAM`-style board with ESP32-S3 N16R8, OV3660 camera, dual USB-C connectors, and header pins marked `5V` and `GND` | Record manufacturer, exact model/revision, ESP32 module top marking, regulator marking and 5 V input specification; confirm the installed sensor marking rather than relying only on the supplied image |

External 5 V and USB 5 V must not be connected simultaneously unless the exact
board's documented power-path isolation has been verified.

## Completion gate for task 1.1

Task 1.1 may be checked only after:

- every `Not recorded` identity needed for the selected build has been replaced
  with an observed exact part or an explicit decision that the item is absent;
- every voltage, current, fuse, connector, wire and temperature limit that
  bounds the activity has a manufacturer/supplier source tied to that exact
  part;
- photographs or transcribed markings distinguish all three LM2596 modules and
  both motors;
- the exact meter manual and installed current fuse are verified; and
- unresolved ratings are converted into explicit no-power blockers rather than
  filled with generic assumptions.
