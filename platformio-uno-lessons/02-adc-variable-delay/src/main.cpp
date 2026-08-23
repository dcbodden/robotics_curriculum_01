#include <Arduino.h>

// The digital pin that controls the transistor base.
const int TRANSISTOR_PIN = 8;

// The analog pin that reads the adjustable voltage.
const int ANALOG_INPUT_PIN = A0;

// The smallest and largest values produced by the Uno's 10-bit ADC.
const int ADC_MIN = 0;
const int ADC_MAX = 1023;

// The fastest and slowest delay values, in milliseconds.
const unsigned long DELAY_MIN_MS = 100;
const unsigned long DELAY_MAX_MS = 2000;

void setup() {
	// Tell the Arduino to use pin 8 as an output.
	pinMode(TRANSISTOR_PIN, OUTPUT);

	// Start the serial monitor connection at 9,600 bits per second.
	Serial.begin(9600);
}

void loop() {
	// Read the voltage on A0 as a number from 0 through 1023.
	int adcReading = analogRead(ANALOG_INPUT_PIN);
	// Map that ADC reading to a delay from 100 through 2,000 milliseconds.
	unsigned long stateDelayMs = map(adcReading, ADC_MIN, ADC_MAX, DELAY_MIN_MS, DELAY_MAX_MS);
	Serial.print("ADC reading: ");
	Serial.print(adcReading);
	Serial.print(" | Delay: ");
	Serial.print(stateDelayMs);
	Serial.println(" ms");

	// Turn the transistor ON by setting the pin HIGH (about 5 V).
	digitalWrite(TRANSISTOR_PIN, HIGH);
	delay(stateDelayMs);

	// Read and map A0 again so the next interval uses the latest knob position.
	adcReading = analogRead(ANALOG_INPUT_PIN);
	stateDelayMs = map(adcReading, ADC_MIN, ADC_MAX, DELAY_MIN_MS, DELAY_MAX_MS);
	Serial.print("ADC reading: ");
	Serial.print(adcReading);
	Serial.print(" | Delay: ");
	Serial.print(stateDelayMs);
	Serial.println(" ms");

	// Turn the transistor OFF by setting the pin LOW (0 V).
	digitalWrite(TRANSISTOR_PIN, LOW);
	delay(stateDelayMs);
}
