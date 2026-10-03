# ESP32 rover power architecture

This exploration defines a staged path from the Bluetooth actuator bench work
to a battery-powered, two-motor electrical assembly. It begins with the
Bluetooth ESP32 and both TT motors. A camera branch is reserved for a later
phase and remains disconnected until the first phase passes its checks.

This is a power-integration activity, not a completed mobile-rover design.

## Relationship to Lesson 02

[`platformio-esp32-lessons/02-bluetooth-motor-servo-control`](../../platformio-esp32-lessons/02-bluetooth-motor-servo-control/README.md)
is the prerequisite. That lesson remains unchanged:

- the ESP32 is powered by USB;
- one motor and one servo use a separate current-limited bench supply;
- only one HW-627/DRV8833 bridge is exercised; and
- batteries, rover wiring, and chassis motion remain outside the lesson.

This exploration reuses Lesson 02's neutral-before-arm, explicit-disarm,
report-freshness, disconnect, reversal-delay, coast, and failsafe behavior. It
does not retroactively authorize battery operation in Lesson 02.

The work here is limited to secured, unloaded motors on the bench. It does not
include wheels, moving-rover tests, deliberate stalls, restrained shafts,
independent left/right commands, differential steering, or the Lesson 02
servo. Mirroring one safe motor command to both bridges is only a way to
exercise the electrical load.

## Baseline decisions

- Use a charged, protected 3S 18650 pack as the source.
- Use three separate, non-paralleled HW-411/LM2596 buck-converter branches.
- Power the Bluetooth ESP32 DevKit through its `5V`/`VIN` input at 5.0 V.
- Begin the motor rail at 4.0 V and raise it only as needed, never above 4.5 V
  in this activity.
- Power one TT motor from each HW-627/DRV8833 H-bridge.
- Reserve the third 5.0 V branch for the ESP32-S3 camera board; leave that
  converter and load disconnected during phase 1.
- Join grounds at the defined distribution point, but never join converter
  positive outputs.

The target architecture includes a pack-side fuse and a reachable DC-rated
cutoff. The current [hardware record](RoverPowerHardwareRecord.md) documents
that their selection is deferred. The diagram below is therefore the target
architecture, not a claim that every component is already installed.

## Three-branch block diagram

```text
                             TARGET 3S ARCHITECTURE

  charged, protected 3S pack
       12.6 V full; 11.1 V nominal
                 |
          [pack-side fuse]
                 |
       [DC-rated SPST cutoff]
                 |
        +--------+---------+--------------------+
        |  positive distribution / branch point |
        +--------+---------+--------------------+
                 |         |                    |
                 |         |                    `-- disconnected in phase 1
                 |         |
          +------v---+ +---v------+          +--v-------+
          | HW-411 A | | HW-411 B |          | HW-411 C |
          |  5.0 V   | | 4.0 V    |          |  5.0 V   |
          +------|---+ | to 4.5 V |          +--|-------+
                 |     +---|------+             |
                 |         |                    |
       +---------v--+ +----v---------+   +------v-----------+
       | ESP32      | | HW-627 /     |   | ESP32-S3 N16R8   |
       | DEVKITV1   | | DRV8833      |   | + OV3660 camera  |
       | Bluetooth  | +--|--------|--+   | phase 2 only     |
       +------------+    |        |      +------------------+
                      motor A  motor B
                      1:48 TT  1:48 TT

  protected-pack negative
                 |
        +--------v------------------------------------------+
        | ground distribution / star reference             |
        +--------+----------------+-------------------------+
                 |                |                 |
             HW-411 A         HW-411 B          HW-411 C
                 |                |                 |
          ESP32 ground     HW-627 and control   camera ground

  Rules: converter positive outputs remain separate; all control circuits
  share the defined ground reference; motor current does not flow through an
  ESP32 header or signal-return path.
