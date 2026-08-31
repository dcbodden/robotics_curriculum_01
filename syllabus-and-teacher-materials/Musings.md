## Flywheel diode, MOSFET selection

Something I noticed in a lot of criticisms of Arduino PWM motor control examples was concern about the poor selection of MOSFETs and flywheel diodes.

This MOSFET was selected because it's a TTL level at the gate - the Arduino can push the gate to the needed level to fully open the channel, which isn't true for all MOSFETs. Failing to fully open the gate results in heat and power inefficiency.

This diode was selected because Schottky diodes have good switching speed. A lot of Arduino HOWTOs use rectifier diodes which don't provide fast enough switching speeds to be efficient.