#include <Arduino.h>

namespace {
  constexpr uint8_t kMotorStandbyPin = 13;

  enum class RobotMode : uint8_t {
    kIdle,
    kManualAssist,
    kSemiAutonomous,
  };

  RobotMode currentMode = RobotMode::kIdle;
}

void setup() {
  Serial.begin(115200);
  pinMode(kMotorStandbyPin, OUTPUT);
  digitalWrite(kMotorStandbyPin, HIGH);
  Serial.println("Semi-autonomous robotics template ready for ESP32 perception and mobility experiments.");
}

void loop() {
  switch (currentMode) {
    case RobotMode::kIdle:
    case RobotMode::kManualAssist:
    case RobotMode::kSemiAutonomous:
      break;
  }

  delay(1000);
}
