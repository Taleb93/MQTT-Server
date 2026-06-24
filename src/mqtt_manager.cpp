#include "mqtt_manager.h"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long previousMillis = 0;
bool blinking = false;
bool ledState = false;
// ============================================
// Nachricht empfangen
// ============================================
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("================================");
  Serial.print("Topic:   ");
  Serial.println(topic);
  Serial.print("Nachricht: ");
  Serial.println(message);
  Serial.println("================================");

  // ============================================
  // LED Steuerung
  // ============================================
  if (String(topic) == "home/led/command") {
            if (message == "blink") {
        blinking = true;
        mqttSend("home/led/status", "LED ist Blinking");
        }
        if (message == "HIGH") {
        blinking = false;  // ← Blink stoppen!
        digitalWrite(LED_PIN, HIGH);
        mqttSend("home/led/status", "LED ist AN");
        }
        if (message == "LOW") {
        blinking = false;  // ← Blink stoppen!
        digitalWrite(LED_PIN, LOW);
        mqttSend("home/led/status", "LED ist AUS");
        }

  }

  // ============================================
  // Pumpe Steuerung
  // ============================================
  if (String(topic) == "home/pump/command") {
    if (message == "ON") {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println(">>> PUMPE AN! 💧");
      mqttSend("home/pump/status", "Pumpe ist AN");
    }
    if (message == "OFF") {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println(">>> PUMPE AUS!");
      mqttSend("home/pump/status", "Pumpe ist AUS");
    }
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

      // Topics abonnieren
      client.subscribe("home/led/command");
      client.subscribe("home/pump/command");
      Serial.println("Abonniert: home/led/command ✅");
      Serial.println("Abonniert: home/pump/command ✅");

      // Online Nachricht
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
