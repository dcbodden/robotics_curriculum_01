# Four gated bench missions

These missions turn a PS4-compatible controller into commands for one secured,
unloaded motor and one secured, unloaded servo. Complete them in order. A
teacher or qualified bench supervisor owns every gate that enables external
actuator power.

Read [`BENCH_WIRING.md`](BENCH_WIRING.md) first. The circuit remains fixed
through all four missions. “Servo only” and “motor only” describe which stick
axis the student moves; they do not mean rewiring the power system.

`CONTROL` records describe firmware commands, not measured motion or voltage.
In particular, `"actuator_ready":true` means that the ESP32 output adapter
initialized; it does not prove that external power is connected or that an
actuator moved. Record commands, electrical measurements, and physical
observations separately.

## Mission 1: prove control while actuator power is off

**Goal:** Prove the complete input and safety sequence before either actuator
can move.

1. Turn off and disconnect the external actuator supply. Keep the motor and
   servo positive branches unpowered. Connect only ESP32 USB power after the
   teacher has checked the fixed signal wiring and common ground against
   [`BENCH_WIRING.md`](BENCH_WIRING.md).
2. From this lesson directory, run the build, upload, and 115200-baud monitor
   commands in [`README.md`](README.md). Confirm this is the
   `esp32doit-devkit-v1` environment and require `firmware_started`,
   `bluetooth_ready`, and `accepting_connections` evidence.
3. Complete either the documented first-pair test or bonded-reconnection test.
   Require both `controller_connected` and a later valid input record; a
   controller light alone is not evidence.
4. Release the left stick and both safety buttons. Find a `CONTROL` record with
   `state:"disarmed"`, both axes between -40 and 40, `target_motor:0`,
   `applied_motor:0`, `bridge_direction:"coast"`, `bridge_duty:0`, and
   `servo_pulse_us:1500`. The transition reason `neutral_observed` establishes
   readiness.
5. Move either left-stick axis outside -40 through 40. Press and release
   OPTIONS while it remains displaced. Confirm the state stays `disarmed`, the
   bridge remains at coast with duty 0, and the servo command remains 1500.
   Return the stick to neutral and release OPTIONS.
6. After a fresh neutral record, press and release OPTIONS again. Require a
   `control_transition` with `state:"armed"` and
   `transition_reason:"armed"`. Keep the stick neutral: the motor fields must
   remain zero and the servo command must remain 1500.
7. Press CIRCLE. Require an immediate transition to `state:"disarmed"` with
   `transition_reason:"disarmed"`, zero motor command, bridge coast/duty 0,
   and a 1500-microsecond servo command. Stick movement must not create a motor
   command while disarmed.
8. Re-arm from a fresh neutral report. Under teacher direction, interrupt valid
   reports without touching the bench circuit. A distinct watchdog pass
   requires `state:"failsafe"` and `transition_reason:"watchdog_expired"`
   within the configured 300 ms interval, followed by zero/coast/1500 safety
   output. If the controller reports a disconnect first, record that as a
   disconnect pass, not a watchdog pass; keep the physical watchdog check
   marked pending and retain the host-policy check as separate evidence.
9. Power off or disconnect the controller and require
   `transition_reason:"controller_disconnected"` with zero/coast/1500 output.
   Reconnect it. Confirm that displaced input cannot resume output and that a
   new neutral report plus a new OPTIONS edge is required to arm again. Press
   CIRCLE and leave the system disarmed.

Do not use external actuator power at any point in Mission 1.

### Mission 1 evidence

| Check | Exact observed evidence or `pending` | Pass/fail |
| --- | --- | --- |
| Build and upload used the ESP32 lesson/environment | | |
| Fresh pair or reconnection plus valid input | | |
| Neutral disarmed record: axes, motor, bridge, servo | | |
| Displaced-stick OPTIONS press rejected | | |
| Neutral OPTIONS edge armed | | |
| CIRCLE disarmed with safe output | | |
| Watchdog expiry with elapsed time and safe output | | |
| Disconnect and reconnect required neutral plus re-arm | | |

### Teacher gate to Mission 2

Keep external power off. The teacher must sign off all of the following before
the servo may be energized:

