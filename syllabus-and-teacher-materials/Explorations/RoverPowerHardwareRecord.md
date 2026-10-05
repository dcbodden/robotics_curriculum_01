# Rover power hardware record

This record applies to the hardware used for the `add-rover-power-architecture`
change. It is an identification and rating gate, not permission to energize the
3S system. A blank or `not recorded` field blocks powered work that depends on
that rating.

## Record status

**Provisional phase-1 record — task 1.1 remains open.** Repository evidence
identifies the existing ESP32-WROOM lesson target and photographed HW-627
specimen. The user identifies all three converters as HW-411 modules and
authorizes treating the source as a charged, protected 3S 18650 pack with
sufficient capacity. Charging, charger selection, cell balancing, and pack or
BMS construction are outside this activity. The camera is deferred and its
branch remains disconnected. The power wiring is identified as 18 AWG and the
meter's 10 A input is reported as fused. Main-fuse selection is deliberately
deferred, as is a separate cutoff switch. The power path uses soldered 18 AWG
wires and screw terminals. The meter model and motor specimen source identities
are not yet established.

Record information from the actual part, its packaging, and manufacturer or
supplier documentation. Do not copy a rating from a visually similar listing.
Photograph markings and connector orientation before installation. Preserve a
source link or local datasheet for every claimed limit.

## Battery source, protection, and charging

| Item | Exact identification | Ratings needed before use | Current record |
| --- | --- | --- | --- |
| Complete protected pack | Treated as an externally supplied, charged, protected 3S 18650 pack | Verify correct polarity and measure pack voltage before connection; use only within the pack supplier's discharge limits | User authorizes the protected-pack and sufficient-capacity assumptions for this activity; pack construction is not part of the activity |
| Cell type inside pack | Outside activity scope | No loose-cell assembly, cell replacement, or cell-level testing | Three-series-cell 18650 chemistry is assumed; internal cell details are not required for this activity |
| BMS/protection assembly | Integral to the assumed protected pack; not separately constructed or qualified here | Do not bypass the pack's protection; stop on protection trip or abnormal pack behavior | Protected-pack behavior is assumed; balancing and BMS construction are outside activity scope |
| Charger | Not used in this activity | Pack must arrive charged; remove it from the activity for charging with its appropriate external charging equipment | Explicitly out of scope |
| Main fuse and holder | Deferred | Not selected for the present documentation pass | User directs that main-fuse selection be deferred for now |
| Physical cutoff | Deferred | No separate cutoff selected for the present documentation pass | User directs that cutoff-switch selection be deferred for now |

Do not charge the pack as part of this activity or substitute a bare three-cell
holder for the assumed protected 3S source. If charging becomes necessary,
remove the pack and use charging equipment specified for that pack outside this
procedure.

## Distribution hardware

| Item | Exact identification | Ratings needed before use | Current record |
| --- | --- | --- | --- |
| Pack connector pair | Battery-holder screw terminals | DC voltage and continuous/peak current | User identifies screw terminals on the battery holder; terminal ratings not recorded |
| Main distribution connector or block | Battery-holder screw terminals feeding the HW-411 input wires | DC voltage and per-position/total current | Supply-side 18 AWG conductors are soldered into each HW-411 through-hole input and terminate at the battery-holder screw terminals |
| Bluetooth branch connection | ESP32 breakout-board screw terminals | DC voltage and continuous/peak current | HW-411 output uses through-hole-soldered 18 AWG conductors terminating at the ESP32 breakout-board screw terminals |
| Motor branch connection | Screw terminals feeding the breadboard-mounted HW-627 assembly | DC voltage and continuous/peak current | HW-411 output uses through-hole-soldered 18 AWG conductors terminating at screw terminals associated with the breadboard holding the HW-627 |
| Main positive/negative wire | 18 AWG; insulation, conductor material, and strand construction not recorded | Temperature and current rating for the installed length and bundling | Gauge supplied by user; remaining construction details not recorded |
| Logic-branch wire | 18 AWG power wire | Current rating and acceptable voltage drop | Gauge supplied by user |
| Motor-branch and motor-lead wire | 18 AWG branch wiring; motors retain their supplied 200 mm leads of unspecified gauge | Startup-current rating, acceptable voltage drop, flex/strain suitability | Branch-wire gauge supplied by user; integral motor-lead gauge not recorded |

