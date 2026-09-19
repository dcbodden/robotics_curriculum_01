## Purpose

Defines a three-lesson Arduino Uno progression that introduces HC-SR04 distance measurement, external-interrupt pulse capture, and distance-informed motor control through safe, independently buildable activities.

## ADDED Requirements

### Requirement: Standalone ultrasonic lesson progression
The curriculum SHALL provide independently buildable PlatformIO projects named `07-ultrasonic-blocking-distance`, `08-interrupt-ultrasonic-joystick-control`, and `09-distance-aware-motor-control`. Lesson 07 SHALL introduce direct blocking measurement, lesson 08 SHALL add interrupt-driven measurement to lesson 06's joystick motor/servo baseline, and lesson 09 SHALL use that measurement to modify motor power.

#### Scenario: Open an ultrasonic lesson
- **WHEN** a student opens any lesson 07–09 directory as a PlatformIO project
- **THEN** it builds independently without compiling another lesson and identifies the preceding lesson as recommended prior learning

### Requirement: Shared HC-SR04 wiring and measurement meaning
Lessons 07–09 SHALL identify the supported sensor as the four-pin 5 V HC-SR04 and SHALL connect VCC to Arduino 5 V, GND to Arduino/shared ground, Trigger to D4, and Echo to D2/INT0. They SHALL issue a Trigger pulse of at least 10 microseconds and explain that the Echo HIGH duration represents ultrasonic round-trip time, so calculated one-way distance divides the traveled distance by two.

#### Scenario: Wire the sensor with power removed
- **WHEN** the student follows a lesson's wiring instructions before applying power
- **THEN** the HC-SR04 power, ground, Trigger, and Echo connections match the documented Uno pins and the instructions require USB and actuator power to be removed before wiring changes

#### Scenario: Interpret a valid Echo pulse
- **WHEN** the lesson receives a valid Echo HIGH duration
- **THEN** it can report the round-trip time and calculate distance in centimeters without presenting the round-trip path as a one-way distance

### Requirement: Blocking distance measurement lesson
Lesson 07 SHALL initiate one measurement, use a bounded blocking Echo-pulse wait, report the valid round-trip delay in fractional milliseconds and the calculated distance in centimeters, and then use a 500 millisecond blocking delay before repeating. The timeout SHALL cover the HC-SR04's advertised 4,000 millimeter range without using the unbounded/default one-second wait. A missing or timed-out Echo SHALL be reported explicitly and SHALL NOT be represented as a zero-distance measurement.

#### Scenario: Observe a valid blocking measurement
- **WHEN** a target produces an Echo within the configured lesson 07 timeout
- **THEN** the serial monitor reports its fractional round-trip milliseconds and calculated centimeters before the program enters its 500 millisecond delay

#### Scenario: Observe a missing Echo
- **WHEN** no complete Echo pulse arrives before lesson 07's bounded timeout
- **THEN** the serial monitor identifies the measurement as invalid or missing instead of reporting zero centimeters

### Requirement: Eighty-millisecond interrupt measurement schedule
Lessons 08 and 09 SHALL retain lesson 06's nominal 20 millisecond Timer1 joystick-control schedule and SHALL initiate an HC-SR04 measurement at the end of every fourth control frame, giving 80 milliseconds between Trigger pulses. They SHALL continue every joystick ADC read and command cycle without skipping it for ultrasonic acquisition. A measurement that remains incomplete when the following 20 millisecond control frame begins SHALL expire and later edges from that measurement SHALL be ignored.

#### Scenario: Start a scheduled measurement
- **WHEN** the fourth 20 millisecond control frame completes its joystick reads and actuator command updates
- **THEN** the program issues the Trigger pulse as the final timed operation without omitting that frame's joystick work

#### Scenario: Expire an overly long or missing Echo
- **WHEN** an interrupt-driven measurement is still waiting for a rising or falling Echo edge at the start of the next Timer1 control frame
- **THEN** the program marks that measurement invalid, ignores any later unmatched edge, and performs the new frame's joystick reads and command updates

### Requirement: D2 external-interrupt Echo capture
Lessons 08 and 09 SHALL use D2's external interrupt on Echo signal changes. A recognized rising edge SHALL record the pulse start, and its matching falling edge SHALL publish the elapsed Echo duration as a valid measurement. The edge handler SHALL perform no Serial output, and foreground reporting SHALL use consistent snapshots of multi-byte interrupt data.

#### Scenario: Capture both Echo edges
- **WHEN** D2 rises and then falls for the active measurement before the next control frame
- **THEN** the external-interrupt logic publishes the elapsed HIGH duration for distance interpretation

#### Scenario: Report while interrupts continue
- **WHEN** the foreground takes and prints a measurement snapshot
- **THEN** it obtains a consistent duration and validity state without leaving interrupts disabled during Serial output