```

## Why 3S is the baseline

| Source | Approximate pack range | Assessment for this architecture |
| --- | ---: | --- |
| 2S | 8.4 V full, about 6 V near discharge | Not selected. An LM2596 buck has little regulation margin for a robust 5 V ESP32 rail after battery sag and converter dropout. |
| 3S | 12.6 V full, 11.1 V nominal, about 9 V near the intended lower boundary | Recommended. It gives both 5 V branches useful buck-converter headroom without the additional voltage and conversion loss of 4S. |
| 4S | 16.8 V full, 14.8 V nominal | Not needed for these loads. It increases switch, capacitor, wiring, and converter stress and wastes more power in the buck converters. It requires separate qualification before use. |

Pack voltage alone does not establish usable capacity. The protected pack,
connections, and each converter must tolerate the measured load without severe
droop, a protection trip, excessive heating, or an ESP32 reset.

## Phase boundaries

### Phase 1: Bluetooth ESP32 and two motors

Only HW-411 A and B are connected to loads. HW-411 C and the camera remain
electrically disconnected.

Phase 1 proceeds in this order:

1. Record source polarity and voltage with all loads disconnected.
2. Set HW-411 A to 5.0 V and HW-411 B initially to 4.0 V with no loads
   connected.
3. Measure the Bluetooth ESP32 branch separately during boot, connection, and
   representative reporting.
4. Bench-test the HW-627 and motor A, motor B, and then both secured motors.
   Record free-start and unloaded-running behavior; do not stall a motor.
5. Combine the two branches and repeat voltage and current observations from
   safe idle through conservative simultaneous motor operation.

The motor rail may be raised toward 4.5 V only if an unloaded motor does not
start reliably at 4.0 V. Remove power before adjustment and repeat every
affected voltage, current, droop, and temperature check.

Phase 1 passes only after the source and both load rails remain in range, the
current records are attributable, both motors work separately and together,
and there is no unexplained reset, Bluetooth interruption, driver fault,
persistent non-turning energized motor, severe droop, odor, damaged insulation,
or excessive heating.

### Phase 2: camera branch

Phase 2 is intentionally deferred. After phase 1 passes, HW-411 C can be set to
5.0 V with its load disconnected and the camera branch can be qualified by
itself before any all-branch test. USB 5 V and external 5 V must not be applied
to either ESP32 simultaneously unless power-path isolation for that exact board
has been verified.

Disconnecting HW-411 C returns the assembly to the phase-1 configuration.
Removing the pack and converters returns the work to Lesson 02's original USB
and bench-supply arrangement.

## Planning power budget

The values in this section size the first tests. They are not proof of battery,
converter, driver, or wiring capacity. `Measured` remains pending until the
corresponding staged test is performed.

| Load | Rail | Expected operating current | Conservative planning current | Planning output power | Measured |
| --- | ---: | ---: | ---: | ---: | --- |
| Bluetooth ESP32 DEVKITV1 | 5.0 V | 0.15–0.20 A with Bluetooth active | 0.50 A | 2.50 W | Pending |
| TT motor A | 4.5 V maximum | 0.155 A no-load reference | 1.20 A supplied stall reference | 5.40 W | Pending |
| TT motor B | 4.5 V maximum | 0.155 A no-load reference | 1.20 A supplied stall reference | 5.40 W | Pending |
| Phase-1 output total | mixed | At least 2.40 W plus driver losses using the reference no-load currents | Simultaneous arithmetic planning case | 13.30 W | Pending |
| ESP32-S3 camera branch, phase 2 | 5.0 V | Unknown until measured | 1.00 A placeholder | 5.00 W | Deferred |
| Phase-2 output total | mixed | Unknown until measured | Simultaneous arithmetic planning case | 18.30 W | Deferred |

The 1.20 A motor values are supplied stall references at 4.5 V, not measured
startup currents and not a test target. The procedure never deliberately
stalls either motor. A handheld meter may also miss a short startup transient,
so an observed maximum must not be represented as the exact peak.

Using 80% aggregate conversion efficiency only as a conservative planning
assumption gives:

| Planning case | Load-side power | Estimated pack power | Pack current at 12.6 V | Pack current at 11.1 V | Pack current at 9.0 V |
| --- | ---: | ---: | ---: | ---: | ---: |
| Phase 1 | 13.30 W | 16.63 W | 1.32 A | 1.50 A | 1.85 A |
| Phase 2 | 18.30 W | 22.88 W | 1.82 A | 2.06 A | 2.54 A |

These totals do not establish that one HW-411 can continuously deliver the
motor branch's arithmetic case or that the HW-627 can sustain both bridges at
their stall references. Voltage droop, resets, faults, and temperature are part
of the acceptance evidence.

## Expansion boundary

No surplus actuator power is claimed yet. Arithmetic difference between a
seller's battery or converter rating and this table is not demonstrated spare
capacity. The available margin is constrained independently by:

- the protected pack and its connections;
- the assigned HW-411 branch and its temperature rise;
- the motor driver and each bridge;
- wiring and connector voltage drop; and
- measured startup behavior of the complete assembly.

A second pair of these TT motors would add as much as 10.8 W of load-side motor
planning demand at the supplied 4.5 V stall reference. It would require another
driver, normally another qualified converter branch, distribution protection,
and a new staged test; it must not be attached to the existing HW-627 outputs.

A stepper motor similarly requires its own current-regulating stepper driver
and a budget based on phase current and driver configuration. A servo requires
its measured or documented peak current, a suitable regulated rail, and a
driver/control arrangement that keeps its transient current out of the ESP32
rail. None of these future loads is authorized by unused arithmetic wattage.

After phase-1 measurements, calculate provisional margin separately for each
power-path element:

```text
provisional margin = documented component limit
                   - highest applicable measured demand
                   - explicit engineering reserve
