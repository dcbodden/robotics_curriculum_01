## Why

Lesson 02 proves Bluetooth-controlled actuator behavior on a split bench supply, but it intentionally does not define a battery-powered path toward a rover. A staged follow-on is needed to establish a protected 3S power architecture, validate a single DRV8833 with two lower-voltage motors, and collect electrical evidence before adding the Wi-Fi camera load.

## What Changes

- Add a staged rover power-integration activity based on a protected 3S 18650 source and three LM2596 buck-converter branches.
- In phase 1, use one LM2596 adjusted to 5.0 V for the Bluetooth ESP32-WROOM and one LM2596 adjusted to a measured starting target between 4.0 and 4.5 V for one HW-627/DRV8833 driving two secured, unloaded TT motors, one motor per H-bridge.
- Require phase-1 multimeter checks of protected-pack voltage and polarity, both LM2596 outputs before connection, Bluetooth ESP32 current by itself, driver-and-motor current by itself, and the integrated current and rail voltages during idle, individual-motor startup, and simultaneous-motor startup.
- In phase 2, add an ESP32-S3 N16R8 camera board with OV3660 on the third LM2596 adjusted to 5.0 V only after phase 1 passes its voltage, current, temperature, and reset-observation gates.
- Add a block diagram, power budget, wiring table, capacitor and suppression guidance, measurement-record tables, shutdown procedure, and explicit stop conditions.
- Preserve Lesson 02 as the existing USB/bench-supply lesson; the new material is a follow-on power-integration activity and does not make the lesson's original bench missions battery-powered.
- Keep independent differential-drive mapping, chassis motion, loaded-wheel testing, charging-circuit construction, and additional servos or steppers outside this change.

## Capabilities

### New Capabilities

- `esp32-rover-power-integration`: Defines the staged 3S/LM2596 rover power architecture, two-motor DRV8833 bench integration, multimeter evidence, phase gates, and later camera-power integration.

### Modified Capabilities

None. The existing `esp32-bluetooth-actuator-experiment` remains a bench-only prerequisite.

## Impact

- Adds follow-on curriculum and teacher-facing electrical-validation material, primarily under `syllabus-and-teacher-materials/Explorations/` and through links from relevant ESP32 curriculum indexes or Lesson 02 boundary documentation.
- Reuses the inspected HW-627, two Elegoo-style TT motors, the ESP32-WROOM Bluetooth controller setup, the ESP32-S3 camera board, and three LM2596 modules.
- Introduces manual hardware verification involving a protected 3S pack, balancing BMS, fuse and cutoff, star distribution, branch-local capacitance, and current measurements made with a suitably fused multimeter.
- Does not alter the standalone Bluetooth diagnostic baseline or authorize unprotected cells, deliberate motor stalls, moving-rover trials, or simultaneous USB and external 5 V connection without verified power-path isolation.
