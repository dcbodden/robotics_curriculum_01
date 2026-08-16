#include <Arduino.h>

namespace {
  constexpr uint8_t kMotorStandbyPin = 13;
  constexpr uint8_t kMotorLeftForwardPin = 26;
  constexpr uint8_t kMotorLeftReversePin = 27;
  constexpr uint8_t kMotorRightForwardPin = 14;
  constexpr uint8_t kMotorRightReversePin = 25;
  constexpr unsigned long kLoopDelayMs = 1000;
}  // namespace

void setup() {
  Serial.begin(115200);

  pinMode(kMotorStandbyPin, OUTPUT);
  pinMode(kMotorLeftForwardPin, OUTPUT);
  pinMode(kMotorLeftReversePin, OUTPUT);
  pinMode(kMotorRightForwardPin, OUTPUT);
  pinMode(kMotorRightReversePin, OUTPUT);

  digitalWrite(kMotorStandbyPin, HIGH);
  digitalWrite(kMotorLeftForwardPin, LOW);
  digitalWrite(kMotorLeftReversePin, LOW);
  digitalWrite(kMotorRightForwardPin, LOW);
  digitalWrite(kMotorRightReversePin, LOW);

  Serial.println("FPV Wi-Fi RC car template ready for OV7670 and DRV8833 integration.");
}

void loop() {
  // Placeholder pacing for the starter template until Wi-Fi, camera, and drive logic are added.
  delay(kLoopDelayMs);
}
