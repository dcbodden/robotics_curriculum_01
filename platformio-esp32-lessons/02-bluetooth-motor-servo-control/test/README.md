# Host checks

Run the pure mapping, safety-policy, and actuator-command checks without an ESP32, Bluetooth
controller, motor driver, servo, or PlatformIO dependency tree:

```bash
./test/run-host-checks.sh
```

The script uses the host `g++` compiler in C++11 mode with `-Wall -Wextra
-Werror`. It builds into a temporary directory and leaves no generated files in
the lesson.

`test_control_mapping.cpp` checks the complete nominal axis range plus values
beyond it. It covers both boundaries of each ±40 dead zone, input inversion,
forward and reverse endpoints, output clamps, servo centering, and monotonic
motor and servo mapping.

`test_control_policy.cpp` checks first-valid-controller selection, ignored
secondary controllers, no silent handoff, neutral-before-arm behavior, OPTIONS
and CIRCLE edges, explicit disarm, invalid input, the exact 300 ms watchdog
boundary, disconnect and reconnect behavior, 20 ms/16-count slew updates,
immediate safety zero, and the full zero interval before reversal.

`test_actuator_commands.cpp` checks the complete DRV8833 channel-A truth table,
command clamping, mutual exclusion of AIN1/AIN2 drive, safe-zero output for
startup, disarm, failsafe, and reversal interlock, plus bounded conversion from
servo pulse widths to 50 Hz LEDC duty values.
