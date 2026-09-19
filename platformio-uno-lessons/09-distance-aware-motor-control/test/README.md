# Lesson 09 Host Checks

Run these checks from the `09-distance-aware-motor-control` lesson directory. They use only the computer's C++ compiler, so an Uno, HC-SR04, joystick, motor, and servo are not required.

## Ultrasonic Conversion

```bash
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson09-ultrasonic-check
/tmp/lesson09-ultrasonic-check
```

Expected result:

```text
Ultrasonic checks passed: zero, 10 cm, 50 cm, and 4,000 mm conversions.
```

This checks the pure foreground conversions from Echo microseconds to fractional round-trip milliseconds and one-way centimeters. The 4,000 mm case verifies the conversion formula; lesson 09's interrupt acquisition deliberately rejects a measurement that does not finish before the next 20 ms control frame.

## Distance-Aware Motor Policy

```bash
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_distance_motor_policy.cpp -o /tmp/lesson09-policy-check
/tmp/lesson09-policy-check
```

Expected result:

```text
Distance policy checks passed: 579/580/581 us, invalid readings, and all 256 PWM requests.
```

This checks the strict 580 microsecond boundary, 50 percent integer slowdown, zero and maximum requests, odd-request rounding, full control for invalid readings, and all PWM requests from 0 through 255.

## Preserved Lesson 06 Control Mappings

```bash
g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_control_mapping.cpp -o /tmp/lesson09-mapping-check
/tmp/lesson09-mapping-check
```

Expected result:

```text
Mapping checks passed: neutral, boundaries, endpoints, clamps, and all 1024 ADC values.
```

This checks the inherited center-off motor mapping and bounded servo mapping, including their boundary values and every ADC input from 0 through 1023.
