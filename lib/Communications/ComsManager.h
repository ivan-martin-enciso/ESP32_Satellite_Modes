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

// ----- Telemetry & Telecommand-----
extern volatile bool sendTelemetry;
extern String receivedPayloadWiFi;
extern String receivedPayloadLoRa;  
extern volatile bool receivedLora; 
extern volatile bool receivedWifi; 


class ComsManager {
public:
    ComsManager();
    ~ComsManager();
    void initializeComs();
    void sendTelemetryData(JsonDocument telemetryJson);
    float getWiFiRSSI();
    float getLoRaRSSI();
    float getLoRaSNR();
    float getLoRaFreqError();
    void setLoRaRSSI();
    void setLoRaSNR();
    void setLoRaFreqError();
    void serialWifiComunication();
    void receivePackageUsingLora();
private:
    float loraRSSI = 0;
    float loraSNR = 0;
    float loraFrequencyError = 0;
};

extern ComsManager comsManager;

#endif
