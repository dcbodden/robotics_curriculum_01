// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_distance_motor_policy.cpp -o /tmp/lesson09-policy-check
// /tmp/lesson09-policy-check
#include "distance_motor_policy.h"

#include <assert.h>
#include <stdio.h>

int main() {
	assert(MOTOR_SLOWDOWN_ECHO_THRESHOLD_US == 580UL);
	assert(shouldSlowMotorForEcho(true, 579));
	assert(!shouldSlowMotorForEcho(true, 580));
	assert(!shouldSlowMotorForEcho(true, 581));
	assert(!shouldSlowMotorForEcho(false, 579));
	assert(!shouldSlowMotorForEcho(false, 580));
	assert(!shouldSlowMotorForEcho(false, 581));

	assert(appliedMotorPwmForEcho(0, true, 579) == 0);
	assert(appliedMotorPwmForEcho(200, true, 579) == 100);
	assert(appliedMotorPwmForEcho(201, true, 579) == 100);
	assert(appliedMotorPwmForEcho(255, true, 579) == 127);
	assert(appliedMotorPwmForEcho(200, true, 580) == 200);
	assert(appliedMotorPwmForEcho(200, true, 581) == 200);
	assert(appliedMotorPwmForEcho(255, false, 579) == 255);

	for (int requested = 0; requested <= 255; ++requested) {
		const uint8_t requestedPwm = static_cast<uint8_t>(requested);
		const uint8_t slowedPwm = appliedMotorPwmForEcho(requestedPwm, true, 579);
		assert(slowedPwm == requested / 2);
		assert(slowedPwm <= requestedPwm);
		assert(appliedMotorPwmForEcho(requestedPwm, true, 580) == requestedPwm);
		assert(appliedMotorPwmForEcho(requestedPwm, true, 581) == requestedPwm);
		assert(appliedMotorPwmForEcho(requestedPwm, false, 579) == requestedPwm);
	}

	puts("Distance policy checks passed: 579/580/581 us, invalid readings, and all 256 PWM requests.");
}
