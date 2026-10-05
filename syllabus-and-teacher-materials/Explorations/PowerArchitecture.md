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

The values in this section size the first tests. Every current shown before a
staged test is a **planning estimate**, even when it is based on supplier test
data. It is not proof of battery, converter, driver, or wiring capacity.
`Measured` remains pending until the corresponding staged test is performed.

| Load | Rail | Expected operating current | Conservative planning current | Planning output power | Measured |
| --- | ---: | ---: | ---: | ---: | --- |
| Bluetooth ESP32 DEVKITV1 | 5.0 V | 0.15–0.20 A planning estimate with Bluetooth active | 0.50 A planning allowance | 2.50 W | Pending |
| TT motor A | 4.5 V maximum | 0.155 A supplier reference used as a planning estimate | 1.20 A supplier stall reference used as a planning bound | 5.40 W | Pending |
| TT motor B | 4.5 V maximum | 0.155 A supplier reference used as a planning estimate | 1.20 A supplier stall reference used as a planning bound | 5.40 W | Pending |
| Phase-1 output total | mixed | At least 2.40 W plus driver losses using the reference no-load currents | Simultaneous arithmetic planning case | 13.30 W | Pending |
| ESP32-S3 camera branch, phase 2 | 5.0 V | Unknown until measured | 1.00 A placeholder planning allowance, not a documented demand | 5.00 W | Deferred |
| Phase-2 output total | mixed | Unknown until measured | Simultaneous arithmetic planning case | 18.30 W | Deferred |

The motor figures come from a supplier's test of one example of the same
described motor type. They are planning estimates for these two specimens, not
their measurements. In particular, the 1.20 A values are supplier stall
references at 4.5 V, not measured startup currents and not a test target. The
procedure never deliberately stalls either motor. Recorded free-start and
unloaded-running values replace the corresponding planning values for later
budgets; the supplier stall figure remains only an unverified upper reference.
A handheld meter may also miss a short startup transient, so an observed
maximum must not be represented as the exact peak.

The **80% aggregate conversion efficiency is a planning estimate**, not a
Texas Instruments datasheet value and not a measurement of any HW-411. Actual
efficiency depends on input voltage, output voltage, current, the converter IC,
diode, inductor, capacitors, PCB layout, and temperature. Replace 80% in later
budgets with input/output measurements for each assigned HW-411. Until then,
using it as a conservative arithmetic assumption gives:

| Planning case | Load-side power | Estimated pack power | Pack current at 12.6 V | Pack current at 11.1 V | Pack current at 9.0 V |
| --- | ---: | ---: | ---: | ---: | ---: |
| Phase 1 | 13.30 W | 16.63 W | 1.32 A | 1.50 A | 1.85 A |
| Phase 2 | 18.30 W | 22.88 W | 1.82 A | 2.06 A | 2.54 A |

These totals do not establish that one HW-411 can continuously deliver the
motor branch's arithmetic case or that the HW-627 can sustain both bridges at
their stall references. Voltage droop, resets, faults, and temperature are part
of the acceptance evidence.

The phase-2 camera's **1.00 A allowance is also only a planning estimate**. It
is not an Espressif or camera-board specification and does not claim that the
board will draw 1.00 A. The exact third-party camera board, regulator, OV3660
operating mode, Wi-Fi conditions, frame size, frame rate, illumination, and
firmware can all affect demand. Keep phase 2 disconnected until the board is
identified; then replace this allowance with measured boot, Wi-Fi-connect, and
representative-streaming current.

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

## Suppression, bypass, and energy storage

Capacitors in this architecture have different jobs. The motor-terminal parts
reduce brush noise at its source, the driver ceramic supplies high-frequency
switching current, and the bulk capacitors reduce slower rail droop. One part
does not replace the others, and additional capacitance does not repair an
undersized converter, driver, connector, wire, or battery.