- the correct project built and uploaded, and usable Bluetooth input was shown;
- neutral, rejected arming, successful arming, and explicit disarming evidence
  matches the fields above;
- disconnect recovery passed, and watchdog evidence is either observed or
  explicitly marked pending rather than inferred from disconnect;
- the actual servo voltage range is compatible with the regulated supply;
- wiring, capacitor polarity/rating, separate positive branches, and common
  ground match the inspected HW-627 bench guide;
- the unloaded servo and bare motor are secured, both motion areas are clear,
  and the external supply cutoff is immediately reachable.

If any required safety or wiring check fails, stop here. To finish for the day,
close the monitor and disconnect USB. Change wiring only after both power
sources are confirmed off.

## Mission 2: move only the servo axis

**Goal:** Verify controlled servo direction and safe pulse limits while the
motor command remains zero.

1. Keep the approved circuit unchanged. The teacher verifies that the motor is
   bare and secured, the unloaded servo body is secured, the horn or output
   shaft has its full area clear, the supply is at the approved voltage with a
   conservative current limit, and the cutoff is within reach.
2. Connect USB, reconnect the controller, and prove a fresh neutral,
   `state:"disarmed"`, zero/coast motor record. The teacher then enables the
   external actuator supply. The servo may move to its 1500-microsecond center;
   the motor must remain electrically undriven. Stop for unexpected motion.
3. From neutral, press and release OPTIONS. Keep the left stick's vertical
   motor axis inside its dead zone throughout this mission.
4. Move the horizontal axis only a small distance to one side of center. Compare
   `axis_x`, `servo_pulse_us`, and the observed servo direction. Return to
   center and require 1500. Repeat with a small movement to the other side.
   With the default non-inverted mapping, negative X produces a pulse below
   1500 and positive X produces one above 1500; the physical left/right result
   depends on servo mounting. Record it rather than assuming it.
5. Increase travel gradually only with teacher approval. Every record must stay
   within the configured 1000–2000 microsecond bounds, and
   `target_motor:0`, `applied_motor:0`, `bridge_direction:"coast"`, and
   `bridge_duty:0` must remain true. The configured bounds are clamps, not a
   promise that every servo can safely reach them.
6. Return to center, press CIRCLE, and verify disarmed zero/coast/1500 output.

If the servo binds, hits a stop, moves unexpectedly, or buzzes persistently,
remove external actuator power immediately and then disconnect USB. Do not
force the horn or repeatedly test the same endpoint. With all power off, move
the affected `kServoMinimumPulseUs` upward or `kServoMaximumPulseUs` downward
toward `kServoCenterPulseUs` in `main/control_config.h`. Preserve
minimum < center < maximum. Rebuild and upload with actuator power off, repeat
Mission 1's neutral and arming checks, and restart Mission 2 near center. Set
`kServoDirectionInverted` only when the required physical steering sense has
been established; inversion does not cure binding.

### Mission 2 evidence

| Observation | Recorded value |
| --- | --- |
| Supply voltage and current-limit setting | |
| Center pulse and observed center behavior | |
| Negative-X pulse and physical direction | |
| Positive-X pulse and physical direction | |
| Lowest and highest pulses actually tested | |
| Motor command/duty throughout | |
| Binding, buzzing, or endpoint change (`none` if absent) | |

### Teacher gate to Mission 3

Proceed only if the servo moved freely in both tested directions, returned to
center, remained inside the recorded approved pulse range, and the motor fields
stayed zero/coast. The teacher rechecks the secured bare motor, insulated motor
terminals, current limiting, clear shaft area, and immediate access to the
supply cutoff. Leave the circuit fixed. If stopping, disarm, remove external
power first, then disconnect USB.

## Mission 3: test forward, coast, reverse, and reversal

**Goal:** Demonstrate bounded bidirectional motor control while the servo stays
centered.

1. Keep the approved circuit unchanged. The teacher confirms the supply voltage
   and current-limit setting, bare secured shaft, insulated terminals, clear
   area, cool driver and motor, and reachable power cutoff. Keep hands and
   objects away from the shaft; never grip or block it.
