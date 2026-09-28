# HW-627 specimen inspection

This record applies to the photographed module in `photos/hw-627-front.jpeg`
and `photos/hw-627-back.jpeg`. It is not a claim that every board sold as
HW-627 has the same layout.

## Observed labels and population

The back silkscreen exposes these edge connections:

- left edge, top to bottom: `IN4`, `IN3`, `GND`, `VCC`, `IN2`, `IN1`;
- right edge, top to bottom: `EEP`, `OUT1`, `OUT2`, `OUT3`, `OUT4`, `ULT`;
- an unbridged two-pad solder jumper marked `J2`.

The shortened `EEP` and `ULT` labels and their routing correspond to the
DRV8833's active-low `nSLEEP` and `nFAULT` pins respectively. The firmware uses
GPIO 13 for the exposed `EEP`/`nSLEEP` input. It holds that signal low while
establishing safe motor-input levels and raises it only after PWM initialization.
The exposed `ULT`/`nFAULT` output is recorded but remains unwired: fault
telemetry is optional and this lesson has not assigned another ESP32 GPIO to it.

The front photograph shows:

- a 16-pin Texas Instruments device marked `DRV8833`;
- one indicator LED;
- resistors marked `472` (4.7 kohms) and `473` (47 kohms);
- capacitors in footprints marked `2.2uF`, `0.01uF`, and `10uF`.

Those visible parts do not establish that a 100 nF ceramic bypass capacitor is
present directly across `VCC` and `GND`, nor do they replace external bulk
capacitance sized for the motor and servo transients. Wiring and supplementary
capacitance are handled in the later bench-wiring tasks.

The DRV8833 signal interpretation is checked against Texas Instruments'
DRV8833 data sheet, revision E. `nSLEEP` is active-low; `nFAULT` is an output.
