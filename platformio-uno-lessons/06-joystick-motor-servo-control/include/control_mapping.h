#pragma once

#include <stdint.h>

// Adjust neutral only after checking the actual joystick with actuator power off.
constexpr int ADC_MIN = 0;
constexpr int ADC_MAX = 1023;
constexpr int MOTOR_NEUTRAL_ADC = 512;
constexpr int MOTOR_DEADBAND_ADC = 40;
constexpr int MOTOR_ACTIVE_START_ADC = MOTOR_NEUTRAL_ADC + MOTOR_DEADBAND_ADC;
constexpr uint8_t MOTOR_PWM_MAX = 255;

// Pulse commands describe position, not guaranteed physical angles.
constexpr int SERVO_CENTER_ADC = 512;
constexpr uint16_t SERVO_MIN_PULSE_US = 1000;
constexpr uint16_t SERVO_CENTER_PULSE_US = 1500;
constexpr uint16_t SERVO_MAX_PULSE_US = 2000;

static_assert(MOTOR_DEADBAND_ADC >= 0 &&
	MOTOR_NEUTRAL_ADC - MOTOR_DEADBAND_ADC >= ADC_MIN &&
	MOTOR_ACTIVE_START_ADC < ADC_MAX, "Motor neutral/deadband must fit the ADC range");
static_assert(SERVO_CENTER_ADC > ADC_MIN && SERVO_CENTER_ADC < ADC_MAX,
	"Servo center must be inside the ADC range");
static_assert(SERVO_MIN_PULSE_US < SERVO_CENTER_PULSE_US &&
	SERVO_CENTER_PULSE_US < SERVO_MAX_PULSE_US, "Servo pulse limits must surround center");

inline int clampAdcReading(int reading) {
	if (reading < ADC_MIN) return ADC_MIN;
	if (reading > ADC_MAX) return ADC_MAX;
	return reading;
}

inline uint8_t motorPwmFromAdc(int reading) {
	const int adc = clampAdcReading(reading);
	// Neutral, the deadband, and all inactive-direction travel keep power off.
	if (adc <= MOTOR_ACTIVE_START_ADC) return 0;
	return static_cast<uint8_t>(static_cast<int32_t>(adc - MOTOR_ACTIVE_START_ADC) *
		MOTOR_PWM_MAX / (ADC_MAX - MOTOR_ACTIVE_START_ADC));
}

inline uint16_t servoPulseFromAdc(int reading) {
	const int adc = clampAdcReading(reading);
	// Two halves give an exact middle command despite the unequal ADC spans.
	if (adc <= SERVO_CENTER_ADC) {
		return SERVO_MIN_PULSE_US + static_cast<int32_t>(adc - ADC_MIN) *
			(SERVO_CENTER_PULSE_US - SERVO_MIN_PULSE_US) / (SERVO_CENTER_ADC - ADC_MIN);
	}
	return SERVO_CENTER_PULSE_US + static_cast<int32_t>(adc - SERVO_CENTER_ADC) *
		(SERVO_MAX_PULSE_US - SERVO_CENTER_PULSE_US) / (ADC_MAX - SERVO_CENTER_ADC);
}
