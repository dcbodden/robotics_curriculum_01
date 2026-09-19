#include <Arduino.h>

#include "ultrasonic_math.h"

// HC-SR04 connections: Trigger is an Arduino output; Echo is an input.
const uint8_t ECHO_PIN = 2;
const uint8_t TRIGGER_PIN = 4;

const unsigned long SERIAL_BAUD = 9600;
const unsigned long REPORT_DELAY_MS = 500;
const unsigned int TRIGGER_SETTLE_US = 2;
const unsigned int TRIGGER_PULSE_US = 10;

void startUltrasonicMeasurement() {
	// Start from LOW, then send the sensor's required 10 us HIGH pulse.
	digitalWrite(TRIGGER_PIN, LOW);
	delayMicroseconds(TRIGGER_SETTLE_US);
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(TRIGGER_PULSE_US);
	digitalWrite(TRIGGER_PIN, LOW);
}

void setup() {
	// Set the output latch LOW before enabling D4 as an output.
	digitalWrite(TRIGGER_PIN, LOW);
	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);

	Serial.begin(SERIAL_BAUD);
}

void loop() {
	startUltrasonicMeasurement();

	// pulseIn blocks here until Echo finishes or the explicit 25 ms timeout
	// expires. Zero means that no complete Echo pulse was measured in time.
	const uint32_t echoDurationUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

	if (isValidEchoDuration(echoDurationUs)) {
		Serial.print("Echo delay: ");
		Serial.print(roundTripMilliseconds(echoDurationUs), 3);
		Serial.print(" ms | Distance: ");
		Serial.print(distanceCentimeters(echoDurationUs), 1);
		Serial.println(" cm");
	} else {
		Serial.print("Echo: invalid/no echo within ");
		Serial.print(roundTripMilliseconds(ECHO_TIMEOUT_US), 3);
		Serial.println(" ms");
	}

	// This deliberate foreground wait makes lesson 07 a blocking baseline.
	delay(REPORT_DELAY_MS);
}
