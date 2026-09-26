// SPDX-License-Identifier: Apache-2.0

#include "actuator_output.h"

#include <Arduino.h>

#include "bridge_command.h"
#include "control_config.h"

namespace control {

bool ActuatorOutput::begin() {
    if (initialized_) return true;

    // Establish passive-safe levels before any pin is connected to LEDC.
    pinMode(kMotorAin1Pin, OUTPUT);
    pinMode(kMotorAin2Pin, OUTPUT);
    pinMode(kServoSignalPin, OUTPUT);
    digitalWrite(kMotorAin1Pin, LOW);
    digitalWrite(kMotorAin2Pin, LOW);
    digitalWrite(kServoSignalPin, LOW);

    motorAin1Attached_ =
        ledcAttachChannel(kMotorAin1Pin, kMotorPwmFrequencyHz, kMotorPwmResolutionBits, kMotorAin1PwmChannel);
    if (!motorAin1Attached_ || !ledcWrite(kMotorAin1Pin, 0)) {
        detachPwmAndDriveLow();
        return false;
    }

    motorAin2Attached_ =
        ledcAttachChannel(kMotorAin2Pin, kMotorPwmFrequencyHz, kMotorPwmResolutionBits, kMotorAin2PwmChannel);
    if (!motorAin2Attached_ || !ledcWrite(kMotorAin2Pin, 0)) {
        detachPwmAndDriveLow();
        return false;
    }

    servoAttached_ =
        ledcAttachChannel(kServoSignalPin, kServoPwmFrequencyHz, kServoPwmResolutionBits, kServoPwmChannel);
    if (!servoAttached_ || !writeServo(kServoCenterPulseUs)) {
        detachPwmAndDriveLow();
        return false;
    }

    initialized_ = true;
    return true;
}

bool ActuatorOutput::apply(const ControlOutput& output) {
    if (!initialized_) return false;

    const int motorCommand = output.reversalInterlock ? 0 : output.appliedMotor;
    const bool motorWritten = writeMotor(motorCommand);
    const bool servoWritten = writeServo(output.servoPulseUs);
    return motorWritten && servoWritten;
}

bool ActuatorOutput::safeStop() {
    if (!initialized_) return false;
    const bool motorWritten = writeMotor(0);
    const bool servoWritten = writeServo(kServoCenterPulseUs);
    return motorWritten && servoWritten;
}

bool ActuatorOutput::writeMotor(int command) {
    const BridgeCommand bridge = bridgeCommandFromMotor(command);

    // Drop the inactive side first. The policy supplies the zero interval for
    // reversals; this ordering additionally avoids briefly energizing both sides.
    if (bridge.ain1Duty > 0) {
        return ledcWrite(kMotorAin2Pin, 0) && ledcWrite(kMotorAin1Pin, bridge.ain1Duty);
    }
    if (bridge.ain2Duty > 0) {
        return ledcWrite(kMotorAin1Pin, 0) && ledcWrite(kMotorAin2Pin, bridge.ain2Duty);
    }
    return ledcWrite(kMotorAin1Pin, 0) && ledcWrite(kMotorAin2Pin, 0);
}

bool ActuatorOutput::writeServo(uint16_t pulseUs) {
    return ledcWrite(kServoSignalPin, servoDutyFromPulseUs(pulseUs));
}

void ActuatorOutput::detachPwmAndDriveLow() {
    if (motorAin1Attached_) ledcDetach(kMotorAin1Pin);
    if (motorAin2Attached_) ledcDetach(kMotorAin2Pin);
    if (servoAttached_) ledcDetach(kServoSignalPin);

    motorAin1Attached_ = false;
    motorAin2Attached_ = false;
    servoAttached_ = false;
    initialized_ = false;

    pinMode(kMotorAin1Pin, OUTPUT);
    pinMode(kMotorAin2Pin, OUTPUT);
    pinMode(kServoSignalPin, OUTPUT);
    digitalWrite(kMotorAin1Pin, LOW);
    digitalWrite(kMotorAin2Pin, LOW);
    digitalWrite(kServoSignalPin, LOW);
}

}  // namespace control