The photographed HW-411 already carries a 100 uF, 50 V input electrolytic and
a 220 uF, 35 V output electrolytic. Keep those installed, but do not treat them
as load-local capacitance when 18 AWG wiring and screw terminals separate the
converter from the ESP32 or HW-627.

### Starting capacitor plan

| Location and purpose | Starting configuration | Polarity and voltage rating | Placement |
| --- | --- | --- | --- |
| Motor A brush-noise suppression | Retain its factory-installed terminal capacitor. A nominal 100 nF ceramic directly across the two motor terminals is the target arrangement; do not add a duplicate merely because the existing value is unmarked. | Must be non-polarized. Prefer a ceramic rated at least 25 V; 50 V is a common robust choice for motor-terminal transients. | Directly across Motor A's two metal terminals with the shortest practical leads. |
| Motor B brush-noise suppression | Retain its factory-installed terminal capacitor under the same inspection rule as Motor A. | Must be non-polarized and suitably voltage-rated; use the same target rating as Motor A if replacement is required. | Directly across Motor B's two metal terminals with the shortest practical leads. |
| HW-627 high-frequency bypass | 100 nF ceramic from `VCC` to `GND`. The module's visible `0.01uF`, `2.2uF`, and `10uF` footprints do not establish this external bypass. | Non-polarized; at least 16 V, with 25 V or higher acceptable. | At the HW-627 supply pins, not at the far end of the branch wire or on a distant breadboard rail. |
| HW-627 local bulk storage | Begin with approximately 470 uF electrolytic from `VCC` to `GND` because two motors share the rail. | Positive lead to `VCC`; negative stripe to `GND`. At least 10 V; 16 V preferred for the 4.0–4.5 V rail. | Beside the HW-627 power connection with short, low-impedance leads. |
| Bluetooth ESP32 local bulk | Begin with 100–220 uF electrolytic from `VIN`/5 V to `GND`. | Positive lead to `VIN`/5 V; negative stripe to `GND`. At least 10 V; 16 V preferred. | At the ESP32 breakout screw terminals or immediately beside the board's power pins. |
| Protected 3S distribution bulk | Optional starting range of 470–1000 uF electrolytic across positive and ground distribution. Add only after the source and branch wiring are verified. | Positive lead to the positive star; negative stripe to the ground star. At least 25 V for a 12.6 V fully charged 3S source. | Downstream of the target fuse and cutoff, at the star distribution point. Keep leads short and insulated. |
| Camera ESP32 local bulk, phase 2 only | Begin with 220–470 uF electrolytic from camera `5V` to `GND` when that branch is eventually qualified. | Positive lead to `5V`; negative stripe to `GND`. At least 10 V; 16 V preferred. | At the camera-board power input, not back at HW-411 C. Remains absent or disconnected in phase 1. |

Before relying on the installed motor capacitors, inspect both motors with power
removed:

1. Confirm each capacitor is physically connected across that motor's two
   terminals, rather than from one terminal to the metal case.
2. Confirm it is a non-polarized ceramic part. Record any readable value or
   marking; do not invent a value when the part is unmarked.
3. Check for cracked coating, broken leads, solder bridges, or a lead touching
   the motor can unintentionally.
4. Retain an intact factory capacitor even when its exact value is unreadable.
   Replace or supplement it only when its construction is unsuitable or later
   measurements show a noise problem that warrants a controlled change.

If the factory part can be verified as approximately 100 nF directly across the
terminals, it already satisfies the starting motor-suppression configuration.
Installing another 100 nF in parallel is unnecessary. Do not add
terminal-to-case capacitors without a motor-manufacturer EMI scheme that calls
for them.

### Physical layout

- Twist Motor A's two leads together from `OUT1`/`OUT2` to Motor A, and twist
  Motor B's pair from `OUT3`/`OUT4` to Motor B. Keep each pair short and separate
  from ESP32 antenna, USB, and logic-signal wiring where practical.
- Keep the motor-terminal capacitors at the motors. Moving them to the HW-627
  end of 200 mm leads leaves the lead pair acting as a noise source.
