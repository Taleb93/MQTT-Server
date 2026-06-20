#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=========================");
  Serial.println("  ESP32 WROOM 32 - OK!  ");
  Serial.println("=========================");
}

void loop() {
  Serial.println("ESP32 is running...");
  delay(2000);
}
