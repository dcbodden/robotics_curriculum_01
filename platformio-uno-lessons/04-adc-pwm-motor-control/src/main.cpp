#include <Arduino.h>

// The PWM-capable pin that controls the MOSFET gate.
const int MOTOR_PWM_PIN = 3;

// The analog pin that reads the RV wiper voltage.
const int ANALOG_INPUT_PIN = A0;

// The input and output ranges used by the direct mapping.
const int ADC_MIN = 0;
const int ADC_MAX = 1023;
const int PWM_MIN = 0;
const int PWM_MAX = 255;

// Pause between control updates so the serial output is easy to read.
const unsigned long CONTROL_UPDATE_DELAY_MS = 500;
const unsigned long SERIAL_BAUD = 9600;

void setup() {
	// Start with the motor command off.
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	analogWrite(MOTOR_PWM_PIN, PWM_MIN);

	// Start the serial monitor connection at 9,600 bits per second.
	Serial.begin(SERIAL_BAUD);
}

void loop() {
	// Read A0 as a number from 0 through 1023.
	const int adcReading = analogRead(ANALOG_INPUT_PIN);

	// Map the ADC reading directly to a PWM command from 0 through 255.
	const int pwmCommand = map(adcReading, ADC_MIN, ADC_MAX, PWM_MIN, PWM_MAX);
	analogWrite(MOTOR_PWM_PIN, pwmCommand);

	// Report the input reading and the output command together.
	Serial.print("ADC reading: ");
	Serial.print(adcReading);
	Serial.print(" | PWM command: ");
	Serial.println(pwmCommand);

	// Intentionally wait before reading and updating the motor again.
	delay(CONTROL_UPDATE_DELAY_MS);
}