- Keep the 100 nF driver ceramic and 470 uF driver bulk capacitor at the HW-627
  supply pins. A capacitor at HW-411 B does not provide the same high-frequency
  current loop.
- Keep the Bluetooth bulk capacitor on the ESP32 side of its branch wiring and
  the distribution bulk capacitor at the star. They serve different current
  paths.
- Insulate all exposed capacitor leads and mechanically secure large
  electrolytics so vibration or handling cannot fatigue their connections.

### Polarity, power-off, and adjustment rules

Ceramic capacitors in this plan are non-polarized. Every electrolytic is
polarized: verify its `+` lead and negative stripe against the rail before
power is applied. Also read the printed voltage rating; physical size and color
are not evidence of a safe rating.

Open and disconnect battery power before installing, removing, or moving a
capacitor. Allow bulk capacitors to discharge and verify the rail is near 0 V
before touching conductors. Larger bulk values increase connection inrush and
stored energy, so change one value at a time and repeat the applicable source,
droop, temperature, and shutdown checks.

Treat every value above as a provisional starting configuration to be evaluated
by measurements. Increase bulk capacitance only when recorded voltage droop or
resets justify a trial. Stop and correct wiring, supply, driver, or load
problems rather than adding capacitance to conceal persistent stalls, severe
droop, overheating, or a protection trip.

### No single flyback diode across a reversible motor

Do not install one conventional flyback diode across either motor or across
`OUT1`/`OUT2` or `OUT3`/`OUT4`. Each H-bridge intentionally reverses motor
terminal polarity. A diode that blocks one direction becomes forward-biased in
the other and can effectively short the bridge output.

The DRV8833 bridge provides the bidirectional switching and winding-current
recirculation paths. The external motor-noise component is the non-polarized
terminal capacitor already fitted to each motor, not a single polarized diode.

## Component sources and evidence limits

Use these primary manufacturer or supplier references to understand the named
components. They do not override the staged measurements, and they do not
certify a visually similar module or board whose complete design is unknown.

