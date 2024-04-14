#ifndef TELECOMMANDS_MANAGER_H
#define TELECOMMANDS_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Strings.h"
#include "arduino_secrets.h"
#include "StorageManager.h"
#include "Buzzer.h"
#include "ChartDisplay.h"

/**
 * @file TelecommandsManager.h
 * @brief Header class for the handling Telecommands.
 * @author Ivan Martin Enciso 
 */
extern int volatile nextDefaultMode; ///< The next default mode.
extern int volatile mode5Function;   ///< Mode 5 function set int Utils.cpp by readMode5Data()
class TelecommandsManager {
public:
    int processTelecommand(String payload, int currentMode);

    // Mode-specific telecommand processing functions
    void processMode0(JsonObject data);
    void processMode1(JsonObject data);
    void processMode2(JsonObject data);
    void processMode3(JsonObject data);
    void processMode4(JsonObject data);
    void processMode5(JsonObject data);

    /**
     * @brief Struct to store readings labels and corresponding functions set in readMode5Data().
     */
    struct Readings {
        String label; ///< Label of the reading.
        int function; ///< Function corresponding to the reading.
    };
    Readings readings[5] = {
        {"TEMPERATURE", 0},
        {"LDR", 1},
        {"HUMIDITY", 2},
        {"ALTITUDE", 3},
        {"PRESSURE", 4}
    };

private:
    typedef void (TelecommandsManager::*TelecommandFunction)(JsonObject); ///<Array to store mode-specific processing functions used in reflection.
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
