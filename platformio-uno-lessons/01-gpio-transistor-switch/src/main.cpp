#include <Arduino.h>

namespace {
  constexpr uint8_t kControlPin = 8;
  constexpr unsigned long kBlinkDelayMs = 500;
}  // namespace

void setup() {
  pinMode(kControlPin, OUTPUT);
}

void loop() {
  digitalWrite(kControlPin, HIGH);
  delay(kBlinkDelayMs);
  digitalWrite(kControlPin, LOW);
  delay(kBlinkDelayMs);
}
