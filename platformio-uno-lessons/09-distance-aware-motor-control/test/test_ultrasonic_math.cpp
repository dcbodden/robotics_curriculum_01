// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Iinclude test/test_ultrasonic_math.cpp -o /tmp/lesson09-ultrasonic-check
// /tmp/lesson09-ultrasonic-check
#include "ultrasonic_math.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

void assertNear(float actual, float expected, float tolerance) {
	assert(fabs(actual - expected) <= tolerance);
}

int main() {
	assertNear(roundTripMilliseconds(0), 0.0F, 0.0001F);
	assertNear(distanceCentimeters(0), 0.0F, 0.0001F);

	assertNear(roundTripMilliseconds(580), 0.580F, 0.0001F);
	assertNear(distanceCentimeters(580), 10.0F, 0.001F);
	assertNear(roundTripMilliseconds(2900), 2.9F, 0.0001F);
	assertNear(distanceCentimeters(2900), 50.0F, 0.001F);
	assertNear(roundTripMilliseconds(23200), 23.2F, 0.001F);
	assertNear(distanceCentimeters(23200), 400.0F, 0.001F);

	puts("Ultrasonic checks passed: zero, 10 cm, 50 cm, and 4,000 mm conversions.");
}
