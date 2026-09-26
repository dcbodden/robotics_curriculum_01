// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

namespace control {

constexpr int kAxisMinimum = -512;
constexpr int kAxisMaximum = 511;
constexpr int kAxisDeadZone = 40;
constexpr int kMotorCommandMaximum = 255;
constexpr uint16_t kServoMinimumPulseUs = 1000;
constexpr uint16_t kServoCenterPulseUs = 1500;
constexpr uint16_t kServoMaximumPulseUs = 2000;

static_assert(kAxisMinimum < -kAxisDeadZone, "Negative axis range must extend beyond the dead zone");
static_assert(kAxisMaximum > kAxisDeadZone, "Positive axis range must extend beyond the dead zone");
static_assert(kServoMinimumPulseUs < kServoCenterPulseUs &&
                  kServoCenterPulseUs < kServoMaximumPulseUs,
              "Servo pulse limits must surround center");

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
        return static_cast<uint16_t>(kServoCenterPulseUs - offset);
    }

    const int offset = scaleRounded(axis - kAxisDeadZone, kAxisMaximum - kAxisDeadZone,
                                    kServoMaximumPulseUs - kServoCenterPulseUs);
    return static_cast<uint16_t>(kServoCenterPulseUs + offset);
}

inline bool leftStickIsNeutral(int axisX, int axisY) {
    return axisIsNeutral(axisX) && axisIsNeutral(axisY);
}

inline MappedCommands mapLeftStick(int axisX, int axisY) {
    return {motorCommandFromAxisY(axisY), servoPulseFromAxisX(axisX)};
}

}  // namespace control