```

The smallest margin in the path is the binding one. A later change must define
the new actuator, driver, converter, protection, wiring, and validation plan
before expansion.

## Point-to-point wiring

This table describes the target architecture shown above. The hardware record
currently defers the pack fuse and cutoff; those rows therefore describe the
approved design, not installed hardware. Keep all power removed while making
or changing any connection.

### Conductor conventions

| Use | Conductor | Color and termination |
| --- | --- | --- |
| Pack, distribution, and converter power | 18 AWG power wire; strand construction not recorded | Red for positive and black for negative; solder into the HW-411 through-holes with strain relief; use screw terminals sized to capture 18 AWG securely |
| Motor branch from HW-411 B to HW-627 | 18 AWG power wire; strand construction not recorded | Red to `VCC`, black to `GND`; use a direct screw-terminal path rather than carrying motor current through a solderless breadboard rail |
| Motor outputs | Supplied 200 mm motor leads; gauge not established | Keep each pair together and identify both ends before connection; do not extend with lightweight signal jumpers |
| Logic signals and signal-ground reference | Short insulated signal wire appropriate for ESP32 headers | Use distinct colors and labels; these wires carry logic/reference current only, never motor current |

Wire gauge does not establish connector suitability. Verify polarity, screw
capture, exposed-strand length, strain relief, and terminal tightness before
power is applied. No positive output from one converter may touch another
converter's positive output.

### Protected source and input distribution

| From | To | Conductor | Required state or value | Verification |
| --- | --- | --- | --- | --- |
| Protected 3S pack positive | Pack-fuse input | Red 18 AWG | Pack voltage, nominally 11.1 V and no more than 12.6 V when fully charged | Confirm polarity at the disconnected pack leads before inserting the fuse |
| Pack-fuse output | DC-rated SPST cutoff input | Red 18 AWG | Fuse and cutoff ratings must cover the documented DC load | Fuse and cutoff are target components currently marked deferred in the hardware record |
| Cutoff output | Positive star/distribution point | Red 18 AWG | Becomes live only when the cutoff is closed | Keep this point inaccessible to loose tools and strands |
| Protected 3S pack negative | Ground star/distribution point | Black 18 AWG | Common source return | Treat as the only high-current return meeting point |
| Positive star | HW-411 A `IN+` | Red 18 AWG | Phase 1 connected | Verify `IN+`; do not use the output-side pad |
| Ground star | HW-411 A `IN-` | Black 18 AWG | Phase 1 connected | Verify `IN-` |
| Positive star | HW-411 B `IN+` | Red 18 AWG | Phase 1 connected | Verify `IN+`; do not use the output-side pad |
| Ground star | HW-411 B `IN-` | Black 18 AWG | Phase 1 connected | Verify `IN-` |
| Positive and ground stars | HW-411 C `IN+` and `IN-` | No phase-1 conductors | Both input connections physically disconnected and insulated during phase 1 | A switch left open is not the phase gate; remove or isolate the conductors |

The existing battery-holder screw terminals may serve as connection hardware
only when the target fuse, cutoff, and star order is preserved. Do not place a
cutoff only in an individual branch: it must remove the protected pack positive
from every converter branch.

### Phase-1 regulated power branches

| From | To | Conductor | Required state or value | Verification |
| --- | --- | --- | --- | --- |
| HW-411 A `OUT+` | Bluetooth ESP32 breakout `VIN`/5 V input | Red 18 AWG into the breakout screw terminal | 5.0 V, checked unloaded before connection | Measure polarity and voltage at `OUT+`/`OUT-`, then again at the ESP32 terminals |
| HW-411 A `OUT-` | Bluetooth ESP32 breakout `GND` | Black 18 AWG into the breakout screw terminal | Logic-branch return | This return goes to A, then to the star; it is not a motor-current path |
| HW-411 B `OUT+` | HW-627 `VCC` | Red 18 AWG into the motor-branch screw terminal | Initially 4.0 V; permitted range 4.0–4.5 V | Route directly to the driver supply connection, not through an ESP32 header or solderless breadboard rail |
| HW-411 B `OUT-` | HW-627 `GND` | Black 18 AWG into the motor-branch screw terminal | Motor-branch return | Route directly back to B; do not send motor return through ESP32 ground wiring |

The HW-627 may be supported by a breadboard, but its `VCC`, `GND`, and motor
output current paths must not depend on breadboard bus strips or lightweight
jumper contacts. The ESP32 and HW-627 still need a short logic-reference ground
connection as shown below; that lead does not replace the motor return.

### HW-627 control and motor outputs

| From | To | Connection purpose | Phase-1 rule |
| --- | --- | --- | --- |
| Bluetooth ESP32 GPIO 26 | HW-627 `IN1` and `IN3` | Input-1 signal fanned out to both bridges | One GPIO drives both high-impedance inputs; do not connect any output terminals together |
| Bluetooth ESP32 GPIO 27 | HW-627 `IN2` and `IN4` | Input-2 signal fanned out to both bridges | Preserves the existing PWM, direction, reversal-delay, coast, disarm, and failsafe behavior on both channels |
| Bluetooth ESP32 GPIO 13 | HW-627 `EEP`/`nSLEEP` | Driver enable/sleep control | Firmware holds the driver asleep during safe initialization and enables it only afterward |
| Bluetooth ESP32 `GND` | HW-627 `GND` | Short logic-signal reference | Logic/reference current only; the 18 AWG HW-411 B return carries motor current |
| HW-627 `ULT`/`nFAULT` | No connection | Optional active-low fault output | Leave insulated and unconnected until firmware and a GPIO are explicitly assigned |
| HW-627 `J2` | No connection | Unknown configuration jumper | Leave open exactly as photographed |
| HW-627 `OUT1` | Motor A lead A1 | First H-bridge output | Record lead color and resulting shaft direction; remove power before swapping leads |
| HW-627 `OUT2` | Motor A lead A2 | First H-bridge output | Never join to `OUT3`, `OUT4`, ground, or a supply rail |
| HW-627 `OUT3` | Motor B lead B1 | Second H-bridge output | Record lead color and resulting shaft direction; remove power before swapping leads |
| HW-627 `OUT4` | Motor B lead B2 | Second H-bridge output | Never parallel either bridge or either motor winding |

The mirrored input wiring is for a bounded electrical load test, not
differential drive. Channel-isolated testing is performed with power removed
while the unused motor is disconnected from its two output terminals.

### Disconnected phase-2 branch

| From | To | Phase-1 state | Phase-2 target |
| --- | --- | --- | --- |
| HW-411 C input | Positive and ground stars | Physically disconnected and insulated | Connect only after the phase-1 gate passes |
| HW-411 C `OUT+` | Camera-board `5V` input | No conductor to the camera | Set and verify 5.0 V unloaded before connection |
| HW-411 C `OUT-` | Camera-board `GND` | No conductor to the camera | Connect as the camera return and common control reference |
| Camera USB-C connectors | USB source | Unplugged | Keep unplugged whenever HW-411 C supplies external 5 V unless isolation for the exact board is verified |

### USB and external-power exclusion

The photographed Bluetooth `ESP32 DEVKITV1` and the deferred camera board do
not have verified power-path isolation. Use one positive 5 V source at a time:

- For phase-1 pack operation, unplug USB from the Bluetooth ESP32 before
  connecting HW-411 A to `VIN`/5 V.
- For firmware upload or USB serial work, open and disconnect battery power,
  disconnect HW-411 A from the ESP32 positive input, and leave HW-411 B off.
- For future camera operation, unplug both camera USB-C connectors before
  connecting HW-411 C to the camera `5V` input.
- A common ground used only for an intentionally configured measurement does
  not authorize tying two positive 5 V sources together.

Before energizing any branch, verify red-to-positive and black-to-negative at
the source, converter input, converter output, screw terminal, and load. An
HW-411 adjusted correctly with reversed output leads is still a destructive
connection.

## Follow-on sections

Suppression and capacitance details, authoritative source references,
multimeter procedure, measurement records, shutdown steps, and the acceptance
checklist are added by subsequent tasks in this OpenSpec change. The current
hardware inventory and known markings are in
[RoverPowerHardwareRecord.md](RoverPowerHardwareRecord.md).
