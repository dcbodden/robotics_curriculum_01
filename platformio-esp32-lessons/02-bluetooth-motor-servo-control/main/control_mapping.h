// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "control_config.h"

namespace control {

struct MappedCommands {
    int motor;
    uint16_t servoPulseUs;
};

inline int clampAxis(int value) {
    if (value < kAxisMinimum) return kAxisMinimum;
    if (value > kAxisMaximum) return kAxisMaximum;
    return value;
}

inline bool axisIsNeutral(int value) {
    const int axis = clampAxis(value);
    return axis >= -kAxisDeadZone && axis <= kAxisDeadZone;
}

inline int scaleRounded(int magnitude, int inputSpan, int outputSpan) {
    return static_cast<int>((static_cast<int32_t>(magnitude) * outputSpan + inputSpan / 2) / inputSpan);
}

inline int motorCommandFromAxisY(int value) {
    const int axis = clampAxis(value);
    if (axisIsNeutral(axis)) return 0;

    // Bluepad32 reports stick-up as negative Y. Invert it so forward is positive.
    if (axis < -kAxisDeadZone) {
        return scaleRounded(-axis - kAxisDeadZone, -kAxisMinimum - kAxisDeadZone, kMotorCommandMaximum);
    }
    return -scaleRounded(axis - kAxisDeadZone, kAxisMaximum - kAxisDeadZone, kMotorCommandMaximum);
}

inline uint16_t servoPulseFromAxisX(int value) {
    const int axis = clampAxis(value);
    if (axisIsNeutral(axis)) return kServoCenterPulseUs;

    if (axis < -kAxisDeadZone) {
        const int offset = scaleRounded(-axis - kAxisDeadZone, -kAxisMinimum - kAxisDeadZone,
                                        kServoCenterPulseUs - kServoMinimumPulseUs);
        const uint16_t pulse = static_cast<uint16_t>(kServoCenterPulseUs - offset);
        return kServoDirectionInverted ? static_cast<uint16_t>(kServoMinimumPulseUs + kServoMaximumPulseUs - pulse)
                                       : pulse;
    }

    const int offset = scaleRounded(axis - kAxisDeadZone, kAxisMaximum - kAxisDeadZone,
                                    kServoMaximumPulseUs - kServoCenterPulseUs);
    const uint16_t pulse = static_cast<uint16_t>(kServoCenterPulseUs + offset);
    return kServoDirectionInverted ? static_cast<uint16_t>(kServoMinimumPulseUs + kServoMaximumPulseUs - pulse)
                                   : pulse;
}

inline bool leftStickIsNeutral(int axisX, int axisY) {
    return axisIsNeutral(axisX) && axisIsNeutral(axisY);
}

inline MappedCommands mapLeftStick(int axisX, int axisY) {
    return {motorCommandFromAxisY(axisY), servoPulseFromAxisX(axisX)};
}

}  // namespace control
