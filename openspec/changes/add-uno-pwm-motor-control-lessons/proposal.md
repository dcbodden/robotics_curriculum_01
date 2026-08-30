## Why

The existing Arduino Uno lessons establish digital transistor control and ADC-driven timing, but they stop before students control a real inductive load or see why responsive control requires work to continue during a blocking foreground loop. Three cumulative motor lessons will connect those ideas through PWM, potentiometer input, and timer interrupts while introducing a protected external MOSFET power stage.

## What Changes

- Add `03-pwm-motor-ramp` as a standalone PlatformIO lesson that smoothly ramps a motor command from 0% to 100% and back over eight seconds.
- Add `04-adc-pwm-motor-control` as a standalone lesson that maps A0 readings to PWM output and intentionally waits 500 ms between sample-and-report cycles.
- Add `05-interrupt-driven-motor-control` as a standalone lesson that updates ADC-derived PWM every 100 ms from a Timer1 interrupt while the foreground loop reports values every 500 ms.
- Document a teacher-verified, low-side RFP30N06LE motor driver using Arduino D3, a series gate resistor, a gate pulldown resistor, an external two-AA B1 motor supply, a 1N5817 flywheel diode, and shared ground.
- Extend the lesson collection documentation with the new independent projects, prerequisites, student observations, and electrical and mechanical safety checks.

## Capabilities

### New Capabilities

- `arduino-uno-pwm-motor-lessons`: Defines the progressive PWM motor ramp, blocking ADC control, interrupt-driven ADC control, protected MOSFET power circuit, and beginner learning activities.

### Modified Capabilities

None.

## Impact

- Adds three independently buildable projects under `platformio-uno-lessons/` and updates that collection's README.
- Reuses the Arduino Uno, PlatformIO configuration, A0 potentiometer voltage divider, and serial-monitor conventions from lessons 01 and 02.
- Introduces an external 3 V motor supply, yellow 3–6 V gearbox motor, RFP30N06LE MOSFET, 1N5817 Schottky diode, breadboard, and gate resistors.
- Uses D3/Timer2 for PWM, Timer1 for 100 ms interrupts in lesson 05, and Timer0-backed Arduino foreground timing without adding an external timer library.
