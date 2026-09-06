#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

// The PWM-capable pin that controls the MOSFET gate.
const int MOTOR_PWM_PIN = 3;

// The analog pin that reads the RV wiper voltage.
const int ANALOG_INPUT_PIN = A0;

// The input and output ranges used by the direct mapping.
const int ADC_MIN = 0;
const int ADC_MAX = 1023;
const int PWM_MIN = 0;
const int PWM_MAX = 255;

// The foreground reports the latest values once per second.
const unsigned long REPORT_DELAY_MS = 1000;
const unsigned long SERIAL_BAUD = 9600;

// Timer1 receives 250,000 clock ticks per second after the 64:1 prescaler.
// Counting from 0 through 24,999 therefore produces one interrupt every 100 ms.
const uint16_t TIMER1_COMPARE_MATCH = 24999;

// These values are shared between the interrupt and the main program.
volatile uint16_t latestAdcReading = ADC_MIN;
volatile uint8_t latestPwmCommand = PWM_MIN;

void configureTimer1For100MillisecondInterrupt() {
	// Pause interrupts while changing the Timer1 control registers.
	noInterrupts();

	// Reset Timer1 without changing Timer0 (Arduino timing) or Timer2 (D3 PWM).
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	OCR1A = TIMER1_COMPARE_MATCH;

	// Clear Timer1 on each compare match and use a 64:1 clock prescaler.
	TCCR1B |= _BV(WGM12);
	TCCR1B |= _BV(CS11) | _BV(CS10);

	// Enable the Timer1 compare-A interrupt, then resume interrupts.
	TIMSK1 |= _BV(OCIE1A);
	interrupts();
}

ISR(TIMER1_COMPA_vect) {
	// Read the RV and map its 0–1023 ADC value to a 0–255 PWM command.
	const uint16_t adcReading = analogRead(ANALOG_INPUT_PIN);
	const uint8_t pwmCommand = map(adcReading, ADC_MIN, ADC_MAX, PWM_MIN, PWM_MAX);

	// Apply the new command, then publish both values for the main program.
	analogWrite(MOTOR_PWM_PIN, pwmCommand);
	latestAdcReading = adcReading;
	latestPwmCommand = pwmCommand;
}

void setup() {
	// Keep the motor command off until the first timed ADC reading.
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	analogWrite(MOTOR_PWM_PIN, PWM_MIN);

	Serial.begin(SERIAL_BAUD);
	configureTimer1For100MillisecondInterrupt();
}

void loop() {
	uint16_t adcSnapshot;
	uint8_t pwmSnapshot;

	// Copy one consistent pair, then restore interrupts before serial output.
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		adcSnapshot = latestAdcReading;
		pwmSnapshot = latestPwmCommand;
	}

	Serial.print("ADC reading: ");
	Serial.print(adcSnapshot);
	Serial.print(" | PWM command: ");
	Serial.println(pwmSnapshot);

	// Timer1 interrupts keep updating the motor while the foreground waits.
	delay(REPORT_DELAY_MS);
}
