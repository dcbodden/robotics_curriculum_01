#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "control_mapping.h"
#include "distance_motor_policy.h"
#include "ultrasonic_math.h"

// The joystick's two analog outputs.
const int MOTOR_CONTROL_INPUT_PIN = A0;
const int SERVO_CONTROL_INPUT_PIN = A1;

// D2 receives the ultrasonic Echo signal through external interrupt INT0.
const uint8_t ECHO_PIN = 2;
const uint8_t TRIGGER_PIN = 4;

// D3 controls the protected motor stage; D9 will carry the servo signal.
const int MOTOR_PWM_PIN = 3;
const int SERVO_SIGNAL_PIN = 9;

// Start with motor power off and a middle servo position command.
const uint8_t INITIAL_MOTOR_PWM_COMMAND = 0;
const uint16_t INITIAL_SERVO_PULSE_US = SERVO_CENTER_PULSE_US;

volatile uint8_t requestedMotorPwmCommand = INITIAL_MOTOR_PWM_COMMAND;
volatile uint8_t appliedMotorPwmCommand = INITIAL_MOTOR_PWM_COMMAND;
volatile bool motorSlowdownActive = false;
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
uint8_t ultrasonicFrameDivider = 0;

struct TelemetrySnapshot {
	uint16_t motorAdcReading;
	uint16_t servoAdcReading;
	uint8_t requestedMotorPwmCommand;
	uint8_t appliedMotorPwmCommand;
	bool motorSlowdownActive;
	uint16_t servoPulseCommandUs;
	uint32_t echoDurationUs;
	bool echoMeasurementValid;
};

// Timer1: 16 MHz / 8 = 2 MHz, or two ticks per microsecond.
constexpr uint16_t SERVO_FRAME_US = 20000;
constexpr uint16_t TIMER1_TICKS_PER_US = 2;
constexpr uint16_t TIMER1_TOP = SERVO_FRAME_US * TIMER1_TICKS_PER_US - 1;
constexpr uint8_t CONTROL_FRAMES_PER_ULTRASONIC_TRIGGER = 4;
constexpr uint16_t ULTRASONIC_TRIGGER_PULSE_US = 10;
static_assert(F_CPU == 16000000UL, "This timer configuration requires a 16 MHz Uno");
static_assert(SERVO_MAX_PULSE_US < SERVO_FRAME_US, "Servo pulse must fit its frame");
static_assert(static_cast<uint32_t>(CONTROL_FRAMES_PER_ULTRASONIC_TRIGGER) * SERVO_FRAME_US == 80000UL,
	"Four 20 ms control frames must provide 80 ms Trigger spacing");

uint16_t readSettledJoystickAxis(uint8_t pin) {
	// Discard one conversion after switching the multiplexer, then keep the next.
	// analogRead polls the ADC hardware; it does not need interrupts to complete.
	analogRead(pin);
	return analogRead(pin);
}

void beginEchoAcquisition() {
	// Starting a new attempt does not erase the latest completed policy.
	echoAcquisitionState = ECHO_WAITING_FOR_RISE;
}

void issueAppliedMotorCommand() {
	const uint8_t newAppliedCommand = appliedMotorPwm(
		requestedMotorPwmCommand,
		motorSlowdownActive
	);
	analogWrite(MOTOR_PWM_PIN, newAppliedCommand);
	appliedMotorPwmCommand = newAppliedCommand;
}

void expireIncompleteEchoAcquisition() {
	if (echoAcquisitionState == ECHO_IDLE) return;
	// Invalid/no Echo restores full control of the latest joystick request.
	echoAcquisitionState = ECHO_IDLE;
	latestEchoDurationUs = 0;
	latestEchoMeasurementValid = false;
	motorSlowdownActive = false;
	issueAppliedMotorCommand();
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
	const uint32_t completedEchoDurationUs = echoFallTimestampUs - echoRiseTimestampUs;
	latestEchoDurationUs = completedEchoDurationUs;
	latestEchoMeasurementValid = true;
	motorSlowdownActive = shouldSlowMotorForEcho(true, completedEchoDurationUs);
	// Apply the completed measurement to the latest joystick request immediately.
	issueAppliedMotorCommand();
	echoAcquisitionState = ECHO_IDLE;
}

