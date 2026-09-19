This fits naturally as lessons 07–09, assuming the Elegoo sensor is the four-pin HC-SR04.

```text
07  Blocking distance measurement
        ↓
08  Interrupt-driven measurement added to lesson 06
        ↓
09  Distance modifies the joystick’s motor command
```

## Proposed lesson set

| Lesson | Main concept | Suggested project name |
|---|---|---|
| 07 | Measure an Echo pulse with `pulseIn()`, calculate distance, then block for 500 ms | `07-ultrasonic-blocking-distance` |
| 08 | Capture Echo edges with the D2 external interrupt while lesson 06’s controls continue | `08-interrupt-ultrasonic-joystick-control` |
| 09 | Separate requested motor power from distance-limited applied power | `09-distance-aware-motor-control` |

Suggested shared wiring:

| HC-SR04 pin | Uno |
|---|---|
| VCC | Arduino 5 V |
| GND | Shared ground |
| Trigger | D4 |
| Echo | D2 / INT0 |

The HC-SR04 expects a trigger pulse of at least 10 µs, measures roughly 2–400 cm, and recommends more than 60 ms between measurements. A 500 ms cadence is comfortably within that guidance. [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)

## Lesson 07: blocking measurement

The core loop can deliberately expose both kinds of blocking:

```text
send 10 µs Trigger pulse
        ↓
pulseIn() waits for Echo
        ↓
calculate and print
        ↓
delay(500)
        ↓
repeat
```

A bounded `pulseIn()` timeout is important. Without one, the Arduino default can wait much longer when no Echo arrives. Something around 25–30 ms covers the sensor’s advertised range without allowing a missing echo to freeze the lesson for a full second.

I interpret “report the delay in milliseconds” as reporting the Echo pulse’s round-trip duration. It should be a fractional value:

```text
Echo delay: 0.580 ms | Distance: 10.0 cm
```

Integer milliseconds would be misleading: the Echo time for 10 cm is only about 0.58 ms, so truncation would report `0 ms`. Internally, retain microseconds and convert only for display:

```text
distance_cm = echo_duration_us / 58.0
delay_ms    = echo_duration_us / 1000.0
```

“No echo” should be printed explicitly rather than converted to `0 cm`; those are different conditions.

## Lesson 08: interrupt measurement on lesson 06

The existing lesson 6 architecture already demonstrates independent timing:

```text
Timer1 overflow ISR ── every 20 ms ── joystick → motor + servo
Foreground loop     ── every 1 s  ── telemetry
```

That is visible in [main.cpp](/home/dbodden/code/fresh_checkout/robotics_curriculum_01/platformio-uno-lessons/06-joystick-motor-servo-control/src/main.cpp:57).

The new measurement would add:

```text
D4 Trigger pulse
      │
      ▼
HC-SR04 Echo on D2
      │
      ├── rising interrupt  → save start timestamp
      └── falling interrupt → save pulse duration
```

The D2 ISR should only timestamp edges and publish a completed duration. Distance calculation and Serial output remain outside the ISR. Shared 32-bit timestamps and status flags need an atomic snapshot, following lesson 6’s existing shared-data pattern.

One hardware interaction deserves explicit design treatment: lesson 6’s Timer1 ISR performs four ADC conversions and can occupy roughly several hundred microseconds. AVR interrupts do not normally interrupt another ISR, so sufficiently short Echo pulses could be timestamped late—or both edges could occur while Timer1 is busy.

A good integration strategy would be:

- Set an `echoMeasurementInProgress` flag immediately before Trigger.
- While that flag is set, let the Timer1 ISR skip its long ADC/control work.
- Let the D2 falling-edge ISR clear the flag.
- Add a roughly 30 ms timeout that clears a stuck measurement.
- Resume normal 20 ms joystick updates afterward.

That temporarily skips at most a couple of joystick updates while giving D2 reliable access to the short Echo pulse. The implementation mechanics belong in a helper or teacher explanation; the student-facing concept remains “an interrupt notices the pulse edges while the main program is occupied.”

## Lesson 09: distance-limited motor command

I recommend teaching two separate values:

```text
joystick → requested PWM → distance rule → applied PWM → D3
```

For example:

```text
Requested PWM: 180 | Distance: 7.4 cm | Slowdown: 50% | Applied PWM: 90
```

The simplest observable rule is:

```text
distance >= 10 cm       applied PWM = requested PWM
valid distance < 10 cm applied PWM = requested PWM / 2
```

A 50% factor is easy to understand, easy to test, and audibly visible on the secured bare motor. It also preserves joystick control instead of replacing it with a binary stop command.

Useful pure-function tests would cover:

- zero requested PWM remains zero;
- 255 becomes approximately 127 below 10 cm;
- exactly 10 cm is not slowed;
- servo mapping remains completely unchanged;
- invalid/no-echo data has an explicitly chosen policy;
- requested and applied commands never exceed 0–255.

The biggest unresolved choice is the invalid-reading policy:

- **Full command:** Treat no Echo as “nothing nearby.” Simple, but not fail-safe.
- **Slow or stop:** Treat unknown distance conservatively. Safer conceptually, but students may see the motor slow because of a missed reading rather than an obstacle.

Because an HC-SR04 is not a safety-rated obstacle detector—and the 500 ms measurement cadence alone permits substantial response latency—I would keep the motor bare and secured, as lesson 6 already requires. The activity should demonstrate sensor-informed control, not claim collision avoidance.

## Curriculum implications

