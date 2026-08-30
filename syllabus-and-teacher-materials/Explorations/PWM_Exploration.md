[$openspec-explore](/home/dbodden/code/fresh_checkout/robotics_curriculum_01/.agents/skills/openspec-explore/SKILL.md) I want to develop an extension of the arduino gpio transistor control and variable blink timing lessons that will interact with an external RFP30N06LE n channel power MOSFET to run PWM motor control. The MOSFET, a flywheel diode, a low-voltage power supply (e.g. a snap circuits B1 w/ 2 AA batteries), an RV variable resisitor snap, and a small external motor like those in the Elegoo arduino kits will be set up with a breadboard with the power components (motor, MOSFET, flywheel diode, and B1 battery snap) sharing ground with the arduino. There are 3 variants of this demonstration lesson:

1. programmatic PWM cycling from 0% to 100% and back down smoothly over an 8 second cycle
2. blocking ADC reads setting the PWM values dynamically adding a 0.5 second delay between reads to facilitate the serial console writes of the ADC values
3. The introduction of timer-based interrupts for the ADC reads every 100ms, with console writes every 0.5 seconds in a blocking loop.


This forms a coherent three-lesson extension, but it introduces a meaningful hardware transition: D8 must move to a PWM-capable pin, and the motor requires a complete protected MOSFET switching stage.

```text
Arduino D3 ── 220–330 Ω ── Gate
                              │
                            10 kΩ
                              │
Arduino GND ──────────────────┼──── B1 negative
                              │
                           Source
                         RFP30N06LE
                           Drain
                              │
                              ├──── Motor negative
                              │
                      diode anode
                      diode cathode ── B1 positive
                                      │
                                      └── Motor positive
```

B1 positive must not connect to Arduino 5 V or VIN. Only the grounds are joined. The diode’s cathode faces B1 positive; its anode faces the MOSFET drain. The motor should be physically secured with no propeller or wheel attached.

