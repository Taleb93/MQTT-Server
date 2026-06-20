#include "mqtt_manager.h"
#include "config.h"
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure espClient;
PubSubClient client(espClient);

// ============================================
// Nachricht empfangen
// ============================================
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Empfangen [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  if (message == "ON") {
    Serial.println(">>> PUMPE AN!");
    digitalWrite(RELAY_PIN, LOW);
  }
  if (message == "OFF") {
    Serial.println(">>> PUMPE AUS!");
    digitalWrite(RELAY_PIN, HIGH);
  }
}

// ============================================
// MQTT Verbinden
// ============================================
void connectMQTT() {
  espClient.setInsecure();
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("MQTT verbinden...");

    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("MQTT verbunden! ✅");
      client.subscribe(TOPIC_COMMAND);
      client.publish(TOPIC_STATUS, "ESP32 ist online!");
    } else {
      Serial.print("Fehler: ");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

// ============================================
// Senden
// ============================================
void mqttSend(const char* topic, const char* message) {
  client.publish(topic, message);
  Serial.print("Gesendet [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);
}

// ============================================
// Loop
// ============================================
void mqttLoop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();
}

bool isMQTTConnected() {
  return client.connected();
}
