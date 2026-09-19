#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "control_mapping.h"

// The joystick's two analog outputs.
const int MOTOR_CONTROL_INPUT_PIN = A0;
const int SERVO_CONTROL_INPUT_PIN = A1;

// D2 receives the ultrasonic Echo signal through external interrupt INT0.
const uint8_t ECHO_PIN = 2;

// D3 controls the protected motor stage; D9 will carry the servo signal.
const int MOTOR_PWM_PIN = 3;
const int SERVO_SIGNAL_PIN = 9;

// Start with motor power off and a middle servo position command.
const uint8_t INITIAL_MOTOR_PWM_COMMAND = 0;
const uint16_t INITIAL_SERVO_PULSE_US = SERVO_CENTER_PULSE_US;

volatile uint8_t motorPwmCommand = INITIAL_MOTOR_PWM_COMMAND;
volatile uint16_t servoPulseCommandUs = INITIAL_SERVO_PULSE_US;
volatile uint16_t latestMotorAdcReading = MOTOR_NEUTRAL_ADC;
volatile uint16_t latestServoAdcReading = SERVO_CENTER_ADC;

enum EchoAcquisitionState : uint8_t {
	ECHO_IDLE,
	ECHO_WAITING_FOR_RISE,
	ECHO_WAITING_FOR_FALL
};

volatile EchoAcquisitionState echoAcquisitionState = ECHO_IDLE;
volatile uint32_t echoRiseTimestampUs = 0;
volatile uint32_t latestEchoDurationUs = 0;
volatile bool latestEchoMeasurementValid = false;

struct TelemetrySnapshot {
	uint16_t motorAdcReading;
	uint16_t servoAdcReading;
	uint8_t motorPwmCommand;
	uint16_t servoPulseCommandUs;
	uint32_t echoDurationUs;
	bool echoMeasurementValid;
};

// Timer1: 16 MHz / 8 = 2 MHz, or two ticks per microsecond.
constexpr uint16_t SERVO_FRAME_US = 20000;
constexpr uint16_t TIMER1_TICKS_PER_US = 2;
constexpr uint16_t TIMER1_TOP = SERVO_FRAME_US * TIMER1_TICKS_PER_US - 1;
static_assert(F_CPU == 16000000UL, "This timer configuration requires a 16 MHz Uno");
static_assert(SERVO_MAX_PULSE_US < SERVO_FRAME_US, "Servo pulse must fit its frame");

uint16_t readSettledJoystickAxis(uint8_t pin) {
	// Discard one conversion after switching the multiplexer, then keep the next.
	// analogRead polls the ADC hardware; it does not need interrupts to complete.
	analogRead(pin);
	return analogRead(pin);
}

void beginEchoAcquisition() {
	// The Timer1 scheduler will call this immediately before a Trigger pulse.
	echoAcquisitionState = ECHO_WAITING_FOR_RISE;
}

void handleEchoChange() {
	const bool echoIsHigh = digitalRead(ECHO_PIN) == HIGH;

	if (echoIsHigh) {
		// Ignore rising edges unless a scheduled measurement is waiting for one.
		if (echoAcquisitionState != ECHO_WAITING_FOR_RISE) return;
		echoRiseTimestampUs = micros();
		echoAcquisitionState = ECHO_WAITING_FOR_FALL;
		return;
	}

	// Ignore falling edges that do not complete the recognized rising edge.
	if (echoAcquisitionState != ECHO_WAITING_FOR_FALL) return;
	const uint32_t echoFallTimestampUs = micros();
	// Unsigned subtraction remains correct if micros() rolls over between edges.
	latestEchoDurationUs = echoFallTimestampUs - echoRiseTimestampUs;
	latestEchoMeasurementValid = true;
	echoAcquisitionState = ECHO_IDLE;
}

