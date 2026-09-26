// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Imain test/test_control_mapping.cpp -o /tmp/lesson02-mapping-check
// /tmp/lesson02-mapping-check

#include "control_mapping.h"

#include <assert.h>
#include <stdio.h>

int main() {
    using namespace control;

    assert(clampAxis(-1000) == -512);
    assert(clampAxis(1000) == 511);

    assert(motorCommandFromAxisY(-1000) == 255);
    assert(motorCommandFromAxisY(-512) == 255);
    assert(motorCommandFromAxisY(-41) == 1);
    assert(motorCommandFromAxisY(-40) == 0);
    assert(motorCommandFromAxisY(0) == 0);
    assert(motorCommandFromAxisY(40) == 0);
    assert(motorCommandFromAxisY(41) == -1);
    assert(motorCommandFromAxisY(511) == -255);
    assert(motorCommandFromAxisY(1000) == -255);

    assert(servoPulseFromAxisX(-1000) == 1000);
    assert(servoPulseFromAxisX(-512) == 1000);
    assert(servoPulseFromAxisX(-41) == 1499);
    assert(servoPulseFromAxisX(-40) == 1500);
    assert(servoPulseFromAxisX(0) == 1500);
    assert(servoPulseFromAxisX(40) == 1500);
    assert(servoPulseFromAxisX(41) == 1501);
    assert(servoPulseFromAxisX(511) == 2000);
    assert(servoPulseFromAxisX(1000) == 2000);

    assert(leftStickIsNeutral(-40, 40));
    assert(!leftStickIsNeutral(-41, 0));
    assert(!leftStickIsNeutral(0, 41));

    int previousMotor = 255;
    uint16_t previousServo = 1000;
    for (int axis = -512; axis <= 511; ++axis) {
        const int motor = motorCommandFromAxisY(axis);
        const uint16_t servo = servoPulseFromAxisX(axis);
        assert(motor <= previousMotor);
        assert(motor >= -255 && motor <= 255);
        assert(servo >= previousServo);
        assert(servo >= 1000 && servo <= 2000);
        previousMotor = motor;
        previousServo = servo;
    }

    const MappedCommands centered = mapLeftStick(0, 0);
    assert(centered.motor == 0);
    assert(centered.servoPulseUs == 1500);

    puts("Mapping checks passed: inversion, dead zones, endpoints, clamps, and full axis range.");
}
