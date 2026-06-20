#pragma once

void connectMQTT();
void mqttLoop();
void mqttSend(const char* topic, const char* message);
bool isMQTTConnected();
