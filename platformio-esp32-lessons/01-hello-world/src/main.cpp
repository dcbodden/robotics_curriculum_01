#include <Arduino.h>

const unsigned long SERIAL_SPEED = 115200;
const unsigned long LED_ON_TIME_MS = 250;
const unsigned long LED_OFF_TIME_MS = 250;
const unsigned long TIME_BETWEEN_GROUPS_MS = 1000;

void blinkFibonacciGroup(int blinkCount) {
  Serial.print("Blinking Fibonacci ");
  Serial.println(blinkCount);

  for (int blinkNumber = 0; blinkNumber < blinkCount; blinkNumber++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(LED_ON_TIME_MS);
    digitalWrite(LED_BUILTIN, LOW);
    delay(LED_OFF_TIME_MS);
  }

  delay(TIME_BETWEEN_GROUPS_MS);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(SERIAL_SPEED);
}

void loop() {
  blinkFibonacciGroup(1);
  blinkFibonacciGroup(1);
  blinkFibonacciGroup(2);
  blinkFibonacciGroup(3);
  blinkFibonacciGroup(5);
}
