#include <Arduino.h>
#include "control_mapping.h"

// The joystick's two analog outputs.
const int MOTOR_CONTROL_INPUT_PIN = A0;
const int SERVO_CONTROL_INPUT_PIN = A1;

// D3 controls the protected motor stage; D9 will carry the servo signal.
const int MOTOR_PWM_PIN = 3;
const int SERVO_SIGNAL_PIN = 9;

// Start with motor power off and a middle servo position command.
const uint8_t INITIAL_MOTOR_PWM_COMMAND = 0;
const uint16_t INITIAL_SERVO_PULSE_US = SERVO_CENTER_PULSE_US;

uint8_t motorPwmCommand = INITIAL_MOTOR_PWM_COMMAND;
uint16_t servoPulseCommandUs = INITIAL_SERVO_PULSE_US;

const unsigned long SERIAL_BAUD = 9600;

void setup() {
	// Read joystick voltages without enabling input pullups.
	pinMode(MOTOR_CONTROL_INPUT_PIN, INPUT);
	pinMode(SERVO_CONTROL_INPUT_PIN, INPUT);

	// Keep the motor off before any control checks are enabled.
	digitalWrite(MOTOR_PWM_PIN, LOW);
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	analogWrite(MOTOR_PWM_PIN, motorPwmCommand);

	// Hold the signal low until Timer1 pulse generation is added in task 1.3.
	// The middle pulse-width command is stored above; analogWrite() is not
	// a servo position signal.
	digitalWrite(SERVO_SIGNAL_PIN, LOW);
	pinMode(SERVO_SIGNAL_PIN, OUTPUT);

	Serial.begin(SERIAL_BAUD);
}

void loop() {
	// Task 1.3 will call the mapping functions from the control ISR.
	// Interrupt control and reporting follow in tasks 1.3–1.4.
}
