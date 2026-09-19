#pragma once

#include <stdint.h>

constexpr float MICROSECONDS_PER_MILLISECOND = 1000.0F;
constexpr float ECHO_MICROSECONDS_PER_CENTIMETER = 58.0F;

inline float roundTripMilliseconds(uint32_t echoDurationUs) {
	return echoDurationUs / MICROSECONDS_PER_MILLISECOND;
}

inline float distanceCentimeters(uint32_t echoDurationUs) {
	return echoDurationUs / ECHO_MICROSECONDS_PER_CENTIMETER;
}