| Component | Primary source | What the source establishes here | What it does not establish |
| --- | --- | --- | --- |
| 1:48 TT motors | [Adafruit product 3777: TT Motor, 200 RPM, 3–6 VDC](https://www.adafruit.com/product/3777) | The product description matches the supplied 1:48 ratio, 200 mm leads, 3–6 V range, and single-sample no-load/stall figures used as planning estimates. | It does not prove the exact SKU of MOTOR-A or MOTOR-B, their startup current, or their condition. Do not deliberately reproduce its stall test. |
| DRV8833 IC | [Texas Instruments DRV8833 datasheet](https://www.ti.com/lit/ds/symlink/drv8833.pdf) | The IC is a dual H-bridge intended to drive two DC motors and documents its supply range, package-dependent current ratings, current regulation, recirculation behavior, and protection functions. | It does not establish the HW-627's package option, thermal pad/layout, current-limit configuration, connector capability, or safe complete-module continuous current. |
| LM2596 IC | [Texas Instruments LM2596 datasheet](https://www.ti.com/lit/ds/symlink/lm2596.pdf) | The IC datasheet documents the regulator family, reference designs, test conditions, external-component dependence, efficiency curves, and thermal/layout considerations. | It does not prove that the HW-411 uses a genuine TI IC, reproduce its reference design, sustain 3 A, or achieve 80% efficiency at either assigned load. |
| ESP32-WROOM family | [Espressif ESP32-WROOM-32 datasheet](https://documentation.espressif.com/esp32-wroom-32_datasheet_en.html) | It supplies family-level module voltage, radio, and current-consumption context. | The photographed board's shield does not identify an exact Espressif module variant, and the datasheet does not specify the complete DEVKITV1 board's 5 V input or Bluetooth workload current. |
| ESP32-S3-WROOM-1 family | [Espressif ESP32-S3-WROOM-1/1U datasheet](https://documentation.espressif.com/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf) | It supplies family-level module voltage, Wi-Fi, memory-variant, and current-consumption context for later identification. | It does not specify the unidentified camera carrier board, its 5 V power path/regulator, OV3660 sensor load, or streaming workload current. |

The evidence labels in this guide mean:

- **Datasheet limit** applies only when the exact manufacturer part, package,
  circuit, and stated test conditions have been established.
- **Supplier reference used as a planning estimate** is published test data for
  one supplier sample; it is not a measurement of either curriculum specimen.
- **Planning estimate** or **planning allowance** is provisional arithmetic for
  choosing the first meter range and test boundary. It must not be reported as
  observed demand, efficiency, capacity, or surplus.
- **Measured** means a value recorded from the identified assembly at a stated
  measurement point, rail voltage, and operating state. Later tasks add the
  measurement forms and replace applicable planning values with that evidence.

## Multimeter safety procedure

Voltage and current measurements use different meter connections. Confusing
them can place the meter's low-resistance current shunt directly across the
pack or a converter output. A current-input fuse is a last line of protection,
not permission to probe a voltage rail.

The general connection rules agree with Fluke's
[digital-multimeter jack guidance](https://www.fluke.com/en-us/learn/best-practices/test-tools-basics/digital-multimeters/digital-multimeter-jacks)
and its [current-measurement explanation](https://media.fluke.com/ade6b718-4577-4b57-903b-b10600664c67_original%20file.pdf),
but the manual for the actual meter controls its allowed ranges, fuses, and
measurement duration.

The reported meter has a fused 10 A input, but its model, installed fuse part,
fuse interrupt rating, lead ratings, accuracy, and 10 A time limit are not yet
recorded. Before any series-current test, identify the meter and follow its
manual to inspect or test the current-input fuse. Confirm that the selected
jack, range, fuse, leads, and permitted measurement duration cover the planned
DC test. Do not bypass a fuse or substitute an unspecified fuse. If these
details cannot be confirmed, perform voltage checks only and obtain a suitable
meter or DC current probe before continuing.

### Voltage mode: probe in parallel

Voltage mode observes the difference between two points without intentionally
opening the circuit:

```text
positive o----------------o load +
         |                |
        red               load
    [ meter: DC V ]       |
        black             |
         |                |
negative o----------------o load -
       parallel connection
```

1. Put the black lead in `COM` and the red lead in the jack marked for volts
   (`V` or `V/ohm`), never an `A`, `10A`, `mA`, or `uA` jack.
2. Select DC volts and a range above the highest expected voltage. The maximum
   planned source voltage is 12.6 V for a fully charged 3S pack; the regulated
   rails are 5.0 V and 4.0–4.5 V.
3. Touch the black probe to the applicable negative/ground point and the red
   probe to the applicable positive point. This is a **parallel** connection;
   do not cut or disconnect the conductor being measured.
4. Record the two exact probe points, polarity, operating state, and voltage.
   A negative display normally indicates reversed probe polarity; remove the
   probes and resolve polarity before connecting a load.
5. Remove the red probe first and then the black probe. Keep the meter in this
   voltage configuration unless a current measurement is immediately required.

### Current mode: insert in series

Current mode temporarily replaces one conductor so all current at the named
measurement boundary flows through the meter:

```text
positive o---- red [ meter: DC A ] black ----o load +
negative o-----------------------------------o load -
                series insertion
```

Use the positive conductor for these insertions so the common ground reference
between the ESP32 and HW-627 remains intact. Suitable named boundaries include
the protected-pack positive output for whole-pack current, HW-411 A `OUT+` for
Bluetooth-branch output current, and HW-411 B `OUT+` for motor-branch output
current. The record must identify which boundary is used; currents measured on
converter inputs and outputs are not interchangeable.

Follow this sequence for every current measurement:

1. Disarm the controller and command coast/zero output.
2. Open the physical cutoff **and disconnect the protected pack**. While the
   separate cutoff remains deferred, physical pack disconnection is mandatory;
   do not treat firmware, an unplugged USB cable, or a switch of unverified DC
   suitability as isolation.
3. With the meter still configured for DC voltage, verify the conductor to be
   opened is at or near 0 V after stored energy has decayed. Remove both probes.
4. Confirm the planned current boundary and expected upper demand. Inspect the
   leads and use the meter manual to verify the fuse and any maximum-duration or
   cooldown rule for the selected input.
5. Move the red lead to the **fused high-current/10 A jack** and select DC amps
   on the highest suitable fused range. Begin there for every motor-branch or
   whole-pack test because motor startup is transient and the present planning
   case exceeds common mA-input ratings. Never use an unfused current input.
6. Open only the selected positive conductor and connect the meter across that
   break: red toward the source, black toward the load. Secure the connections
   so a loose probe cannot open the motor circuit or touch an adjacent terminal.
7. Have a second person or teacher verify: pack disconnected, correct boundary,
   series path, red lead in the intended fused current jack, DC-current range,
   and no meter lead placed across positive and negative.
8. Reconnect the pack and close the cutoff only for the bounded test. Keep hands
   away from the wiring. Record the measurement boundary, rail voltage,
   operating state, observed current, and whether min/max capture was used.
9. Open the cutoff immediately after the reading, disconnect the pack, and wait
   for motion to stop and stored energy to decay. Never move a lead, change a
   jack, change a current range, or remove the series meter while energized.
10. Remove the meter and restore the opened conductor with power disconnected.
    If a reading was negative or out of range, correct the setup only after this
    full power-removal sequence; do not swap live leads.

Start the Bluetooth-only measurement on the same verified high-current range.
A lower fused range may be used later only when its manual, fuse, time limit,
and the already observed upper current establish adequate margin. Repeat the
power-off rewiring sequence to change ranges. Be aware that a current meter's
burden voltage can lower the load voltage, especially on a lower range; recheck
the rail at the load after returning the meter to voltage mode.

### Mandatory restoration checkpoint

Complete this checkpoint after **every** current measurement, before anyone
performs another voltage check or puts the meter away:

- [ ] Cutoff open, protected pack physically disconnected, and test stopped.
- [ ] Series meter removed and the opened circuit conductor safely restored.
- [ ] Red lead moved out of `A`/`10A`/`mA` and back to the `V` or `V/ohm` jack.
- [ ] Black lead remains in `COM`.
- [ ] Selector set to DC volts on a range above 12.6 V, or meter switched off
      for storage after the lead has been restored.
- [ ] A second person or teacher states and verifies: **meter restored to
      voltage mode**.
- [ ] Before re-energizing, the next measurement's probe points and polarity
      are identified; after reconnection, voltage is probed only in parallel.

Never place a meter configured for current directly across the protected pack,
an HW-411 input or output, the HW-627 supply, a motor, or either ESP32 supply.
Never estimate a short startup transient as an exact peak merely because the
meter displayed a maximum; handheld-meter sample rate and min/max behavior are
part of the measurement limitations.

## Measurement records

Create a new record ID for each operating state. Use that ID in both the
electrical-reading table and the observation table so a current reading cannot
be separated from its voltage, load state, fault, connectivity, and thermal
context. Enter `not measured`, `not supported`, or `not applicable` instead of
leaving an ambiguous blank or writing zero.

With one meter, voltage and current readings are sequential, not simultaneous.
Return the meter to voltage mode using the mandatory checkpoint, reproduce the
same bounded operating state, and record the time of each reading. If the state
cannot be reproduced consistently, create separate record IDs and say so; do
not combine the readings into one claimed operating point.

### Test-session header

Complete one header for every bench session.

| Field | Record |
| --- | --- |
| Session ID, date, and time | ___ |
| Operator and teacher/checker | ___ |
| Protected-pack identification and starting voltage | ___ |
| Meter make/model and asset or serial ID | ___ |
| Current input, installed fuse, selected range, and allowed duration | ___ |
| Min/max feature available and its documented behavior | yes / no / unknown; details: ___ |
| HW-411 assignments and unloaded settings | A: ___ V; B: ___ V; C: disconnected / ___ V |
| Motor specimen and lead-orientation labels | MOTOR-A: ___; MOTOR-B: ___ |
| Firmware/build identity and relevant command settings | ___ |
| Ambient temperature and temperature instrument, if used | ___ |
| Camera branch state | physically disconnected / phase 2 authorized |

### Required operating-state matrix

The rows below name the minimum records. Do not skip directly to an integrated
or two-motor row because a later state happens to work. `Start` means a free,
unrestrained start of a secured motor; it never means deliberate stall.

| Record ID | Phase and configuration | Required operating state | Current boundary/classification | Required rail-voltage point or points |
| --- | --- | --- | --- | --- |
| `P1-SRC-01` | Source; converter loads disconnected | Pack connected through the intended distribution path, no loads | Current not required; source-voltage record | Protected-pack/distribution input positive to negative |
| `P1-A-01` | HW-411 A only, no ESP32 | Converter energized unloaded | Current optional; label as A input or A output if taken | HW-411 A `OUT+` to `OUT-` |
| `P1-A-02` | Bluetooth branch only | ESP32 boot | HW-411 A output: A `OUT+` toward ESP32 | A output and ESP32 `VIN`/5 V to its ground |
| `P1-A-03` | Bluetooth branch only | Bluetooth connected, neutral/disarmed | HW-411 A output | A output and ESP32 5 V input |
| `P1-A-04` | Bluetooth branch only | Representative active reporting | HW-411 A output | A output and ESP32 5 V input |
| `P1-B-01` | HW-411 B and HW-627; motors disconnected | Driver idle | HW-411 B output: B `OUT+` toward HW-627 | B output and HW-627 `VCC` to `GND` |
| `P1-B-A-START` | MOTOR-A only | Free start | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-A-FWD` | MOTOR-A only | Settled forward run | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-A-REV` | MOTOR-A only | Bounded reversal and settled reverse run | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-A-COAST` | MOTOR-A only | Coast command and stop | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-B-START` | MOTOR-B only | Free start | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-B-FWD` | MOTOR-B only | Settled forward run | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-B-REV` | MOTOR-B only | Bounded reversal and settled reverse run | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-B-COAST` | MOTOR-B only | Coast command and stop | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-AB-START` | Both motors | Conservative simultaneous free start | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-AB-RUN` | Both motors | Settled simultaneous run | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-AB-REV` | Both motors | Bounded reversal and settled reverse run | HW-411 B output | B output and HW-627 `VCC` |
| `P1-B-AB-COAST` | Both motors | Coast command and stop | HW-411 B output | B output and HW-627 `VCC` |
| `P1-I-01` | Integrated phase 1 | Safe neutral/disarmed idle | Whole pack: protected positive after intended fuse/cutoff path | Pack, ESP32 5 V input, and HW-627 `VCC` |
| `P1-I-02` | Integrated phase 1 | Bluetooth-connected idle | Whole pack | Pack, ESP32 5 V input, and HW-627 `VCC` |
| `P1-I-03` | Integrated phase 1 | MOTOR-A operating | Whole pack | Pack, ESP32 5 V input, and HW-627 `VCC` |
| `P1-I-04` | Integrated phase 1 | MOTOR-B operating | Whole pack | Pack, ESP32 5 V input, and HW-627 `VCC` |
| `P1-I-05` | Integrated phase 1 | Conservative simultaneous motor operation | Whole pack | Pack, ESP32 5 V input, and HW-627 `VCC` |
| `P2-C-01` | Camera branch only; phase 2 | Camera boot | HW-411 C output: C `OUT+` toward camera | C output and camera-board 5 V input |
| `P2-C-02` | Camera branch only; phase 2 | Wi-Fi connection | HW-411 C output | C output and camera-board 5 V input |
| `P2-C-03` | Camera branch only; phase 2 | Representative OV3660 streaming workload | HW-411 C output | C output and camera-board 5 V input |
| `P2-I-01` | All three branches; phase 2 | Bluetooth connected and camera streaming, motors idle | Whole pack | Pack, both ESP32 5 V inputs, and HW-627 `VCC` |
| `P2-I-02` | All three branches; phase 2 | Camera streaming and bounded simultaneous motor operation | Whole pack | Pack, both ESP32 5 V inputs, and HW-627 `VCC` |

Phase-2 rows remain unused until the phase-1 gate authorizes them. If the motor
rail is adjusted after a failed 4.0 V start, suffix the repeated IDs with the
actual setting, such as `P1-B-A-START-4V5`, and repeat every affected row.

### Electrical-reading table

Add rows as necessary when one operating state needs several voltage points.
The current measurement point must name the two nodes separated for series
insertion. `Converter side` must be one of `whole pack`, `converter input`,
`converter output`, or `not applicable`; do not describe an output measurement
as battery current.

| Record ID and reading time | Operating state and loads energized | Current measurement point (source node -> load node) | Converter side | Current range | Observed current (A) | Min/max capture (off / on / unsupported) | Observed displayed maximum (A) | Rail-voltage probe points | Baseline voltage (V) | Voltage in state (V) | Droop = baseline - state (V) |
| --- | --- | --- | --- | ---: | ---: | --- | ---: | --- | ---: | ---: | ---: |
| ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ |
| ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ |
| ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ | ___ |

For a stable state, `Observed current` is the displayed value after the state
settles. For a start or reversal, record the largest value actually displayed
in `Observed displayed maximum` and state whether min/max capture was enabled.
If the meter lacks that feature, write `unsupported`; never relabel a visually
noticed number as an exact transient peak. Record the baseline at the same
voltage probe points under the applicable preceding idle state. Use `not
applicable` for droop when there is no meaningful baseline.

### Behavior, fault, connectivity, and temperature table

Complete one observation row for every electrical record ID, including normal
results. `None observed` is evidence; a blank is not. Use instrumented
temperatures when available. Otherwise record a qualitative, touch-free
observation such as `no visible heating or odor`; do not touch energized parts
or spinning motors to estimate temperature.

| Record ID | Test duration | ESP32 reset or brownout evidence | Bluetooth/Wi-Fi connectivity evidence | HW-627 fault indication | Motor response and unexpected motion | Temperature observation (component, method, start/end or qualitative) | Odor, noise, damaged insulation, or protection trip | Outcome and corrective-action reference |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| ___ | ___ | none observed / details: ___ | stable / interrupted / not applicable; details: ___ | inactive / asserted / not observable; details: ___ | ___ | ___ | none observed / details: ___ | pass / stop / repeat; ___ |
| ___ | ___ | none observed / details: ___ | stable / interrupted / not applicable; details: ___ | inactive / asserted / not observable; details: ___ | ___ | ___ | none observed / details: ___ | pass / stop / repeat; ___ |
| ___ | ___ | none observed / details: ___ | stable / interrupted / not applicable; details: ___ | inactive / asserted / not observable; details: ___ | ___ | ___ | none observed / details: ___ | pass / stop / repeat; ___ |

If the HW-627 fault output is not connected or its module mapping remains
unknown, record `not observable`; do not infer an inactive fault merely because
the motors turn. Record the temperature location and method consistently—for
example `HW-411 B case, IR thermometer, 24 C start / 39 C end`—and include the
test duration. Any interrupted test retains its partial readings and is marked
`stop`, never silently rewritten as a passing run.

## Follow-on sections

Shutdown steps and the acceptance checklist are added by subsequent tasks in
this OpenSpec change. The current hardware inventory and known markings are in
[RoverPowerHardwareRecord.md](RoverPowerHardwareRecord.md).
