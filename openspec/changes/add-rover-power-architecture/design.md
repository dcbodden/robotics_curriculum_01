## Context

See `proposal.md` for motivation. Lesson 02 currently powers the ESP32 by USB, powers one motor and one servo from a regulated bench supply, uses only the `IN1`/`IN2` and `OUT1`/`OUT2` half of the inspected HW-627, and explicitly excludes batteries and rover wiring. The follow-on must preserve that validated lesson while reusing its Bluetooth arming and failsafe behavior as the control prerequisite for a separately documented portable-power experiment.

The HW-627 carries a DRV8833 but its package thermal implementation and current-regulation configuration are not established by the photographs. The two TT motors' approximately 1.2-to-1.5 A stall estimate at 6 V is also unverified. Lowering the physical motor rail to 4.0-to-4.5 V is therefore a risk-reduction decision, not proof that sustained stall is safe.

## Goals / Non-Goals

**Goals:**

- Produce one coherent follow-on guide with an electrical block diagram, power budget, wiring table, capacitance guidance, measurement forms, phase gates, and shutdown instructions.
- Establish attributable voltage and current evidence before combining loads.
- Validate the Bluetooth ESP32 and both DRV8833 channels from a protected 3S source before adding camera power.
- Make every transition between voltage and current measurement explicit and safe.
- Preserve a simple rollback point: disconnect the camera branch to return to the phase-1 configuration, or disconnect battery power to return to the original Lesson 02 bench setup.

**Non-Goals:**

- Independent left/right speed control, differential steering, or a drivable chassis.
- Intentional locked-rotor measurement or certification of the HW-627 at its advertised peak current.
- Designing a cell charger, BMS, or custom battery pack from individual loose cells.
- Qualifying arbitrary LM2596 modules by their printed current rating alone.
- Adding the existing lesson's servo to the battery architecture.

## Decisions

### Keep the rover power work as a follow-on exploration

The main guide will be developed from `syllabus-and-teacher-materials/Explorations/PowerArchitecture.md`, with an explicit prerequisite and link from the relevant ESP32 material. Lesson 02 remains independently buildable and retains its USB/bench-supply missions.

Alternative: convert Lesson 02 itself to battery power. Rejected because it would erase the present low-risk learning boundary and change a stable capability whose existing verification is based on split supplies, one motor, and one servo.

### Use 3S and three non-paralleled LM2596 branches

The selected topology is:

```text
protected 3S pack
  -> pack fuse -> physical cutoff -> star distribution
       |-> LM2596 A, 5.0 V --------> ESP32-WROOM 5V input
       |-> LM2596 B, 4.0-4.5 V ----> HW-627 VCC -> two motors
       `-> LM2596 C, 5.0 V --------> ESP32-S3-CAM 5V input (phase 2)

