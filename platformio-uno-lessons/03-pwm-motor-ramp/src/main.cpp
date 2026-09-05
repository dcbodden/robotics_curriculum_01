#include <Arduino.h>

// The PWM-capable pin that controls the MOSFET gate.
const int MOTOR_PWM_PIN = 3;

// Arduino PWM commands range from fully off (0) to fully on (255).
const int PWM_MIN = 0;
const int PWM_MAX = 255;

// Holding each of the 256 values for 16 ms makes each ramp take about 4.1 seconds.
const unsigned long RAMP_STEP_DELAY_MS = 16;

void setup() {
	// Start with the motor command off.
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	analogWrite(MOTOR_PWM_PIN, PWM_MIN);
}

void loop() {
	// Increase the motor command smoothly from 0 to 255.
	for (int pwmValue = PWM_MIN; pwmValue <= PWM_MAX; pwmValue++) {
		analogWrite(MOTOR_PWM_PIN, pwmValue);
		delay(RAMP_STEP_DELAY_MS);
	}

	// Decrease the motor command smoothly from 255 to 0.
	for (int pwmValue = PWM_MAX; pwmValue >= PWM_MIN; pwmValue--) {
		analogWrite(MOTOR_PWM_PIN, pwmValue);
		delay(RAMP_STEP_DELAY_MS);
	}
}
