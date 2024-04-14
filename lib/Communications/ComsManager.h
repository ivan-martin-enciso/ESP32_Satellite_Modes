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

/**
 * @file ComsManager.h
 * @brief Header class for handling Communications. 
 * @author Ivan Martin Enciso 
 */

extern volatile bool sendTelemetry; ///< Flag indicating if telemetry data should be sent. 
extern String receivedPayloadWiFi;  ///< String where information over WiFi is received.
extern String receivedPayloadLoRa;  ///< String where information over LoRa is received.
extern volatile bool receivedLora;  ///< Flag indicating if a message has been received over LoRa.
extern volatile bool receivedWifi;  ///< Flag indicating if a message has been received over WiFi.


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
    float loraRSSI = 0;              ///< LoRa RSSI private property.
    float loraSNR = 0;               ///< LoRa SNR private property.
    float loraFrequencyError = 0;    ///< LoRa frequency error private property.
};

extern ComsManager comsManager;      ///< Global instance for Communications Manager class.

#endif
