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
      Serial.println("Idle mode: base platform powered and awaiting commands.");
      break;
    case RobotMode::kManualAssist:
      Serial.println("Manual-assist mode: mobility base active for supervised driving.");
      break;
    case RobotMode::kSemiAutonomous:
      Serial.println("Semi-autonomous mode: ready for OV7670-guided robotics experiments.");
      break;
  }

  delay(1000);
}
