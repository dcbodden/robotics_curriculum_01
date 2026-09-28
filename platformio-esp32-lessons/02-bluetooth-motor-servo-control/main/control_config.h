// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

namespace control {

// Bluepad32 input range and centered control calibration.
constexpr int kAxisMinimum = -512;
constexpr int kAxisMaximum = 511;
constexpr int kAxisDeadZone = 40;

// Signed drive command and normal policy cadence.
constexpr int kMotorCommandMaximum = 255;
constexpr bool kMotorDirectionInverted = false;
constexpr uint32_t kControlUpdateIntervalMs = 20;
constexpr int kMotorSlewPerUpdate = 16;
constexpr uint32_t kFreshReportWatchdogMs = 300;

// DRV8833 channel A output resources.
constexpr uint8_t kMotorAin1Pin = 26;
constexpr uint8_t kMotorAin2Pin = 27;
constexpr uint8_t kDriverSleepPin = 13;
constexpr bool kDriverFaultTelemetryEnabled = false;
constexpr uint8_t kMotorAin1PwmChannel = 0;
constexpr uint8_t kMotorAin2PwmChannel = 1;
constexpr uint32_t kMotorPwmFrequencyHz = 20000;
constexpr uint8_t kMotorPwmResolutionBits = 8;
constexpr uint32_t kMotorPwmMaximumDuty = (1UL << kMotorPwmResolutionBits) - 1UL;

// Servo output calibration. Narrow the endpoints if the physical servo binds.
constexpr uint8_t kServoSignalPin = 25;
constexpr uint8_t kServoPwmChannel = 2;
constexpr uint32_t kServoPwmFrequencyHz = 50;
constexpr uint8_t kServoPwmResolutionBits = 16;
constexpr uint32_t kServoPwmMaximumDuty = (1UL << kServoPwmResolutionBits) - 1UL;
constexpr uint32_t kServoPwmPeriodUs = 1000000UL / kServoPwmFrequencyHz;
constexpr uint16_t kServoMinimumPulseUs = 1000;
constexpr uint16_t kServoCenterPulseUs = 1500;
constexpr uint16_t kServoMaximumPulseUs = 2000;
constexpr bool kServoDirectionInverted = false;

// Control diagnostics are rate-limited independently of the 20 ms control loop.
constexpr uint32_t kControlTelemetryIntervalMs = 200;

static_assert(kAxisMinimum < -kAxisDeadZone, "Negative axis range must extend beyond the dead zone");
static_assert(kAxisMaximum > kAxisDeadZone, "Positive axis range must extend beyond the dead zone");
static_assert(kMotorCommandMaximum == static_cast<int>(kMotorPwmMaximumDuty),
              "Motor command range must match the 8-bit PWM duty range");
static_assert(kMotorAin1Pin != kMotorAin2Pin && kMotorAin1Pin != kServoSignalPin &&
                  kMotorAin2Pin != kServoSignalPin && kDriverSleepPin != kMotorAin1Pin &&
                  kDriverSleepPin != kMotorAin2Pin && kDriverSleepPin != kServoSignalPin,
              "Actuator GPIO assignments must be unique");
static_assert(kMotorAin1PwmChannel != kMotorAin2PwmChannel &&
                  kMotorAin1PwmChannel != kServoPwmChannel &&
                  kMotorAin2PwmChannel != kServoPwmChannel,
              "Actuator PWM channels must be unique");
static_assert(kMotorPwmFrequencyHz >= 18000 && kMotorPwmFrequencyHz <= 22000,
              "Motor PWM should remain near 20 kHz");
static_assert(kServoPwmFrequencyHz == 50, "Hobby servo PWM must run at 50 Hz");
static_assert(kServoPwmResolutionBits >= 15, "Servo PWM needs sub-microsecond-scale duty resolution");
static_assert(kServoMinimumPulseUs < kServoCenterPulseUs &&
                  kServoCenterPulseUs < kServoMaximumPulseUs &&
                  kServoMaximumPulseUs < kServoPwmPeriodUs,
              "Servo pulse limits must surround center and fit the PWM period");
static_assert(kFreshReportWatchdogMs <= 500 && kFreshReportWatchdogMs > kControlUpdateIntervalMs,
              "Watchdog must meet the specification and exceed one control update");
static_assert(kMotorSlewPerUpdate > 0 && kMotorSlewPerUpdate <= kMotorCommandMaximum,
              "Motor slew step must fit the command range");
static_assert(kControlTelemetryIntervalMs >= kControlUpdateIntervalMs,
              "Telemetry must not run faster than the control update");

}  // namespace control