2. Establish a fresh neutral/disarmed record, enable external power under the
   teacher gate, and arm with OPTIONS. Keep the horizontal servo axis in its
   dead zone; require `servo_pulse_us:1500` throughout this mission.
3. Push the vertical axis slightly upward. Begin with a target and bridge duty
   no greater than about 64 of 255. Require a positive `target_motor`, an
   `applied_motor` that slews upward, `bridge_direction:"forward"`, and only
   one driven bridge side. Briefly observe the motor from a safe distance, then
   release the stick. “Forward” names the bridge command; the observed shaft
   rotation depends on which motor lead is on `OUT1`.
4. At release, require `target_motor:0`, then `applied_motor:0`,
   `bridge_direction:"coast"`, and `bridge_duty:0`. Record whether the bare
   shaft coasted after electrical drive reached zero.
5. Push the vertical axis slightly downward, again starting no greater than
   about 64 of 255. Require a negative target/applied command and
   `bridge_direction:"reverse"`. The observed shaft rotation must oppose the
   forward observation. Release and require coast/duty 0 again.
6. For the reversal check, establish a small, steady positive applied command,
   then move directly to a similarly small negative target. The policy first
   reports `applied_motor:0`, `bridge_direction:"coast"`, `bridge_duty:0`, and
   `reversal_interlock:true`; both bridge inputs remain low for at least one
   complete 20 ms control update before negative duty begins. Repeat at low
   duty if needed to capture evidence, then return to neutral and disarm.

Periodic `CONTROL` records are intentionally limited to one every 200 ms, so a
20 ms reversal interval may fall between records. Count it as observed only if
a record with `reversal_interlock:true` is captured or the teacher measures
both `IN1` and `IN2` low with a logic analyzer or oscilloscope. Otherwise mark
the physical reversal-interval observation `pending`; the deterministic host
policy check remains separate evidence. Sound, coasting, or a direction change
alone does not prove the zero interval.

If the motor does not begin turning at the initial low command, return to
neutral promptly. The teacher may approve a brief, progressively larger bounded
command after checking current, voltage, connections, and free shaft motion.
Never hold a non-turning motor energized, restrain the shaft, or perform a
locked-rotor test. Stop and remove external power for current limiting that does
not recover, excessive heating, driver fault, odor, smoke, unstable voltage, or
unexpected motion.

### Mission 3 evidence

| Observation | Recorded value |
| --- | --- |
| Supply voltage and current-limit setting | |
| Forward target/applied/duty and shaft direction | |
| Released zero/coast and observed mechanical coast | |
| Reverse target/applied/duty and shaft direction | |
| Maximum unloaded current observed | |
| Reversal zero interval: telemetry or instrument evidence | |
| Servo pulse throughout | |
| Heating, fault, or abnormal behavior (`none` if absent) | |

### Teacher gate to Mission 4

Proceed only after forward and reverse motion, zero/coast, normal current and
temperature, and a centered stable servo have passed. The reversal interval
must be recorded as observed or explicitly pending. Recheck both clear motion
areas and the supply cutoff. If stopping, disarm, remove external actuator
power first, wait for motion to stop, then disconnect USB.

## Mission 4: operate both commands and inspect power integrity

**Goal:** Combine moderate drive and steering commands while keeping command,
motion, Bluetooth, and electrical evidence distinct.

1. Use the unchanged circuit and the settings that passed Missions 2 and 3.
   Before arming, the teacher records actuator-supply voltage and current at
   idle and confirms both motion areas and the cutoff remain clear.
2. Arm from neutral. Move the stick diagonally to combine a previously safe
   servo pulse with a previously safe, conservative forward motor duty. Hold
   only long enough to observe stable behavior, then return to neutral. Repeat
   in the reverse motor direction if the first test is normal.
3. Record `axis_x`, `axis_y`, target and applied motor commands, bridge
   direction/duty, servo pulse, and what each actuator actually did. A correct
   command is not evidence that the actuator reached the requested speed or
   position.
4. Observe supply voltage and current during motor startup while the servo is
   moving and after both settle. Measure at the actuator distribution point and,
   where practical, at the HW-627 and servo branches. Record the lowest captured
   voltage and the instrument used; a handheld meter may miss short transients.
