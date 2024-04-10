#include "TelecommandsManager.h"

int volatile nextDefaultMode = 0;

typedef void (TelecommandsManager::*TelecommandFunction)(JsonObject);
TelecommandFunction telecommandFunction[] = {
    &TelecommandsManager::processMode0,
    &TelecommandsManager::processMode1,
    &TelecommandsManager::processMode2,
    &TelecommandsManager::processMode3,
    &TelecommandsManager::processMode4,
    &TelecommandsManager::processMode5
};

bool isModeInRange(int mode){
    return mode >= 0 && mode <= 5;
}

int TelecommandsManager::processTelecommand(String payload, int currentMode) {

    JsonDocument doc; 
    DeserializationError error = deserializeJson(doc, payload);
    if(error) return -1;

    // Extract values from the JSON document
    int cadse_id = doc["cadse_id"];
    if (cadse_id == SECRET_MQTT_BOARDID) {
        Serial.print(F("Telecommand received on topic: "));
        Serial.println(TELECOMMAND_TOPIC);

        if(doc.containsKey("data") && doc["data"].containsKey("setDefaultMode")){
            int newDefaultMode = doc["data"]["setDefaultMode"];
            if(!isModeInRange(newDefaultMode)){
                Serial.println(UNKNOWN_MODE + String(newDefaultMode));
                return -1;
            }
            Serial.println(STORE_DEFAULT_MODE_TC + String(newDefaultMode));
            storageManager.storeDefaultMode(newDefaultMode);
            nextDefaultMode = newDefaultMode;
            return -2;
        }

        int mode = doc["mode"];

        if(!isModeInRange(mode)){
            Serial.println(UNKNOWN_MODE + String(mode));
            return -1;
        }

        if(mode == currentMode){
            JsonObject data = doc["data"];
            Serial.println(TELECOMMAND_PROCESSING + String(mode));
            (this->*telecommandFunction[mode])(data); 
        }
        else{
            buzzer.beep(mode, 100);
        }
        return mode;
    }
    return -1;
}

void TelecommandsManager::processMode0(JsonObject data) {
    // Process mode 0
}

void TelecommandsManager::processMode1(JsonObject data) {
    // Process mode 1
}

void TelecommandsManager::processMode2(JsonObject data) {
    if (!data.containsKey("tempLower") || !data.containsKey("tempUpper")) {
        Serial.println("Error: Missing temperature thresholds in received data.");
        return;
    }

    int lowerThreshold = data["tempLower"];
    int upperThreshold = data["tempUpper"];

    if (!data["tempLower"].is<int>() || !data["tempUpper"].is<int>() ||
        lowerThreshold < 10 || lowerThreshold > 40 || upperThreshold < 10 || upperThreshold > 40) {
        Serial.println("Error: Invalid temperature thresholds. Thresholds must be integers in the range 10 to 40.");
        return;
    }
    storageManager.storeTemperatureDefaultValues(lowerThreshold, upperThreshold);
}

void TelecommandsManager::processMode3(JsonObject data) {
    // Process mode 3
}

void TelecommandsManager::processMode4(JsonObject data) {
    if (!data.containsKey("defLdrT1") || !data.containsKey("defLdrT2") || !data.containsKey("defLdrT3")) {
        Serial.println("Error: Missing ldr thresholds in received data.");
        return;
    }

    int16_t defaultLdrThreshold1 = data["defLdrT1"];
    int16_t defaultLdrThreshold2 = data["defLdrT2"];
    int16_t defaultLdrThreshold3 = data["defLdrT3"];

    if (!data["defLdrT1"].is<int16_t>() || 
        !data["defLdrT2"].is<int16_t>() || 
        !data["defLdrT3"].is<int16_t>() ||
        defaultLdrThreshold1 < 0 || defaultLdrThreshold1 > 5000 ||
        defaultLdrThreshold2 < 0 || defaultLdrThreshold2 > 5000 ||
        defaultLdrThreshold3 < 0 || defaultLdrThreshold3 > 5000) {
        Serial.println("Error: Invalid ldr thresholds. Thresholds must be integers in the range 0 to 5000.");
        return;
    }
    storageManager.storeLdrDefaultValues(defaultLdrThreshold1, defaultLdrThreshold2, defaultLdrThreshold3);
}

void TelecommandsManager::processMode5(JsonObject data) {
    // Process mode 5
}