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
#include "Strings.h"
#include "arduino_secrets.h"
//#include "my_secrets.h"

// ----- Telemetry & Telecommand-----
extern volatile bool sendTelemetry;
extern String receivedPayload; 
extern volatile bool receivedFlag; 

class ComsManager {
public:
    ComsManager();
    ~ComsManager();
    void initializeComs(bool isEduroam = false);
    void sendTelemetryData(String telemetryJson);
    int getWiFiRSSI(bool isEduroam = false);
    float getLoRaRSSI();
    float getLoRaSNR();
    void serialWifiComunication();
    String receivePackageUsignLora();
};

extern ComsManager comsManager;

#endif
