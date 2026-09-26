# Host checks

Run the pure mapping and safety-policy checks without an ESP32, Bluetooth
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
