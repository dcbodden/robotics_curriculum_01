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
  Serial.println("Send 0 for idle, 1 for manual-assist, or 2 for semi-autonomous mode.");
}

void loop() {
  if (Serial.available() > 0) {
    const char command = static_cast<char>(Serial.read());

    switch (command) {
      case '0':
        currentMode = RobotMode::kIdle;
        break;
      case '1':
        currentMode = RobotMode::kManualAssist;
        break;
      case '2':
        currentMode = RobotMode::kSemiAutonomous;
        break;
      default:
        break;
    }
  }

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
