# Bench wiring and electrical safety

This guide applies to the exact HW-627 specimen photographed in
[`photos/hw-627-front.jpeg`](photos/hw-627-front.jpeg) and
[`photos/hw-627-back.jpeg`](photos/hw-627-back.jpeg). Read
[`HW-627-INSPECTION.md`](HW-627-INSPECTION.md) before wiring a different module;
boards sold under the same name may differ.

> **Bench-only scope:** This lesson uses ESP32 USB power plus a regulated,
> current-limited actuator supply. It is not a battery-powered rover wiring
> guide. Do not add wheels, a chassis, steering linkage, or an actuator load.

## Inspected HW-627 wiring table

The selected bridge is the `IN1`/`IN2` and `OUT1`/`OUT2` half of the module.
The Elegoo-style 48:1 geared motor must be bare, unloaded, and firmly secured
before either output wire is connected. Do not fit a wheel, propeller, linkage,
or other load for this lesson.

| Physical label | Observed or inferred function | Bench connection | Inspection status |
| --- | --- | --- | --- |
| `VCC` | DRV8833 motor supply | Regulated actuator-supply positive branch | Label observed; never connect to ESP32 5 V or 3.3 V |
| `GND` | Driver ground and logic reference | Actuator-supply negative and common signal-ground point | Label observed |
| `IN1` | Selected bridge input 1 | ESP32 GPIO 26 | Label observed |
| `IN2` | Selected bridge input 2 | ESP32 GPIO 27 | Label observed |
| `OUT1` | Selected bridge output 1 | One secured bare-motor terminal | Label observed |
| `OUT2` | Selected bridge output 2 | Other secured bare-motor terminal | Label observed |
| `EEP` | Truncated sleep label; DRV8833 active-low `nSLEEP` | ESP32 GPIO 13 | Exposed pad observed; firmware holds it low until safe initialization finishes |
| `ULT` | Truncated fault label; DRV8833 active-low `nFAULT` | Leave unwired in this lesson | Exposed pad observed; optional telemetry is not configured |
| `IN3`, `IN4` | Unused second-bridge inputs | Leave unconnected | Labels observed |
| `OUT3`, `OUT4` | Unused second-bridge outputs | Leave unconnected | Labels observed |
| `J2` | Two-pad configuration jumper | Leave open exactly as photographed | Present and unbridged; function is not assumed |

The front photograph shows a DRV8833, an indicator LED, resistors marked `472`
and `473`, and populated capacitor footprints marked `2.2uF`, `0.01uF`, and
`10uF`. Their presence is recorded, but their voltage ratings cannot be read
from the photograph. No separate populated 100 nF `VCC`-to-`GND` bypass part is
identified with confidence, and the visible 10 uF part is not equivalent to the
100–220 uF starting bulk recommendation below.

