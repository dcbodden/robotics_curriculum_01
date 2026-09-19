#pragma once

#include <stdint.h>

// 580 us is the approximate HC-SR04 round-trip Echo time at 10 cm.
// The boundary is strict: only a valid duration below it selects slowdown.
constexpr uint32_t MOTOR_SLOWDOWN_ECHO_THRESHOLD_US = 580UL;
constexpr uint8_t MOTOR_SLOWDOWN_DIVISOR = 2;

constexpr bool shouldSlowMotorForEcho(bool echoMeasurementValid, uint32_t echoDurationUs) {
	return echoMeasurementValid && echoDurationUs < MOTOR_SLOWDOWN_ECHO_THRESHOLD_US;
}

constexpr uint8_t appliedMotorPwm(uint8_t requestedMotorPwm, bool slowdownActive) {
	// Integer division implements 50 percent without exceeding the 0-255 input range.
	return slowdownActive
		? static_cast<uint8_t>(requestedMotorPwm / MOTOR_SLOWDOWN_DIVISOR)
		: requestedMotorPwm;
}

constexpr uint8_t appliedMotorPwmForEcho(
	uint8_t requestedMotorPwm,
	bool echoMeasurementValid,
	uint32_t echoDurationUs
) {
	return appliedMotorPwm(
		requestedMotorPwm,
		shouldSlowMotorForEcho(echoMeasurementValid, echoDurationUs)
	);
}

static_assert(shouldSlowMotorForEcho(true, 579), "A valid sub-10-cm Echo must slow the motor");
static_assert(!shouldSlowMotorForEcho(true, 580), "The 10 cm boundary must retain full control");
static_assert(!shouldSlowMotorForEcho(false, 579), "An invalid Echo must retain full control");
static_assert(appliedMotorPwm(0, true) == 0, "Slowdown must preserve a zero request");
static_assert(appliedMotorPwm(255, true) == 127, "Slowdown must halve maximum PWM safely");
static_assert(appliedMotorPwmForEcho(200, true, 579) == 100,
	"A valid near measurement must apply 50 percent PWM");
static_assert(appliedMotorPwmForEcho(200, true, 580) == 200,
	"A valid measurement at the boundary must preserve full PWM");
static_assert(appliedMotorPwmForEcho(255, false, 0) == 255,
	"An invalid measurement must preserve the full requested PWM");
