// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson07-ultrasonic-check
// /tmp/lesson07-ultrasonic-check
#include "ultrasonic_math.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

void assertNear(float actual, float expected, float tolerance) {
	assert(fabs(actual - expected) <= tolerance);
}

int main() {
	assert(!isValidEchoDuration(0));
	assert(isValidEchoDuration(1));
	assert(isValidEchoDuration(HC_SR04_MAX_RANGE_ECHO_US));
	assert(isValidEchoDuration(ECHO_TIMEOUT_US));
	assert(!isValidEchoDuration(ECHO_TIMEOUT_US + 1));

	assertNear(roundTripMilliseconds(580), 0.580F, 0.0001F);
	assertNear(distanceCentimeters(580), 10.0F, 0.001F);
	assertNear(roundTripMilliseconds(HC_SR04_MAX_RANGE_ECHO_US), 23.2F, 0.001F);
	assertNear(distanceCentimeters(HC_SR04_MAX_RANGE_ECHO_US), 400.0F, 0.001F);

	puts("Ultrasonic checks passed: validity, time, 10 cm, and 4,000 mm range.");
}