Texas Instruments documents `nSLEEP` as an active-low input and `nFAULT` as an
active-low output in the
[DRV8833 data sheet](https://www.ti.com/lit/ds/symlink/drv8833.pdf).

## Power topology

Use this arrangement only after confirming that 5 V is permitted by the actual
motor and servo data or markings. The actuator source must be regulated,
current-limited for first motion, and capable of supplying motor startup plus
servo transient current without a large voltage drop. A nominal running-current
estimate alone is not a supply rating.

```text
USB host ───────────────> ESP32 USB connector

regulated 5 V actuator supply
  positive distribution point
    ├── separate short branch ──> HW-627 VCC
    └── separate short branch ──> servo positive
  negative/common distribution point
    ├── short return ────────────> HW-627 GND
    ├── short return ────────────> servo ground
    └── signal-reference lead ───> ESP32 GND

ESP32 GPIO 25 ──────────> servo signal
ESP32 GPIO 26/27/13 ────> HW-627 IN1/IN2/EEP
```

The two actuator-positive branches may come from the same adequate bench supply,
but they branch at the distribution point rather than passing servo current
through the HW-627 or motor current through the servo wiring. Use a similarly
low-impedance ground arrangement. In particular, do not route the motor return
through an ESP32 ground pin, a thin signal jumper, or a questionable breadboard
rail before it reaches the supply negative terminal.

Connect the ESP32 ground only as the logic-signal reference. **Never connect the
actuator positive rail to ESP32 5 V, VIN, 3.3 V, or any GPIO.** Do not connect
the regulated supply positive to the USB-derived positive rail. This lesson uses
USB for the ESP32 and the external regulated source for both actuators.

## Initial suppression, bypass, and bulk capacitance

Install the starting parts below before powered motor testing. Keep leads short;
a capacitor at the far end of a long jumper is not equivalent to one at the
noise or transient source.

| Purpose | Starting part and placement | Polarity and rating |
| --- | --- | --- |
| Motor brush-noise suppression | 100 nF ceramic directly across the two motor terminals | Non-polarized; suitable for the full reversed terminal voltage |
| Driver high-frequency bypass | 100 nF ceramic directly between HW-627 `VCC` and `GND` | Non-polarized; place beside the module supply pins |
| Driver local bulk storage | 100–220 uF electrolytic between HW-627 `VCC` and `GND` | Positive to `VCC`, negative stripe to `GND`; voltage rating comfortably above the 5 V rail |

The motor-terminal capacitor is deliberately non-polarized because the motor
terminal polarity reverses. Keep the two motor wires short and twist them
together where practical. The driver bypass and motor-terminal suppression do
different jobs and neither replaces the local electrolytic bulk capacitor.

Before power is connected, verify every electrolytic capacitor's polarity and
printed voltage rating. Replace any part whose rating cannot be established;
never infer a safe voltage rating from physical size or color. Inspect for
shorts and ensure capacitor leads cannot touch adjacent module pads.

Treat these values as a measured starting configuration, not a recipe for
masking an inadequate supply or poor wiring. During later simultaneous-motion
tests, observe the actuator rail at the distribution point, driver, and servo.
If measured droop or symptoms justify adjustment, reasonable trials include:

- increasing driver-local bulk capacitance to about 470 uF;
- adding 100–470 uF near the servo branch;
- adding 470–1000 uF at the actuator distribution point.

Use suitably voltage-rated parts and recheck inrush, supply current limiting,
temperature, and polarity after every change. More capacitance cannot compensate
for an undersized supply, high-resistance wiring, a stalled actuator, or a bad
ground return.

## Inductive current and motor-current limits

**Do not install one conventional flywheel diode directly across `OUT1` and
`OUT2` or across the motor.** That technique applies to a one-polarity switch.
This lesson reverses the voltage across the motor; a diode oriented to block one
direction becomes forward-biased in the other direction and can effectively
short the commanded bridge output.

The DRV8833 is a full MOSFET H-bridge. Its switching devices and intrinsic
recirculation paths accommodate winding current in both motor polarities, and
its input truth table selects drive, coast/fast-decay, or brake/slow-decay
behavior. The lesson deliberately commands `IN1=LOW, IN2=LOW` for zero, which
the data sheet identifies as coast/fast decay. External brush-noise suppression
is still useful, but that is the non-polarized 100 nF motor-terminal capacitor,
not a single diode. See the
[DRV8833 bridge-control and decay-mode documentation](https://www.ti.com/lit/ds/symlink/drv8833.pdf).

The approximately 300 mA figure for the Elegoo-style geared motor is an
unverified estimate of running current. It says nothing reliable about initial
startup current, momentary load transients, or locked-rotor current. Those can
be several times the free-running value. Do not select the supply, wiring, or
module solely from the DRV8833's advertised peak-current number: module copper,
connector quality, ambient temperature, cooling, current regulation, and the
actual motor all affect safe operation.

Use a current-limited bench supply and begin with conservative PWM commands.
Observe current and supply voltage during normal startup without restraining the
shaft. **Never perform a deliberate stall test in this lesson.** If the motor
cannot turn, immediately command disarm and remove actuator power rather than
holding it energized to obtain a measurement.

## Mandatory bench safety and shutdown

Software disarm is a control safeguard, not electrical isolation. Before
changing any wire, capacitor, module connection, or mechanical setup:

1. Press CIRCLE to disarm if the running system can still receive commands.
2. Turn off and disconnect the external actuator supply, then confirm that its
   output has fallen to zero.
3. Disconnect USB power from the ESP32.
4. Allow bulk capacitors to discharge and verify with a meter when necessary.
5. Only then touch or change the circuit.

Complete this checklist before applying actuator power:

- Clamp the geared motor by its gearbox or body. Leave its shaft bare and
  unloaded: no wheel, propeller, linkage, or other driven part. Insulate the
  motor leads and terminal capacitor so they cannot short.
- Secure the servo body. Remove its horn if practical; otherwise leave the horn
  unloaded with no steering linkage and provide clearance for its full sweep.
- Keep hands, hair, tools, and loose leads outside both motion areas. Keep the
  actuator-supply cutoff within immediate reach.
- Recheck supply polarity, electrolytic polarity and rating, the common ground,
  the separation of positive rails, the photographed pin labels, and the open
  `J2` jumper.
- With actuator power still off, confirm that the controller is neutral and the
  firmware reports a safe disarmed state.
- Start the external supply with conservative current limiting. There is no
  universal current-limit value for this setup because motor startup current,
  servo transients, and the actual devices must be measured. Increase the limit
  only as justified by normal, unloaded operation; never defeat it merely to
  force a stalled actuator to move.

Stop immediately for unexpected motion, a motor or servo that cannot move,
persistent chatter, excessive heat, smoke, unusual odor, severe supply-voltage
droop, repeated ESP32 resets, or a driver fault or overtemperature indication.
Press CIRCLE if that can be done without delaying the response, then remove
external actuator power immediately. Remove USB power next, inspect the setup,
and do not reconnect it until the cause is understood and corrected.

For normal shutdown, disarm first, switch off and disconnect the external
actuator supply, verify that motion has stopped and the actuator rail has
decayed, and only then disconnect ESP32 USB power. Perform teardown or wiring
changes only after both sources are off.

## Scope boundary: not yet a rover

This lesson stops at controlled motion of one secured, unloaded motor and one
secured, unloaded servo on the bench. It intentionally does not specify or
authorize:

- batteries, charging, battery protection, fusing, or a main power switch;
- DC/DC conversion or a shared portable power architecture;
- wheels, chassis mounting, a steering horn or linkage, or loaded actuators;
- traction, vehicle stopping behavior, a complete wiring harness, or full rover
  integration.

Do not extrapolate this split bench-supply diagram into a mobile vehicle. Later
curriculum work will cover battery safety, power budgeting, protection and
conversion, mechanical integration, loaded testing, and complete-rover wiring.
