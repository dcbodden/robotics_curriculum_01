## ADDED Requirements

### Requirement: Standalone two-axis motor and servo lesson
The curriculum SHALL provide `06-joystick-motor-servo-control` as an independently buildable Arduino Uno PlatformIO project with fifth-grade instructions. It SHALL use an Elegoo analog joystick to control the existing protected DC motor and an SG90 position servo. Lessons 04 and 05 and their shared worksheet SHALL retain their existing behavior and wiring.

#### Scenario: Open the follow-on project
- **WHEN** a student opens lesson 06
- **THEN** the project builds independently and its instructions identify lesson 05 as prior learning

### Requirement: Twenty-millisecond two-channel interrupt control
Lesson 06 SHALL read joystick axes on A0 and A1 and update the D3 motor power command and D9 servo position command from a timer interrupt every 20 ms, approximately 50 times per second. Both output signals SHALL continue between command updates. The servo signal SHALL repeat on a nominal 20 ms frame with bounded position pulse widths, distinct from motor duty-cycle PWM. The foreground SHALL report a consistent snapshot of both readings and commands approximately once per second while blocking between reports, without serial output or pulse-length waits inside the control interrupt.

#### Scenario: Move both axes while the foreground waits
- **WHEN** both joystick axes change during the foreground's reporting wait
- **THEN** the interrupt routine reads both channels and updates both commands on its 20 ms schedule without waiting for the next report

#### Scenario: Observe the signals
- **WHEN** the servo and motor outputs are measured
- **THEN** servo frames repeat approximately every 20 ms with the selected bounded pulse width while motor PWM continues at its separate switching frequency

### Requirement: Center-off motor and bounded servo mapping
The centered motor-control axis, its neutral deadband, and the opposite direction SHALL command zero motor power. Movement beyond the deadband in the active direction SHALL increase the command monotonically to its maximum. The other axis SHALL map to a bounded servo position with center corresponding to the middle command. Instructions SHALL explain the active direction, neutral check, and conservative servo limits without promising exact angles or instant physical response.

#### Scenario: Release the joystick
- **WHEN** the motor-control axis returns to the verified neutral deadband
- **THEN** the next control check commands zero motor power, although the motor may coast

#### Scenario: Use the opposite direction or endpoints
- **WHEN** the motor axis moves in the inactive direction or the servo axis reaches either endpoint
- **THEN** motor power remains zero in the inactive direction and servo commands remain within the documented limits

### Requirement: External five-volt actuator power
Lesson 06 SHALL document a regulated external 5 V supply for the servo and the documented 3–6 V DC motor, selected for their combined starting/stall current with suitable margin. USB SHALL power the Uno and Arduino 5 V SHALL power the joystick. External actuator positive SHALL remain separate from Arduino 5 V and VIN; actuator negative, servo ground, MOSFET source, joystick ground, and Arduino GND SHALL share a ground reference. The motor SHALL retain its MOSFET, gate series resistor, gate pulldown, and correctly oriented flywheel diode. The servo SHALL receive power directly from the actuator supply and a separate control signal.

#### Scenario: Prepare power with the teacher
- **WHEN** the teacher checks the unpowered lesson 06 circuit
- **THEN** the instructions support verification of 5 V polarity, combined current capacity, shared ground, isolated positive supplies, motor protection, and servo connector polarity before actuator power is enabled

### Requirement: Guided two-axis investigation
Lesson 06 SHALL provide wiring, build/upload, preparation, an observation activity, and troubleshooting. It SHALL require all power removed before wiring changes, actuator power off during uploads, teacher verification before power, a secured bare motor without a wheel or propeller, and a secured unloaded servo with a clear motion area. The activity SHALL separately exercise each axis and then both together, record observations honestly, and explain 50 command checks per second versus servo frames, motor PWM pulses, and serial reporting. Lesson 05 and the lesson index SHALL link to lesson 06.

#### Scenario: Compare independent and combined controls
- **WHEN** a student follows the approved activity
- **THEN** the student observes motor neutral and active-direction behavior, bounded servo movement, and both controls responding between serial reports without handling moving parts

#### Scenario: Unexpected behavior occurs
- **WHEN** a motor fails to turn when commanded, a servo binds or persistently buzzes, parts become hot, or power becomes unstable
- **THEN** instructions direct the teacher and student to remove actuator and USB power and inspect the setup before continuing
