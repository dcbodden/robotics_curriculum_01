## Why

Lesson 05 teaches frequent control checks for one motor. A follow-on lesson can show how the same checking routine uses two joystick axes to coordinate motor power and servo position, with a checking interval suited to a servo's repeating signal.

## What Changes

- Add standalone lesson `06-joystick-motor-servo-control` for Arduino Uno, building on lesson 05.
- Replace the RV input in this new lesson with an Elegoo joystick: one axis controls motor power, the other controls SG90 servo position.
- Read both analog inputs and update both commands from an interrupt routine every 20 ms (50 times per second); keep roughly one-second serial snapshot reporting in the main loop.
- Use center-off motor behavior: the centered joystick and opposite half of its travel command zero power; movement in the active direction increases power. Include a neutral deadband.
- Use a regulated external 5 V actuator supply instead of B1 in lesson 06, retaining the protected motor switching stage and shared ground.
- Provide fifth-grade instructions, teacher preparation, signal-timing distinctions, power checks, and a two-axis observation activity.
- Link the new lesson from the lesson index and lesson 05. Existing lessons and the robot-attention worksheet remain usable as written.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `arduino-uno-pwm-motor-lessons`: Add a standalone joystick-controlled motor/servo lesson with 20 ms interrupt-driven command updates and external actuator power.

## Impact

New configuration, firmware, and instructions under `platformio-uno-lessons/06-joystick-motor-servo-control/`, plus discovery links. The new lesson needs the kit joystick, SG90 position servo, existing protected DC motor circuit, and a suitably rated regulated 5 V supply. Timer1 will generate the servo signal directly; no Arduino Servo library dependency is planned. Earlier projects, B1/RV wiring, and worksheet content are unchanged.