TelemetrySnapshot takeTelemetrySnapshot() {
	TelemetrySnapshot snapshot;
	// Copy one consistent set, including the multi-byte Echo duration.
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		snapshot.motorAdcReading = latestMotorAdcReading;
		snapshot.servoAdcReading = latestServoAdcReading;
		snapshot.motorPwmCommand = motorPwmCommand;
		snapshot.servoPulseCommandUs = servoPulseCommandUs;
		snapshot.echoDurationUs = latestEchoDurationUs;
		snapshot.echoMeasurementValid = latestEchoMeasurementValid;
	}
	return snapshot;
}

void configureTimer1ForServoAndControl() {
	const uint8_t savedStatus = SREG;
	cli();
	// Stop Timer1 and select normal mode so the initial compare is unbuffered.
	TCCR1B = 0;
	TCCR1A = 0;
	TIMSK1 = 0;
	ICR1 = TIMER1_TOP;
	OCR1A = INITIAL_SERVO_PULSE_US * TIMER1_TICKS_PER_US;
	TCNT1 = TIMER1_TOP;
	TIFR1 = _BV(ICF1) | _BV(OCF1A) | _BV(OCF1B) | _BV(TOV1);

	// Mode 14: fixed ICR1 TOP, non-inverting OC1A (D9), clock divided by 8.
	// Timer0 timing and Timer2 motor PWM keep their Arduino configuration.
	TCCR1A = _BV(COM1A1) | _BV(WGM11);
	TIMSK1 = _BV(TOIE1);
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
	SREG = savedStatus;
}

ISR(TIMER1_OVF_vect) {
	// One check of both controls each 20 ms frame. No serial or pulse waits.
	const uint16_t motorReading = readSettledJoystickAxis(MOTOR_CONTROL_INPUT_PIN);
	const uint16_t servoReading = readSettledJoystickAxis(SERVO_CONTROL_INPUT_PIN);
	const uint8_t newMotorCommand = motorPwmFromAdc(motorReading);
	const uint16_t newServoCommand = servoPulseFromAdc(servoReading);
	analogWrite(MOTOR_PWM_PIN, newMotorCommand);
	// The current pulse is already underway; this buffered write applies next frame.
	OCR1A = newServoCommand * TIMER1_TICKS_PER_US;
	motorPwmCommand = newMotorCommand;
	servoPulseCommandUs = newServoCommand;
	latestMotorAdcReading = motorReading;
	latestServoAdcReading = servoReading;
}

const unsigned long SERIAL_BAUD = 9600;
const unsigned long REPORT_DELAY_MS = 1000;

void setup() {
	// Read joystick voltages without enabling input pullups.
	pinMode(MOTOR_CONTROL_INPUT_PIN, INPUT);
	pinMode(SERVO_CONTROL_INPUT_PIN, INPUT);
	pinMode(ECHO_PIN, INPUT);

	// Keep the motor off before any control checks are enabled.
	digitalWrite(MOTOR_PWM_PIN, LOW);
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	analogWrite(MOTOR_PWM_PIN, motorPwmCommand);

	// Prepare D9 before Timer1 takes ownership of its servo pulse signal.
	digitalWrite(SERVO_SIGNAL_PIN, LOW);
	pinMode(SERVO_SIGNAL_PIN, OUTPUT);

	Serial.begin(SERIAL_BAUD);
	attachInterrupt(digitalPinToInterrupt(ECHO_PIN), handleEchoChange, CHANGE);
	configureTimer1ForServoAndControl();
}

void loop() {
	// Echo fields join the atomic snapshot now; task 2.4 will report them.
	const TelemetrySnapshot snapshot = takeTelemetrySnapshot();

	Serial.print("Motor ADC: ");
	Serial.print(snapshot.motorAdcReading);
	Serial.print(" | Motor PWM command: ");
	Serial.print(snapshot.motorPwmCommand);
	Serial.print(" | Servo ADC: ");
	Serial.print(snapshot.servoAdcReading);
	Serial.print(" | Servo pulse command (us): ");
	Serial.println(snapshot.servoPulseCommandUs);

	// Reports are snapshots, not a printout of every 20 ms control update.
	delay(REPORT_DELAY_MS);
}
