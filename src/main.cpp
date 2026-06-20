#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"

unsigned long lastMsg = 0;

void setup() {
  Serial.begin(115200);

  // Pins initialisieren
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Pumpe AUS

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);     // LED AUS

  connectWiFi();
  connectMQTT();
}

void loop() {
  mqttLoop();

  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    mqttSend(TOPIC_STATUS, "ESP32 laeuft!");
    String uptime = String(millis() / 1000) + " Sekunden";
    mqttSend(TOPIC_UPTIME, uptime.c_str());
  }
}