The new set should extend the canonical route currently ending at lesson 06 in [the collection index](/home/dbodden/code/fresh_checkout/robotics_curriculum_01/platformio-uno-lessons/README.md) and the beginner progression spec. Each lesson can remain independently buildable and keep the existing structure:

- short Core Mission;
- power-off wiring and teacher checkpoint;
- one observable result;
- prediction/comparison;
- deeper interrupt and timing details separated from the student focus.

No files or OpenSpec artifacts were changed during this exploration. This is developed enough to turn into a change proposal once we settle the slowdown factor and invalid-reading behavior.


I don't want to skip joystick reads. Instead, let's be practical - the maximum distance for the sensor is 4000 millimeters so unless I'm mistaken we're talking about timings of 23.2 milliseconds max to bounce off something 4 meters away and come back. These readings are only concerns at the nearer range - 10cm obstacle distance and 580 microseconds round trip time from pulse to echo.

As such, I think we're safe to ignore any echoes that take longer than the interval between ISRs (just reset or ignore the ultrasonic interrupt timer at the beginning of the ISR), and we can always run the initiation of the pulse at the END of the ADC->PWM read/command cycle.

So:

ADC ISR starts -> read ADC -> issue PWM -> trigger ulstrasonic pulse.

Ultrasonic ISR starts -> calculate distance -> modify & issue PWM if less than 10 cm

My decision is to adopt "full control" when there's an invalid/no-echo reading from the ultrasonic sensor. The test rig is totally safe and immobile, this is just for demo purposes.

Your timing argument works, with one important state-management adjustment.

Triggering at the very end of the existing Timer1 ISR gives the Echo interrupt almost the entire next 20 ms frame. Since the only decision boundary is 10 cm—about 580 µs—that is ample time. Echoes still pending at the next Timer1 overflow can be declared invalid and ignored. Because invalid means full control, that behavior is deterministic and appropriate for this stationary demonstration.

One correction: the ultrasonic ISR’s slowdown decision must persist. Otherwise, the next 20 ms ADC/PWM cycle will overwrite the reduced PWM with the joystick’s full command.

```text
Every 20 ms — Timer1 overflow ISR
──────────────────────────────────
1. Expire any Echo measurement still pending
   → mark invalid
   → full-control policy

2. Read joystick ADC values

3. Calculate requested motor PWM

4. Apply current distance policy:
   near obstacle → requested PWM × slowdown factor
   otherwise     → requested PWM

5. Issue applied PWM and servo command

6. Every 25th frame:
   send the 10 µs Trigger pulse as the final operation
   enter WAITING_FOR_RISE
```

```text
D2 CHANGE ISR
─────────────
Rising Echo:
    save micros()
    enter WAITING_FOR_FALL

Falling Echo:
    duration = micros() - start
    mark measurement valid
    slowdownActive = duration < 580 µs
    immediately apply the resulting PWM
    return acquisition state to IDLE
```

There should be two separate concepts in the state:

- **Acquisition state:** `IDLE`, `WAITING_FOR_RISE`, or `WAITING_FOR_FALL`.
- **Control policy:** whether the latest completed measurement requires slowdown.

A successful distance result updates the control policy, which remains in effect until the next completed measurement or timeout. A timeout clears slowdown and restores full joystick control.

A few refinements:

- Trigger only every 25 Timer1 frames: `25 × 20 ms = 500 ms`. Triggering every frame would violate the sensor’s recommended measurement spacing.
- Compare the duration directly with `580 µs` inside the ISR. Floating-point distance calculation can wait for the foreground reporting code.
- Ignore D2 edges unless the acquisition state expects them. This safely rejects a late falling edge after a timeout.
- Keep both `requestedMotorPwm` and `appliedMotorPwm`. That makes the lesson’s report much clearer:
  
  ```text
  Echo: 0.432 ms | Distance: 7.4 cm |
  Requested PWM: 180 | Applied PWM: 90 | Slowdown: yes
  ```

- On a new measurement, retain the previous distance policy while awaiting Echo. Only change it when Echo completes or the following 20 ms Timer1 ISR declares the measurement invalid.
- Since `micros()` is backed by Timer0, “resetting the ultrasonic timer” is best expressed as resetting the Echo acquisition state and start timestamp; no additional hardware timer is necessary.

One terminology detail for the lesson material: the existing routine is a **Timer1 overflow ISR that performs ADC reads**, rather than an ADC ISR.

Assuming a 50% slowdown factor, the resulting control behavior is:

| Latest result | Applied motor command |
|---|---|
| Valid, distance < 10 cm | `requestedPWM / 2` |
| Valid, distance ≥ 10 cm | `requestedPWM` |
| Invalid or no Echo | `requestedPWM` |
| Measurement currently pending | Retain the preceding valid policy |

That gives you immediate slowdown within roughly 580 µs at the threshold, continued joystick and servo updates every 20 ms, and full-control fallback after at most one control interval.

What's the minimum recommended sensor measurement spacing?

The HC-SR04 datasheet recommends **more than 60 ms between trigger pulses** to prevent one measurement interfering with the next. [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)

With the lesson’s 20 ms Timer1 frames:

- Every 3 frames = exactly 60 ms
- Every 4 frames = 80 ms, safely above the recommendation
- Every 25 frames = 500 ms, matching your reporting cadence and comfortably safe

So **80 ms is the shortest convenient interval** for this architecture, while 500 ms remains appropriate for the planned lessons.

[$openspec-propose](/home/dbodden/code/fresh_checkout/robotics_curriculum_01/.agents/skills/openspec-propose/SKILL.md) let's use the 80s spacing and produce an openspec proposal from this exploration