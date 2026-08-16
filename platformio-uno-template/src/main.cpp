#include <Arduino.h>

namespace {
constexpr uint8_t kLedPin = LED_BUILTIN;
constexpr unsigned long kBlinkDelayMs = 500;
}

void setup() {
  pinMode(kLedPin, OUTPUT);
}

void loop() {
  digitalWrite(kLedPin, HIGH);
  delay(kBlinkDelayMs);
  digitalWrite(kLedPin, LOW);
  delay(kBlinkDelayMs);
}
