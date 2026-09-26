// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "control_policy.h"

namespace control {

class ActuatorOutput {
public:
    bool begin();
    bool apply(const ControlOutput& output);
    bool safeStop();
    bool initialized() const { return initialized_; }

private:
    bool writeMotor(int command);
    bool writeServo(uint16_t pulseUs);
    void detachPwmAndDriveLow();

    bool motorAin1Attached_ = false;
    bool motorAin2Attached_ = false;
    bool servoAttached_ = false;
    bool initialized_ = false;
};

}  // namespace control