### Requirement: Interrupt lesson preserves coordinated control
Lesson 08 SHALL preserve lesson 06's A0 center-off motor input, A1 bounded servo input, D3 motor output, D9 servo signal, nominal 20 millisecond control updates, and protected external actuator circuit. It SHALL report distance measurements or explicit invalid status approximately every 500 milliseconds while joystick motor and servo control continue during the foreground wait.

#### Scenario: Operate controls between reports
- **WHEN** either joystick axis changes while lesson 08's foreground waits between reports
- **THEN** the Timer1 routine continues reading both axes and updating both actuator commands on its nominal 20 millisecond schedule

#### Scenario: Compare control and reporting rhythms
- **WHEN** a student observes lesson 08 telemetry and actuator response
- **THEN** the instructions distinguish 20 millisecond control frames, 80 millisecond sensor triggers, Echo pulse timing, and approximately 500 millisecond serial reports

### Requirement: Distance-aware requested and applied motor commands
Lesson 09 SHALL distinguish the joystick-requested motor PWM command from the command applied to D3. A valid Echo duration representing a distance strictly closer than 10 centimeters SHALL activate a 50 percent slowdown; a valid measurement at or beyond 10 centimeters SHALL apply the full requested command. The latest completed measurement policy SHALL persist during the next measurement attempt, and both the Timer1 control routine and a newly completed Echo measurement SHALL apply the same policy to the latest requested command.

#### Scenario: Detect a target closer than ten centimeters
- **WHEN** lesson 09 completes a valid Echo pulse shorter than the 10 centimeter round-trip threshold
- **THEN** it promptly applies half of the latest requested motor PWM and subsequent 20 millisecond control frames continue applying the slowdown to new joystick requests

#### Scenario: Detect a target at or beyond ten centimeters
- **WHEN** lesson 09 completes a valid measurement at or beyond the 10 centimeter threshold
- **THEN** it applies the full joystick-requested motor PWM

#### Scenario: Preserve zero and bounds
- **WHEN** slowdown is applied to any valid joystick-requested command
- **THEN** a zero request remains zero and the applied command remains within the 0–255 PWM range

### Requirement: Full-control invalid-reading policy
Lesson 09 SHALL treat a measurement that is incomplete at the next 20 millisecond control frame as invalid and SHALL restore or retain full joystick-requested motor control. Invalid or missing Echo results SHALL NOT be presented as evidence of either a zero-distance obstacle or a safety-rated clear path.

#### Scenario: Echo does not complete in one control interval
- **WHEN** the next Timer1 control frame begins before the active Echo measurement completes
- **THEN** lesson 09 clears the slowdown policy, applies the latest full joystick request, and reports the measurement as invalid

### Requirement: Observable ultrasonic telemetry
Lesson 09 SHALL report Echo validity, fractional round-trip milliseconds and calculated centimeters when valid, requested motor PWM, applied motor PWM, and slowdown status approximately every 500 milliseconds. Lesson 08 SHALL report the measurement fields alongside the lesson 06 control telemetry needed to observe continued motor and servo operation.

#### Scenario: Explain a slowdown from telemetry
- **WHEN** a valid near measurement activates slowdown in lesson 09
- **THEN** one report provides enough labeled values for the student to connect the measured distance, requested PWM, 50 percent factor, and applied PWM

### Requirement: Testable measurement and control mappings
The ultrasonic lessons SHALL isolate deterministic distance conversion and distance-aware PWM mapping from hardware-specific interrupt code so host checks can verify representative durations, timeout/invalid behavior, the strict 10 centimeter boundary, the 50 percent factor, numeric bounds, and preservation of lesson 06 joystick and servo mappings.

#### Scenario: Run host checks
- **WHEN** the documented host test commands are run for lessons 07–09
- **THEN** they verify conversion and control-policy boundaries without requiring an attached Uno or HC-SR04

### Requirement: Guided and safe ultrasonic investigations
Each ultrasonic lesson SHALL provide a short Core Mission before deeper implementation detail, include power-off wiring and teacher checkpoints, and distinguish a demonstration from collision avoidance. Lessons 08 and 09 SHALL retain lesson 06's secured bare motor, unloaded secured servo, external actuator-supply separation, motor protection, clear motion areas, upload power rules, and stop conditions. Students SHALL move a stationary target toward and away from the sensor rather than move the test rig toward an obstacle.

#### Scenario: Conduct the distance-aware demonstration
- **WHEN** a student tests the 10 centimeter behavior in lesson 09
- **THEN** the rig remains immobile, the motor remains bare and secured, the target is moved by hand without entering actuator motion areas, and the instructions do not characterize the HC-SR04 behavior as a safety system

