#include <Arduino.h>
#include <avr/interrupt.h>

// Timer1 receives 250,000 clock ticks per second after the 64:1 prescaler.
// Counting from 0 through 24,999 therefore produces one interrupt every 100 ms.
const uint16_t TIMER1_COMPARE_MATCH = 24999;

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
	// The ADC reading and motor-command update will run here.
}

void setup() {
	configureTimer1For100MillisecondInterrupt();
}

void loop() {
}