Connector and wire ratings must exceed the final protected-circuit limits. A
breadboard power rail or lightweight signal jumper is not a motor-current
distribution component.

The HW-627 may be physically mounted on the breadboard for signal wiring, but
the record does not yet establish whether its supply or motor current passes
through solderless breadboard contacts. Before motor testing, verify that the
screw-terminal path carries motor power directly to the driver wiring rather
than through a breadboard rail or lightweight jumper.

## Measurement equipment

| Item | Exact identification | Ratings needed before use | Current record |
| --- | --- | --- | --- |
| Digital multimeter | Manufacturer/model not recorded | DC-voltage ranges, current-jack time limit, lead rating, and min/max capability | User reports that the 10 A current input is fused; fuse part number and interrupt rating are not recorded |
| Test leads and adapters | Manufacturer/model or marked rating | Voltage/current/category rating and condition | Not recorded |
| Optional temperature instrument | Manufacturer/model | Measurement range and stated accuracy | Not recorded |

The meter manual and installed fuse markings must be checked before any series
current measurement. A current range described only as `10 A` is incomplete
without its fuse, time-limit, and lead/jack information.

## LM2596 converter specimens

The user identifies all three converters as identical HW-411 modules. They are
still assigned durable functional names so voltage settings and measurements
cannot be confused.

The photographed specimen in `hw-411-top.jpeg` and `hw-411-bottom.jpeg` has:

- bottom silkscreen `LM2596:DC-DC` and `HW-411`, with clearly marked `IN+`,
  `IN-`, `OUT+`, and `OUT-` pads;
- a 100 uF, 50 V input electrolytic and 220 uF, 35 V output electrolytic;
- a trimmer marked `W103`; and
- a large shielded inductor whose marking appears to be `47R`, although the
  photograph does not resolve that marking confidently.

The converter IC and diode top marks are not legible enough in the supplied
photograph to claim an exact manufacturer or subpart. The user confirms that
the photographed HW-411 construction represents all three converters.

| Assignment | Specimen ID and photographs | Observed markings and population | Documented limits to record | Current record |
| --- | --- | --- | --- | --- |
| LM2596 A — Bluetooth ESP32, 5.0 V | HW-411-A; represented by supplied HW-411 photographs | Identical photographed HW-411 population | Input range, adjustable-output range, continuous/peak output current for the complete module, thermal conditions, polarity | Selected for phase 1; load qualification pending |
| LM2596 B — HW-627 and two motors, initially 4.0 V | HW-411-B; represented by supplied HW-411 photographs | Identical photographed HW-411 population | Same fields, with particular attention to sustained current and thermal derating | Selected for phase 1; load qualification pending |
| LM2596 C — camera ESP32, 5.0 V, phase 2 | HW-411-C; represented by supplied HW-411 photographs | Identical photographed HW-411 population | Same fields | Reserved and electrically disconnected; camera phase deferred |

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
| Additional markings | User reports no additional HW-627 markings beyond those already photographed and recorded |
| Binding evidence | `platformio-esp32-lessons/02-bluetooth-motor-servo-control/HW-627-INSPECTION.md` |

Powered two-motor work remains bounded by the unresolved module thermal and
current-limit details plus measured behavior. The DRV8833 headline peak rating
is not a complete-module continuous rating.

## Motor specimens

Label the physical motors `MOTOR-A` and `MOTOR-B`; do not exchange them after
individual results are recorded. Both specimens are identified as the same
1:48 TT DC gearbox-motor type with two 200 mm leads terminated in 0.1-inch male
connectors. The supplied figures below are representative measurements from
one motor of this type, not measurements of either installed specimen.

