// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "control_config.h"

namespace control {

struct BridgeCommand {
    uint8_t ain1Duty;
    uint8_t ain2Duty;
};

inline int clampMotorCommand(int command) {
    if (command < -kMotorCommandMaximum) return -kMotorCommandMaximum;
    if (command > kMotorCommandMaximum) return kMotorCommandMaximum;
    return command;
}

inline BridgeCommand bridgeCommandFromMotor(int command) {
    int oriented = clampMotorCommand(command);
    if (kMotorDirectionInverted) oriented = -oriented;

    if (oriented > 0) return {static_cast<uint8_t>(oriented), 0};
    if (oriented < 0) return {0, static_cast<uint8_t>(-oriented)};
    return {0, 0};
}

inline uint16_t clampServoPulseUs(uint16_t pulseUs) {
    if (pulseUs < kServoMinimumPulseUs) return kServoMinimumPulseUs;
    if (pulseUs > kServoMaximumPulseUs) return kServoMaximumPulseUs;
    return pulseUs;
}

inline uint32_t servoDutyFromPulseUs(uint16_t pulseUs) {
    const uint32_t boundedPulseUs = clampServoPulseUs(pulseUs);
    return (boundedPulseUs * kServoPwmMaximumDuty + kServoPwmPeriodUs / 2UL) / kServoPwmPeriodUs;
}

}  // namespace control
