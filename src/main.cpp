#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"

unsigned long lastMsg = 0;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  connectWiFi();
  connectMQTT();
}

void loop() {
  mqttLoop();

  // Alle 5 Sekunden Status senden
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    mqttSend(TOPIC_STATUS, "ESP32 laeuft!");
  }
}
