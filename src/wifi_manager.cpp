#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>
#include <Arduino.h>

void connectWiFi() {
  Serial.print("WiFi verbinden: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi verbunden! ✅");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}
