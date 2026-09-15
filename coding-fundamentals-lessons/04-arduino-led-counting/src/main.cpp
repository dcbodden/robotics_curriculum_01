#include <Arduino.h>

const unsigned long SERIAL_SPEED = 9600;
const int FIRST_GROUP = 1;
const int LAST_GROUP = 5;
const unsigned long LED_ON_TIME_MS = 250;
const unsigned long TIME_BETWEEN_BLINKS_MS = 250;
const unsigned long TIME_BETWEEN_GROUPS_MS = 1000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(SERIAL_SPEED);
}

void loop() {
  // The outer loop chooses a group from 1 through 5.
  for (int groupNumber = FIRST_GROUP; groupNumber <= LAST_GROUP;
       groupNumber++) {
    Serial.print("Blink group: ");
    Serial.println(groupNumber);

    // The inner loop flashes the LED once for each number in the group.
    for (int blinkNumber = 1; blinkNumber <= groupNumber; blinkNumber++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(LED_ON_TIME_MS);
      digitalWrite(LED_BUILTIN, LOW);
      delay(TIME_BETWEEN_BLINKS_MS);
    }

    // This longer pause makes the end of each group easy to see.
    delay(TIME_BETWEEN_GROUPS_MS);
  }
}