common protected-pack negative -> star ground -> all converter inputs
converter-output negatives and control ground -> defined common reference
```

Three separate converters provide logic/motor noise isolation and allow the camera branch to remain physically absent during phase 1. Their positive outputs are never tied together. A preassembled or otherwise appropriately protected 3S pack with balancing protection is used rather than defining a DIY cell-assembly procedure.

Alternative: one 5 V converter shared by both ESP32 boards. Electrically feasible, but rejected for this baseline because the third available converter gives camera fault isolation and clearer branch-current evidence.

Alternative: 2S. Rejected for the LM2596 baseline because a discharged or sagging 2S source provides inadequate regulation margin for robust 5 V ESP32 rails.

### Start the motor rail at 4.0 V and permit advancement to 4.5 V

The guide will start at 4.0 V. It may permit adjustment upward, never beyond 4.5 V in this change, only if secured unloaded motors do not start reliably and all voltage, current, and temperature observations remain acceptable. Voltage is changed only with power removed from the loads and is reverified before reconnection.

Based on the provisional 6 V stall estimate, voltage-proportional estimates are approximately 0.8-to-1.0 A per motor at 4.0 V and 0.9-to-1.125 A at 4.5 V. These figures size the initial measurement range but are not acceptance evidence. Actual startup and running current are recorded without holding a shaft stalled.

Alternative: 6 V. Rejected because two simultaneous 1.2-to-1.5 A demands leave no useful current or thermal margin on an unqualified HW-627.

Alternative: limit a 6 V rail using PWM duty alone. Rejected because the motor and bridge still see 6 V during each on-pulse; reducing the physical rail better constrains instantaneous current.

### Reuse one signed motor command for both H-bridges during power validation

Phase 1 is an electrical-integration activity, not a drive-control redesign. `IN3` and `IN4` will mirror the already safe `IN1` and `IN2` command signals so both bridges receive the same coast, direction, PWM, reversal delay, disarm, and failsafe behavior. This can be implemented as documented signal fan-out after checking that the actual HW-627 inputs match the inspected specimen. It does not parallel bridge outputs.

Channel-isolated tests are performed with power removed while only the motor under test is connected to its bridge output. The sequence is motor A only, motor B only, and then both motors. Motor lead orientation is documented rather than assumed; the bench test does not define rover-forward direction.

Alternative: allocate new GPIOs and implement independent motor commands. Deferred because that would introduce differential-control behavior unrelated to the power question and expand firmware and verification scope.

### Use staged current measurements with explicit measurement boundaries

The guide will distinguish:

- load current measured in series at an LM2596 output;
- converter input current measured in series at a branch input, if used;
- whole-system current measured in series after the protected pack and fuse.

Phase 1 uses this sequence:

| Stage | Loads energized | Required evidence |
| --- | --- | --- |
| Source check | No loads | Protected-pack voltage and polarity |
| Converter setup | One unloaded converter at a time | Output voltage and polarity |
| Bluetooth-only | LM2596 A and ESP32 | 5 V at converter and board, branch-output current while Bluetooth is active |
| Motor A | LM2596 B, driver, motor A | Driver idle plus motor-A startup/running current and motor rail at load |
| Motor B | LM2596 B, driver, motor B | Motor-B startup/running current and motor rail at load |
| Both motors | LM2596 B, driver, both motors | Simultaneous startup/running current, rail droop, fault and heating observations |
| Integrated phase 1 | LM2596 A and B | Whole-pack current and voltage plus both load-terminal voltages for idle, connected, individual-motor, and both-motor states |

Phase 2 first repeats converter setup and camera-only measurement for LM2596 C, then performs a bounded all-branch test. A conventional handheld multimeter may miss a short startup peak, so the guide will describe recorded maxima as observed values rather than exact transient characterization; min/max capture may be used when the meter supports it.

The meter is inserted or removed only with the cutoff open. Current begins on a fused range above the expected transient. The procedure includes a conspicuous checkpoint to move the lead out of the current jack before resuming voltage measurements.

### Use measurement-driven phase gates

Phase 1 passes only when:

- the protected pack and both enabled converter outputs have correct polarity and in-range voltage;
- the 5 V board input and 4.0-to-4.5 V driver input remain in range at the load terminals;
- separate and integrated current records are complete and attributable;
- each motor and then both motors start and run under the bounded bench commands;
- no unexplained reset, Bluetooth interruption, driver fault, persistent stall, severe droop, odor, damaged insulation, or excessive heating occurs.

The phase gate intentionally avoids universal numeric current and temperature pass limits until the exact cells, BMS, fuse, LM2596 modules, motor specimens, and meter are recorded. Their component ratings provide hard ceilings; measured behavior must also leave documented margin.

### Use a conservative planning budget and replace estimates with evidence

The initial planning budget is:

| Load | Rail | Planning allowance | Output power |
| --- | ---: | ---: | ---: |
| Bluetooth ESP32-WROOM | 5.0 V | 0.5 A | 2.5 W |
| Motor A at upper phase-1 rail | 4.5 V | 1.125 A estimated stall/startup ceiling | 5.1 W |
| Motor B at upper phase-1 rail | 4.5 V | 1.125 A estimated stall/startup ceiling | 5.1 W |
| Phase-1 total | mixed | simultaneous planning case | 12.7 W |
| ESP32-S3-CAM phase 2 | 5.0 V | 1.0 A | 5.0 W |
| Phase-2 total | mixed | simultaneous planning case | 17.7 W |

At an intentionally conservative 80% aggregate conversion efficiency, these correspond to approximately 15.9 W and 22.1 W from the pack. That is approximately 1.4 A and 2.0 A at 11.1 V nominal, or 1.8 A and 2.5 A at 9 V. These are selection estimates, not measured surplus. The final guide will include columns for actual idle, running, and observed-startup values and will recalculate margin from them.

### Retain and extend the existing suppression model

Each motor receives a 100 nF non-polarized ceramic directly across its terminals. The driver receives a 100 nF local ceramic and begins with approximately 470 uF of suitably rated local bulk capacitance because two motors now share its rail. The Bluetooth board begins with 100-to-220 uF local bulk; the camera board begins with 220-to-470 uF; and the protected distribution point may use 470-to-1000 uF with voltage rating suitable for the full 3S charge voltage. Existing module capacitance is inventoried rather than assumed sufficient.

Values remain measurement-driven. Capacitance is not used to conceal an undersized converter, driver, wire, connector, or battery pack. A single flyback diode remains prohibited across either reversible motor.

## Risks / Trade-offs

- [The HW-627 package and thermal pad cannot be confirmed from the photographs] → Start below the estimated per-bridge limit, test channels separately before together, record heating, and stop rather than rely on thermal shutdown.
- [The two motors may have different current and starting behavior] → Record each motor separately and preserve specimen identity in the results.
- [A handheld meter may miss motor-start transients] → Label values as observed, use min/max capture where available, and make voltage droop, resets, driver faults, and temperature part of the evidence.
- [Changing a meter to current mode can short a source] → Require power-off series insertion, a fused range, a connection diagram, and a mandatory return to the voltage jack/range.
- [LM2596 modules marketed as 3 A may overheat below that load] → Qualify the actual modules at their assigned loads and document temperature and droop instead of trusting the listing.
- [Motor noise may reset either ESP32] → Preserve separate positive rails, star returns, local bulk and ceramic bypassing, motor-terminal suppression, and short twisted motor leads.
- [External 5 V can conflict with USB power] → Require one source at a time unless exact-board isolation is established; upload and external-power transitions are documented explicitly.
- [The design has no independent left/right control] → State that mirrored drive exists only to validate electrical loading; a later change must define differential control before mobile operation.
- [Series-cell safety depends on the chosen pack] → Require a documented protected pack, matching charger, BMS limits, fuse, wire, connectors, and cutoff before the activity can proceed.

## Migration Plan

1. Preserve Lesson 02 and its existing verification unchanged.
2. Expand the exploration into the complete architecture and teacher-gated measurement procedure.
3. Add links that identify Lesson 02 as the prerequisite and the rover-power activity as later, separate work.
4. Perform and record phase 1 in reversible increments: source, converters, Bluetooth branch, each motor, both motors, then integrated battery power.
5. Add phase 2 only after the recorded gate passes; disconnecting LM2596 C returns the system to the validated phase-1 topology.
6. If any battery-powered stage cannot be validated, remove the pack and converters and return to the original USB plus current-limited bench-supply arrangement.

## Open Questions

- What are the exact protected-pack, BMS, fuse, switch, connector, and wire ratings? The implementation record must resolve these before powered work.
- What current and voltage droop do the actual two TT motor specimens produce at 4.0 V, and is an increase toward 4.5 V necessary?
- What continuous current and temperature rise can each actual LM2596 module sustain in its assigned location?
- What 5 V current does the exact ESP32-S3/OV3660 board draw during the selected representative streaming workload?
