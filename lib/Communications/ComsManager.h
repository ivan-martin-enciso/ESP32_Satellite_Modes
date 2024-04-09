#ifndef COMSMANAGER_H
#define COMSMANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <RadioLib.h>
#include <esp_wpa2.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "Strings.h"

// ----- Housekeeping -----
extern volatile bool sendTelemetry;
extern volatile int wifiRSSI;

// ----- LoRa -----
static const int loraFrequency = 868.0, loraCSPin = 38, loraRSTPin = 48, loraIrqPin = 47;
extern volatile bool receivedFlag; 

class ComsManager {
public:
    ComsManager();
    ~ComsManager();
    void initializeComs(bool isEduroam = false);
    void sendTelemetryData(String telemetryJson);
    void getWiFiRSSI();
    float getLoRaRSSI();
    float getLoRaSNR();
    void handleWifi();
};

extern ComsManager comsManager;

#endif