5. Watch the serial stream throughout. Bluetooth continuity requires fresh
   `CONTROL` records without `watchdog_expired` or
   `controller_disconnected`. An unexpected new `firmware_started` record is
   reset evidence, not a successful continuation. Also record chatter, driver
   fault, heat, unusual sound, or interrupted motion.
6. Return to neutral, require zero/coast and centered servo, then press CIRCLE.
   Turn off and disconnect external actuator power first, wait for motion and
   the rail to decay, and only then close the monitor and disconnect USB.

If severe droop, resets, Bluetooth loss, chatter, or unstable motion occurs,
remove external power immediately and then USB. Inspect supply capacity,
current-limit behavior, connector resistance, branch routing, and the common
ground before changing capacitance. If those are sound and measurements still
show a transient problem, use the optional, suitably rated bulk ranges in
[`BENCH_WIRING.md`](BENCH_WIRING.md) as controlled trials. Make one change at a
time with both sources off, record its location/value/polarity, and repeat the
earlier gates before another simultaneous test. “More capacitance” is not the
answer to an undersized supply, bad return path, or stalled actuator.

### Mission 4 evidence

| Evidence | Idle | Motor startup plus servo motion | Settled simultaneous motion |
| --- | --- | --- | --- |
| Supply voltage at distribution point | | | |
| Voltage at HW-627 branch | | | |
| Voltage at servo branch | | | |
| Supply current | | | |

| Result | Observation |
| --- | --- |
| Forward diagonal commands and actuator response | |
| Reverse diagonal commands and actuator response | |
| Lowest captured voltage and measurement instrument | |
| ESP32 reset evidence | |
| Bluetooth continuity or loss | |
| Chatter, fault, heat, or other abnormal behavior | |
| Existing bulk capacitance and locations | |
| Additional bulk justified by measurement? Why? | |
| Retest result after any approved change | |

### Final teacher signoff

The lesson is complete only when every performed check has observed evidence
and every unperformed check says `pending`. Do not convert a successful build,
host check, or command record into a claim of physical motion or power
integrity. This remains a secured, unloaded bench test; it does not authorize
batteries, wheels, a chassis, steering linkage, or rover wiring.

## Troubleshooting by observed symptom

For unexpected motion, a non-turning energized motor, binding or persistent
servo buzz, excessive heat, smoke, odor, severe voltage droop, or repeated
resets, remove external actuator power immediately and then USB. Do not inspect
or change wiring while either source is connected.

### Wrong project, environment, or serial port

1. Keep actuator power off. From the lesson directory, verify `pwd`,
   `default_envs = esp32doit-devkit-v1`, and `./scripts/pio.sh --version`.
2. A correct build says `Processing esp32doit-devkit-v1` and uploads an ESP32
   `.bin` with `esptool`. Stop if output mentions `uno`, `avrdude`, or `.hex`.
3. Close other monitors, run `./scripts/pio.sh device list`, select the observed
   ESP32 port explicitly if necessary, upload, then open only one monitor.
4. Reset the board and require `firmware_started` for
   `02-bluetooth-motor-servo-control`. A source tab being open does not select
   its PlatformIO project.

### Pairing or reconnection fails

1. Use the furthest `BTDIAG` event, not the controller light, to choose the next
   check. Require `bluetooth_ready` and `accepting_connections` before blaming
   the controller.
2. For first pair, start with the controller off and hold SHARE + HOME until its
   light flashes. For a stored-bond reconnection, press HOME only. Begin within
   the firmware's 75-second window; reset the ESP32 after `connection_timeout`.
3. Require `controller_connected` plus a later valid requested input. If it
   connects and drops, charge the controller, reduce distance, and retain the
   raw stack log.
4. Clear bonds only through the confirmation-gated procedure in `README.md` and
   only when explicitly authorized. Pairing failure alone is not permission to
   change stored bonds.

### Controller connects but will not arm

1. Keep actuator power off and inspect `CONTROL`. Both `axis_x` and `axis_y`
   must be between -40 and 40 in a fresh valid report.
