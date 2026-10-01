With the work in platformio-esp32-lessons/02-bluetooth-motor-servo-control being extensible to a remote-controlled rover, we need to design a power architecture based on battery output and power consumption by the microcontrollers and motors.

Power drains:
- primary ESP32-WROOM devkit-v1 running bluetooth classic
    - We'll supply 5VDC upstream of its linear regulator since it doesn't like to be backpowered at 3.3 VDC
    - We believe this microcontroller will consume 150-200 milliamps with bluetooth classic running

- two Elegoo TT motors
    - The approximate stall and startup current values depend on our power supply voltage - At 6V: ~1.2 A to 1.5
    - 6V seems a desirable starting point
- a second ESP32-S3-WROOM N16R8 module with an OV3660 camera running with wifi
    - need to determine what voltage to supply and where, likely also 5VDC ahead of the linear regulator
    - unclear what the power consumption is with wifi running

Assumptions:
- I'd like to use 2-4 18650 LION batteries and LM2596 DC/DC buck converters.
- I have multiple (more than three) LM2596 converters.
(Fresh AA Batteries)
Efficiency is not a goal for this stage. Robust motor and ESP32 performance are to be prioritized. Double-check provided expected current draws, search for current draws where none are provided.

Develop a proposal for a power architecture that draws current from the batteries, through the LM2596s at proper levels for the microcontrollers and motors, with recommendations for capacitors where desirable, and produce a block diagram, a power budget, a wiring detail table, and the recommendations for 2, 3, or 4 18650s for best performance, with a mention of how much surplus power is left over here for consideration if we want to add a second set of drive motors, stepper motors, or servos.

