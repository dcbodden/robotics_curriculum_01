# Lesson 05 Timer and Shared-Data Reference for Teachers

The student [Core Mission](README.md#core-mission-can-the-motor-notice-an-rv-change-sooner) compares one RV check per 1,000 ms with ten checks in the same interval. This page explains the authentic [`src/main.cpp`](src/main.cpp) implementation. Student assessment does not require register calculations, interrupt mechanics, or shared-data syntax.

## Foreground and ISR Roles

The **foreground** is the normal work in `setup()` and `loop()`. In lesson 05, `loop()` copies the latest ADC and PWM values, prints them, and waits 1,000 ms. That wait blocks the foreground, just as `delay(1000)` blocks lesson 04's loop.

Timer1 generates a compare-A interrupt every 100 ms. The Arduino briefly pauses foreground work and runs `ISR(TIMER1_COMPA_vect)`. The ISR reads A0, maps the 0–1023 ADC value to a 0–255 PWM command, applies it on D3, and saves the latest pair. It does **no serial printing**. After the ISR finishes, foreground work resumes. The routines are interleaved, not simultaneous. Motor command updates can therefore continue during the foreground's one-second report wait. D3's PWM pulse generation is another rhythm: it continues using the last selected command between ISR calls. Ten command updates per second does not mean ten PWM pulses per second.

## Why the Timer Compare Value Is 24,999

The Uno runs at 16,000,000 clock ticks per second. With Timer1's 64:1 prescaler, Timer1 receives **250,000 ticks per second**. A 100 ms interval needs **25,000 ticks**. In clear-timer-on-compare (CTC) mode, the counter counts from **0 through 24,999**, so `OCR1A = 24999` produces the interval. `TCCR1B` sets CTC with `WGM12` and the prescaler with `CS11 | CS10`; `TIMSK1` enables compare-A interrupts through `OCIE1A`. The setup resets Timer1's control and count registers before starting it. Timer1 configuration leaves Timer0's Arduino delay timing and Timer2's D3 PWM mode intact.

The interval is a controller schedule, not a guarantee that the motor reaches a new physical speed in 100 ms. ADC conversion and ISR execution take time, and motor friction, starting threshold, and coasting also affect the observed response.

## Why the Values Need a Snapshot

`latestAdcReading` and `latestPwmCommand` are marked `volatile` because the ISR can change them between ordinary foreground instructions. `volatile` prevents the compiler from treating a previously fetched value as permanently current. It does **not** make a multi-byte read indivisible or guarantee that two related values come from the same ISR update.

The Uno handles data in 8-bit pieces, while the 0–1023 ADC value occupies 16 bits. An ISR could otherwise change one byte while `loop()` copies the other, or update the ADC value between copying the ADC and PWM values. `ATOMIC_BLOCK(ATOMIC_RESTORESTATE)` briefly pauses interrupts, copies both into a consistent local pair, then restores the prior interrupt state. Slow serial printing occurs **after** interrupts resume, so serial output does not hold up the 100 ms control schedule.

Calling `analogRead()` inside this ISR is an intentional demonstration trade-off. It waits synchronously for an ADC conversion and thus makes the ISR longer than generally preferred. Here the bounded 10 Hz rate and lack of serial work keep the example direct. More advanced designs may start ADC conversion separately or leave longer work to foreground code. The teaching lesson remains how often changed information is **used**, not a general ISR design rule.

## Teacher Timer Diagnosis

Always follow the student's [power-off and stop conditions](README.md#core-mission-can-the-motor-notice-an-rv-change-sooner) before investigating wires or source code. Keep B1 off during every upload.

- **ADC changes but reported PWM does not:** Confirm the ISR maps the 0–1023 input to 0–255 and publishes both `latestAdcReading` and `latestPwmCommand` after applying D3 PWM. Also verify the foreground copies a consistent pair.
- **Motor changes only around new serial messages:** Use the same low-to-high RV movement just after a report, and account for motor starting threshold or coasting before concluding timing is wrong. Check `OCR1A == 24999`, Timer1 CTC and the 64:1 prescaler, compare-A interrupt enable, and that `analogRead()` and `analogWrite()` remain inside `ISR(TIMER1_COMPA_vect)`. Confirm the uploaded firmware is lesson 05. If the command changes between reports but motion remains slow, investigate the motor behavior separately.
- **Reports do not appear:** Check the 9,600-baud Monitor, upload result, and foreground `Serial` calls. Printing belongs in `loop()`, not inside the ISR.
- **Motor or Uno stutters or resets:** Remove B1 and USB power, then inspect loose motor-current wiring, shared ground, B1 batteries, the protected D3 gate path, and 1N5817 polarity before another trial.