The RFP30N06LE is appropriate for 5 V gate control: its datasheet specifies a maximum 0.047 Ω on-resistance at `VGS = 5 V`. Its gate is capacitive, though, so I would add both the series gate resistor and a 10 kΩ gate-to-ground pulldown. [RFP30N06LE datasheet](https://www.mouser.com/datasheet/2/149/rfp30n06le-307378.pdf)

## Suggested progression

| Lesson | Main new idea | Motor update | Serial reporting |
|---|---|---:|---:|
| `03-pwm-motor-ramp` | PWM duty cycle | Smooth programmed ramp | Optional |
| `04-adc-pwm-motor-control` | ADC mapped to PWM | Every 500 ms | Every 500 ms |
| `05-interrupt-driven-motor-control` | Foreground versus interrupt work | Every 100 ms | Every 500 ms |

### 03: Programmatic PWM ramp

Use D3 and `analogWrite()` values from 0 through 255. An eight-second cycle naturally means:

```text
0% ── 4 seconds upward ──▶ 100%
100% ── 4 seconds downward ──▶ 0%
```

A useful teaching distinction: the PWM command changes smoothly, but motor speed probably will not. The motor may remain stopped at low duty cycles, suddenly begin moving after overcoming static friction, and coast during the downward ramp.

That behavior is educational rather than a defect: “electrical command” and “physical response” are related but not identical.

### 04: Blocking ADC-to-PWM control

Reuse A0 and map:

```text
ADC 0–1023  ──▶  PWM 0–255
```

The main loop would read A0, update D3, print both values, and wait 500 ms. I would describe this as a “blocking 500 ms sample/report loop.” Although `analogRead()` is synchronous, the noticeable loss of responsiveness comes primarily from `delay(500)`, not from the ADC conversion itself.

This lesson exposes the limitation students should notice: turning RV can take up to half a second to affect the motor.

### 05: Timer-interrupt control

D3 uses Timer2 for PWM, so Timer1 can generate the 100 ms interrupt without disrupting motor PWM. Timer0 should remain untouched because Arduino timing functions depend on it. The Uno has six PWM outputs, while its ATmega328P provides distinct timer and interrupt facilities. [Arduino Uno R3 documentation](https://docs.arduino.cc/hardware/uno-rev3/), [ATmega328P datasheet](https://ww1.microchip.com/downloads/en/devicedoc/atmel-7810-automotive-microcontrollers-atmega328p_datasheet.pdf)

The behavior would be:

```text
Timer1 interrupt every 100 ms
          │
          ├── read A0
          ├── map 0–1023 to 0–255
          └── update D3 PWM

Main loop
          ├── copy latest values safely
          ├── print them
          └── delay 500 ms
```

This intentionally demonstrates that timer interrupts continue while the foreground loop is waiting.

There is one tradeoff: calling `analogRead()` inside an interrupt handler takes roughly a fraction of a millisecond and is not the ideal pattern for a sophisticated real-time system. At only 10 samples per second, however, it is practical for this demonstration and makes the contrast unusually clear. Serial output must remain outside the interrupt handler.

Because an Uno is an 8-bit processor and the ADC reading is 16-bit, the foreground code must copy the shared `volatile` reading atomically. That introduces three important ideas together:

- `volatile` shared state;
- keeping interrupt handlers short;
- safely transferring data between interrupt and foreground code.

## Hardware questions to settle before proposing the change

The most important unknown is the motor—not the MOSFET. We should identify:

- The exact motor or at least its rated voltage and measured stall current.
- The available flywheel diode; a suitably rated Schottky diode would be preferable for repeated PWM switching.
- Whether the Snap Circuits RV has three independently usable terminals and will remain wired between Arduino 5 V, GND, and A0 as in lesson 02.
- Whether these should be three independent PlatformIO lesson directories, which I recommend, or three sketches inside one lesson.

Once those details are settled, this is ready to become an OpenSpec proposal.

Great, I think I can reasonably answer these.

1. The motor can be the yellow-gearbox motor from the elegoo arduino car 3v-6v and 250ma current no-load, we don't need to worry about stall current here as this is just a demo
2. I have schottky diodes which should be adequate in an Inland 20 v - 1000 v diode assortment kit 11.
3. The RV snap has three terminals and will be wired as described
4. I will take your recommendation, please do this as three separate independent platformIO lesson directories.

Those choices establish a clean three-lesson sequence:

1. `03-pwm-motor-ramp`
2. `04-adc-pwm-motor-control`
3. `05-interrupt-driven-motor-control`

Each will be a standalone PlatformIO project while reusing the wiring and concepts introduced previously.

The timing architecture should be:

```text
D3 PWM ───────── Timer2 ───────── Motor MOSFET
100 ms sampling ─ Timer1 interrupt ─ ADC/PWM update
500 ms delay ─── Timer0/foreground ─ Serial reporting
```

This separation prevents the interrupt timer from disrupting PWM or Arduino’s `delay()` timing.

One adjustment: we cannot entirely disregard stall current. The motor experiences it briefly at every startup, and low PWM values may hold it stalled. We do not need an exact characterization experiment, but the protective components must tolerate it. I recommend specifying a **1N5822 Schottky diode (3 A, 40 V)** if your assortment contains one. Otherwise, we should select by printed part number—not merely “a Schottky diode.” The Inland listing identifies a broad 20–1000 V assortment but does not clearly establish every diode’s current rating. [Inland assortment listing](https://www.microcenter.com/product/632686/inland-20-v-1000-v-diode-assortment-kit-12-values-270-pcs)

The settled electrical requirements are:

- Yellow 3–6 V gearbox motor, approximately 250 mA unloaded.
- B1 two-AA supply powers only the motor circuit.
- Arduino remains USB-powered.
- B1 negative, MOSFET source, RV ground, and Arduino GND share a common node.
- D3 drives the RFP30N06LE gate through 220–330 Ω.
- A 10 kΩ gate-to-ground resistor keeps the motor off during reset.
- The diode connects directly across the motor, cathode toward B1 positive.
- RV outer terminals connect to Arduino 5 V and GND; its wiper connects to A0.
- Motor is secured without a wheel or propeller.
- All wiring changes happen with USB and B1 disconnected.

The three demonstrations can now have sharply separated learning outcomes:

- Lesson 03: duty cycle versus actual motor behavior, including the low-duty dead zone.
- Lesson 04: direct ADC-to-PWM mapping with intentionally sluggish 500 ms blocking updates.
- Lesson 05: Timer1 updates ADC/PWM every 100 ms while the foreground loop remains blocked for 500 ms between serial reports.

This is sufficiently defined for an OpenSpec proposal. The only remaining inventory detail is confirming the diode’s printed part number; `1N5822` is the preferred choice.

Use the **1N5817**. It is the best fit here:

- Schottky and intended for freewheeling applications.
- 1 A average forward current and 25 A brief surge rating.
- 20 V reverse rating—ample for the 3 V motor supply.
- Lower forward drop than the 1N5819.

The **1N5819** is also acceptable, but its extra 40 V rating is unnecessary here. The 1N400x and 1N540x parts are slower rectifiers, while the 1N4148 lacks adequate current capacity. [Vishay 1N5817/1N5819 datasheet](https://www.vishay.com/docs/88525/1n5817.pdf)

Final placement:

```text
B1+ / motor+ ─────── cathode (banded end)
motor− / MOSFET drain ── anode
```

So the design can now explicitly specify the 1N5817 rather than a generic flywheel diode. The hardware and three-lesson structure are sufficiently settled to create the OpenSpec proposal.