#include <Arduino.h>

// The digital pin that controls the transistor base.
const int TRANSISTOR_PIN = 8;

// How long to hold each output state (milliseconds).
const unsigned long STATE_DURATION_MS = 2000;

void setup() {
  // Tell the Arduino to use pin 8 as an output.
  pinMode(TRANSISTOR_PIN, OUTPUT);
}

void loop() {
  // Turn the transistor ON by setting the pin HIGH (about 5 V).
  digitalWrite(TRANSISTOR_PIN, HIGH);
  delay(STATE_DURATION_MS);  // hold HIGH for two seconds

  // Turn the transistor OFF by setting the pin LOW (0 V).
  digitalWrite(TRANSISTOR_PIN, LOW);
  delay(STATE_DURATION_MS);  // hold LOW for two seconds
}
