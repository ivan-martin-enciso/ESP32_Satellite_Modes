#ifndef TELECOMMANDS_MANAGER_H
#define TELECOMMANDS_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Strings.h"
#include "arduino_secrets.h"
#include "StorageManager.h"
#include "Buzzer.h"

extern int volatile nextDefaultMode;

class TelecommandsManager {
public:
    int processTelecommand(String payload, int currentMode);
    void processMode0(JsonObject data);
    void processMode1(JsonObject data);
    void processMode2(JsonObject data);
    void processMode3(JsonObject data);
    void processMode4(JsonObject data);
    void processMode5(JsonObject data);

private:
    typedef void (TelecommandsManager::*TelecommandFunction)(JsonObject);
    TelecommandFunction telecommandFunction[6] = {
        &TelecommandsManager::processMode0,
        &TelecommandsManager::processMode1,
        &TelecommandsManager::processMode2,
        &TelecommandsManager::processMode3,
        &TelecommandsManager::processMode4,
        &TelecommandsManager::processMode5
    };
};

#endif 
