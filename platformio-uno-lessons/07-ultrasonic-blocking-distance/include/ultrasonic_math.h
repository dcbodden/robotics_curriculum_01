#pragma once

#include <stdint.h>

// The HC-SR04 data sheet describes a 4 m maximum range. At the commonly
// documented conversion of 58 microseconds per centimeter, that is 23,200 us.
constexpr uint32_t HC_SR04_MAX_RANGE_ECHO_US = 23200UL;

// Allow a little margin beyond the advertised maximum without accepting the
// Arduino pulseIn() default wait of one second.
constexpr uint32_t ECHO_TIMEOUT_US = 25000UL;
constexpr float MICROSECONDS_PER_MILLISECOND = 1000.0F;
constexpr float ECHO_MICROSECONDS_PER_CENTIMETER = 58.0F;

inline bool isValidEchoDuration(uint32_t echoDurationUs) {
	return echoDurationUs > 0 && echoDurationUs <= ECHO_TIMEOUT_US;
}

inline float roundTripMilliseconds(uint32_t echoDurationUs) {
	return echoDurationUs / MICROSECONDS_PER_MILLISECOND;
}

inline float distanceCentimeters(uint32_t echoDurationUs) {
	return echoDurationUs / ECHO_MICROSECONDS_PER_CENTIMETER;
}
