// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_control_mapping.cpp -o /tmp/lesson06-mapping-check
// /tmp/lesson06-mapping-check
#include "control_mapping.h"
#include <assert.h>
#include <stdio.h>

int main() {
	assert(motorPwmFromAdc(0) == 0);
	assert(motorPwmFromAdc(472) == 0);
	assert(motorPwmFromAdc(512) == 0);
	assert(motorPwmFromAdc(552) == 0);
	// Integer PWM quantization leaves the first active count at zero.
	assert(motorPwmFromAdc(553) == 0);
	assert(motorPwmFromAdc(554) == 1);
	assert(motorPwmFromAdc(789) == 128);
	assert(motorPwmFromAdc(1023) == 255);
	assert(motorPwmFromAdc(-100) == 0);
	assert(motorPwmFromAdc(2000) == 255);

	assert(servoPulseFromAdc(0) == 1000);
	assert(servoPulseFromAdc(256) == 1250);
	assert(servoPulseFromAdc(511) == 1499);
	assert(servoPulseFromAdc(512) == 1500);
	assert(servoPulseFromAdc(513) == 1500);
	assert(servoPulseFromAdc(768) == 1750);
	assert(servoPulseFromAdc(1023) == 2000);
	assert(servoPulseFromAdc(-100) == 1000);
	assert(servoPulseFromAdc(2000) == 2000);

	int lastMotor = 0;
	int lastServo = 1000;
	for (int adc = 0; adc <= 1023; ++adc) {
		const int motor = motorPwmFromAdc(adc);
		const int servo = servoPulseFromAdc(adc);
		assert(motor >= lastMotor && motor <= 255);
		assert(servo >= lastServo && servo >= 1000 && servo <= 2000);
		if (adc <= 552) assert(motor == 0);
		lastMotor = motor;
		lastServo = servo;
	}
	puts("Mapping checks passed: neutral, boundaries, endpoints, clamps, and all 1024 ADC values.");
}