TelemetrySnapshot takeTelemetrySnapshot() {
	TelemetrySnapshot snapshot;
	// Copy one consistent set, including the multi-byte Echo duration.
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		snapshot.motorAdcReading = latestMotorAdcReading;
		snapshot.servoAdcReading = latestServoAdcReading;
		snapshot.requestedMotorPwmCommand = requestedMotorPwmCommand;
		snapshot.appliedMotorPwmCommand = appliedMotorPwmCommand;
		snapshot.motorSlowdownActive = motorSlowdownActive;
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
	// Expire the previous ping first, but never skip this frame's control work.
	expireIncompleteEchoAcquisition();

	// One check of both controls each 20 ms frame. No serial output occurs here.
	const uint16_t motorReading = readSettledJoystickAxis(MOTOR_CONTROL_INPUT_PIN);
	const uint16_t servoReading = readSettledJoystickAxis(SERVO_CONTROL_INPUT_PIN);
	const uint8_t newRequestedMotorCommand = motorPwmFromAdc(motorReading);
	const uint16_t newServoCommand = servoPulseFromAdc(servoReading);
	requestedMotorPwmCommand = newRequestedMotorCommand;
	// The latest completed Echo policy persists while the next ping is in progress.
	issueAppliedMotorCommand();
	// The current pulse is already underway; this buffered write applies next frame.
	OCR1A = newServoCommand * TIMER1_TICKS_PER_US;
	servoPulseCommandUs = newServoCommand;
	latestMotorAdcReading = motorReading;
	latestServoAdcReading = servoReading;

	// Start a ping every fourth frame: 4 * 20 ms = 80 ms between Triggers.
	ultrasonicFrameDivider++;
	if (ultrasonicFrameDivider == CONTROL_FRAMES_PER_ULTRASONIC_TRIGGER) {
		ultrasonicFrameDivider = 0;
		beginEchoAcquisition();
		digitalWrite(TRIGGER_PIN, HIGH);
		delayMicroseconds(ULTRASONIC_TRIGGER_PULSE_US);
		// Keep this LOW write as the scheduled frame's final operation.
		digitalWrite(TRIGGER_PIN, LOW);
	}
}

const unsigned long SERIAL_BAUD = 9600;
const unsigned long REPORT_DELAY_MS = 500;

void setup() {
	// Read joystick voltages without enabling input pullups.
	pinMode(MOTOR_CONTROL_INPUT_PIN, INPUT);
	pinMode(SERVO_CONTROL_INPUT_PIN, INPUT);
	pinMode(ECHO_PIN, INPUT);

	// Hold Trigger LOW between the scheduled 10 us pulses.
	digitalWrite(TRIGGER_PIN, LOW);
	pinMode(TRIGGER_PIN, OUTPUT);

	// Keep the motor off before any control checks are enabled.
	digitalWrite(MOTOR_PWM_PIN, LOW);
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	analogWrite(MOTOR_PWM_PIN, appliedMotorPwmCommand);

	// Prepare D9 before Timer1 takes ownership of its servo pulse signal.
	digitalWrite(SERVO_SIGNAL_PIN, LOW);
	pinMode(SERVO_SIGNAL_PIN, OUTPUT);

	Serial.begin(SERIAL_BAUD);
	attachInterrupt(digitalPinToInterrupt(ECHO_PIN), handleEchoChange, CHANGE);
	configureTimer1ForServoAndControl();
}

void loop() {
	// Copy shared ISR data atomically, then convert and print with interrupts on.
	const TelemetrySnapshot snapshot = takeTelemetrySnapshot();

	if (snapshot.echoMeasurementValid) {
		Serial.print("Echo: valid | Echo delay: ");
		Serial.print(roundTripMilliseconds(snapshot.echoDurationUs), 3);
		Serial.print(" ms | Distance: ");
		Serial.print(distanceCentimeters(snapshot.echoDurationUs), 1);
		Serial.print(" cm");
	} else {
		Serial.print("Echo: invalid/no echo");
	}

	Serial.print(" | Motor ADC: ");
	Serial.print(snapshot.motorAdcReading);
	Serial.print(" | Requested motor PWM: ");
	Serial.print(snapshot.requestedMotorPwmCommand);
	Serial.print(" | Applied motor PWM: ");
	Serial.print(snapshot.appliedMotorPwmCommand);
	Serial.print(" | Slowdown: ");
	Serial.print(snapshot.motorSlowdownActive ? "ON (50%)" : "OFF (full control)");
	Serial.print(" | Servo ADC: ");
	Serial.print(snapshot.servoAdcReading);
	Serial.print(" | Servo pulse command (us): ");
	Serial.println(snapshot.servoPulseCommandUs);

	// Timer1 and INT0 keep handling control frames and Echo edges during this wait.
	delay(REPORT_DELAY_MS);
}