2. Release OPTIONS and CIRCLE. Require `neutral_observed`, then create a new
   OPTIONS press edge on a later neutral report. Holding OPTIONS during the
   first neutral report cannot arm.
3. If state is `failsafe` or `disconnected`, reconnect, establish fresh neutral,
   and use a new OPTIONS edge. Arming never resumes an old session.
4. If the axes never settle in the dead zone, record their released values and
   investigate controller calibration or validity with actuator power off; do
   not widen the dead zone merely to conceal a bad report.

### Arms, then immediately enters failsafe

Read `transition_reason` first. `watchdog_expired` means no fresh valid report
arrived for 300 ms; `controller_disconnected` means the selected controller
left; `invalid_controller` means its report was not accepted as a valid
gamepad. Check charge, distance, Bluetooth logs, `freshness_age_ms`, and whether
another device is interfering. Keep actuator power off while reproducing the
input fault. Recovery always requires reconnection when applicable, a new
neutral report, and a new OPTIONS edge; increasing the watchdog without
evidence is not a first-line fix.

### Motor or steering direction is reversed

First compare the raw axis, command, and physical result. Stick-up should make
`target_motor` positive; negative X should make `servo_pulse_us` less than
1500 with the default settings. If telemetry is correct but physical motor
rotation is opposite the intended convention, either swap `OUT1`/`OUT2` with
all power removed or toggle `kMotorDirectionInverted`—not both. If servo motion
is opposite the intended mounting direction, toggle `kServoDirectionInverted`.
Rebuild and upload with actuator power off and repeat Missions 1 and 2 before
powered motor testing. Direction inversion does not cure wrong pin wiring or
mechanical binding.

### Motor does not move

1. Return to neutral promptly. If target, applied command, or bridge duty is
   zero, resolve arming, input, dead-zone, or policy state before inspecting
   hardware.
2. If a small nonzero duty does not start the unloaded motor, briefly try only a
   teacher-approved larger bounded command while observing current and voltage.
   Starting friction is possible; never leave a non-turning motor energized.
3. Remove both power sources. Check supply voltage/current limit, shared ground,
   `EEP` to GPIO 13, `IN1`/`IN2` to GPIO 26/27, motor on `OUT1`/`OUT2`, the open
   `J2`, terminal security, and motor continuity. Inspect for a short or seized
   gearbox without forcing the shaft.
4. Do not infer HW-627 suitability from the DRV8833 peak rating or the
   provisional 300 mA running estimate. Stop if normal unloaded startup requires
   unsafe current, voltage, or temperature.

### Servo is unstable, chatters, or binds

Remove external power, then USB. With power off, verify the actual servo pinout,
GPIO 25 signal, direct common-ground return, branch polarity, connector contact,
and an unloaded clear mechanism. Measure the branch voltage during motion and
check whether motor startup causes the symptom. For endpoint binding, narrow the
affected pulse bound toward 1500 before rebuilding. For supply disturbance,
correct capacity, current limiting, wiring resistance, and return routing
before trying the documented optional local bulk capacitance. Never force the
horn or use added capacitance to mask a stalled servo.

### Driver fault indication or excessive heat

Remove external power immediately, then USB, and allow parts to cool without
touching them. This lesson leaves the photographed `ULT`/`nFAULT` pad unwired,
so it does not claim digital fault telemetry. Inspect supply polarity and
voltage, output shorts, motor/terminal wiring, blocked motion, sustained
current, PWM command, and module damage. Resume only after the cause is found,
the motor turns freely, and the teacher approves a current-limited retest.

### Resets, Bluetooth loss, rail droop, or combined-motion problems

Correlate timestamps for supply voltage/current, `firmware_started`,
`controller_disconnected`, `watchdog_expired`, servo chatter, and motor startup.
Measure at the distribution point and both branches. With both sources off,
check short low-impedance returns, separate branches, connectors, bypass and
bulk placement, capacitor polarity/rating, and the motor-terminal ceramic.
Correct inadequate supply capacity or wiring first. Add or increase bulk only
when measurements justify it, one documented change at a time, and repeat all
earlier gates. A working idle measurement does not prove transient integrity.