| Field | MOTOR-A | MOTOR-B |
| --- | --- | --- |
| Motor identification | TT DC Gearbox Motor, 1:48 | TT DC Gearbox Motor, 1:48 |
| Manufacturer and exact model/SKU | Not recorded | Not recorded |
| Body, gearbox and label photographs | Not recorded | Not recorded |
| Supplied leads and connector | Two 200 mm wires with 0.1-inch male connectors | Two 200 mm wires with 0.1-inch male connectors |
| Documented voltage range | 3–6 V DC | 3–6 V DC |
| Gear ratio | 1:48 | 1:48 |
| Representative no-load data | 150 mA at 120 RPM and 3 V; 155 mA at 185 RPM and 4.5 V; 160 mA at 250 RPM and 6 V | Same motor-type reference data; specimen measurement pending |
| Representative stall data | 1.1 A at 3 V; 1.2 A at 4.5 V; 1.5 A at 6 V | Same motor-type reference data; specimen measurement pending |
| Factory noise-suppression capacitor | Present according to the user; exact value, type, terminal placement, and condition still require visual inspection | Present according to the user; exact value, type, terminal placement, and condition still require visual inspection |
| Actual no-load current and conditions | Not measured | Not measured |
| Actual startup current | Not measured | Not measured |
| Shaft/gearbox condition before test | Not recorded | Not recorded |

Do not deliberately stall either curriculum specimen to reproduce the supplied
stall figures. At 4.5 V, use 1.2 A per motor and 2.4 A for the two-motor
simultaneous-stall case as planning bounds, while measuring only free-start and
unloaded running behavior. The 0.1-inch connectors may simplify temporary
connections, but this record does not qualify a solderless breadboard power
rail or lightweight signal jumpers for motor or stall current.

## ESP32 boards

| Assignment | Confirmed identity | Ratings and identity still needed |
| --- | --- | --- |
| Bluetooth controller | Actual 30-pin board photographed in `bluetooth-esp32-top.jpeg` and `bluetooth-esp32-bottom.jpeg`; bottom silkscreen reads `ESP32 DEVKITV1`; module shield reads `ESP-32`, `WiFi`, `BT`, and `WiFi+BT SoC Inside`; micro-USB connector plus `EN` and `BOOT` buttons; repository PlatformIO board ID `esp32doit-devkit-v1` | The generic module shield does not expose an exact Espressif WROOM model. U1 appears to be an AMS1117-family regulator and U2 appears to be a Silicon Labs CP2102-family USB/UART device, but the photographs do not resolve their complete top marks confidently. Confirm the exact regulator suffix and documented 5 V/VIN input limits if a hard component rating is needed. |
| Camera controller | Deferred. Exploration and supplied image identify an `ESP32-S3-CAM`-style board with ESP32-S3 N16R8, OV3660 camera, dual USB-C connectors, and header pins marked `5V` and `GND` | Phase 2 remains electrically disconnected; record manufacturer, exact model/revision, module and regulator markings, 5 V input specification, and installed sensor identity before revisiting it |

External 5 V and USB 5 V must not be connected simultaneously unless the exact
board's documented power-path isolation has been verified.

## Completion gate for task 1.1

Task 1.1 may be checked only after:

- every `Not recorded` identity needed for the selected build has been replaced
  with an observed exact part or an explicit decision that the item is absent;
- every voltage, current, fuse, connector, wire and temperature limit that
  bounds the activity has a manufacturer/supplier source tied to that exact
  part;
- the supplied photographs establish the common HW-411 construction, durable
  `HW-411-A`, `HW-411-B`, and `HW-411-C` labels distinguish their assignments,
  and the two motors retain distinct specimen labels;
- the exact meter manual and installed current fuse are verified; and
- unresolved ratings are converted into explicit no-power blockers rather than
  filled with generic assumptions.

The user's charged, protected-pack and sufficient-capacity assumptions permit
continued phase-1 documentation and testing. Charging, cell balancing, BMS
design, and pack construction are not acceptance gates for this activity.
