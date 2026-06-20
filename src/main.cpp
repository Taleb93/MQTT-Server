#include <Arduino.h>
#include <WiFi.h> 
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

  if (millis() - lastMsg > 5000) {
    lastMsg = millis();

    // Status senden
    mqttSend(TOPIC_STATUS, "ESP32 laeuft!");

    // IP senden
    mqttSend(TOPIC_IP, WiFi.localIP().toString().c_str());

    // WiFi Signal senden
    String rssi = String(WiFi.RSSI()) + " dBm";
    mqttSend(TOPIC_RSSI, rssi.c_str());

    // Uptime senden
    String uptime = String(millis() / 1000) + " Sekunden";
    mqttSend(TOPIC_UPTIME, uptime.c_str());
  }
}
