#include "TelecommandsManager.h"

/**
 * @file TelecommandsManager.cpp
 * @brief Implementation class for the handling Telecommands.
 * @author Ivan Martin Enciso 
 */

int volatile nextDefaultMode = 0;                                            ///< The next default mode.
int volatile mode5Function = 1;                                              ///< Mode 5 function set int Utils.cpp by readMode5Data()

typedef void (TelecommandsManager::*TelecommandFunction)(JsonObject);        ///<Array to store mode-specific processing functions used in reflection.
TelecommandFunction telecommandFunction[] = {
    &TelecommandsManager::processMode0,
    &TelecommandsManager::processMode1,
    &TelecommandsManager::processMode2,
    &TelecommandsManager::processMode3,
    &TelecommandsManager::processMode4,
    &TelecommandsManager::processMode5
};

/**
 * @brief Checsk if a mode is within the valid range.
 * 
 * @param mode The mode to check.
 * 
 * @return True if the mode is within the range [0, 5], else false.
 */
bool isModeInRange(int mode){
    return mode >= 0 && mode <= 5;
}

/**
 * @brief Process a the telecommand received of WiFi or LoRa as a String.
 * 
 * @details The function converts the received string into a JSON document and extracts the corresponding values.
 *          First it checks if the the cadse_id is equal to 16, then it process the type being (telecommand or setDefaultMode)
 *          If the mode is equal to the current mode it extracts the Data from the JSON and call for the given mode process function. 
 *          If the mode is different to the current mode it calls the buzzer to beep the number of modes and returns the mode to change. 
 *          If an error occurs it always returns -1.
 * 
 * @param payload The String payload containing the telecommand data.
 * @param currentMode The current mode.
 * @return The mode processed or error -1.
 */
int TelecommandsManager::processTelecommand(String payload, int currentMode) {

    JsonDocument doc; 
    DeserializationError error = deserializeJson(doc, payload);
    if(error) return -1;

    // Extract values from the JSON document
    int cadse_id = doc["cadse_id"];
    String type = doc["type"];

    if (cadse_id == SECRET_MQTT_BOARDID) {
        Serial.print(F("Telecommand received on topic: "));
        Serial.println(TELECOMMAND_TOPIC);

        int mode = doc["mode"];
        if(!isModeInRange(mode)){
            Serial.println(UNKNOWN_MODE + String(mode));
            return -1;
        }

        if(type.equals("setDefaultMode")){
            int newDefaultMode = mode;
            Serial.println(STORE_DEFAULT_MODE_TC + String(newDefaultMode));
            storageManager.storeDefaultMode(newDefaultMode);
            nextDefaultMode = newDefaultMode;
            return -2;
        }

        if(type.equals("telecommand")){
            if(mode == currentMode){
                JsonObject data = doc["data"];
                Serial.println(TELECOMMAND_PROCESSING + String(mode));
                (this->*telecommandFunction[mode])(data);
                return -3; 
            }
            else{
                buzzer.beep(mode, 100);
            }
        }
        return mode;
    }
    return -1;
}

/**
 * @brief Process data for mode 0.
 * 
 * @details This function process data specific to mode 0. Currently not implemented
 * 
 * @param data The JSON object containing mode 0 data.
 */
void TelecommandsManager::processMode0(JsonObject data) {}

/**
 * @brief Process data for mode 1.
 * 
 * @details This function process data specific to mode 1. Currently not implemented
 * 
 * @param data The JSON object containing mode 1 data.
 */
void TelecommandsManager::processMode1(JsonObject data) {}

/**
 * @brief Process data for mode 2.
 * 
 * @details This function process data specific to mode 2. 
 *          Checks if tempLower and tempUpper are valid and stores the data on the persistent memory. 
 * 
 * @param data The JSON object containing mode 2 data.
 */
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

/**
 * @brief Process data for mode 3.
 * 
 * @details This function process data specific to mode 3. Currently not implemented
 * 
 * @param data The JSON object containing mode 3 data.
 */
void TelecommandsManager::processMode3(JsonObject data) {}

/**
 * @brief Process data for mode 4.
 * 
 * @details This function process data specific to mode 4. 
 *          Checks if defLdrT1, defLdrT2 and defLdrT3 are valid and stores the data on the persistent memory. 
 * 
 * @param data The JSON object containing mode 4 data.
 */
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

/**
 * @brief Process data for mode 5.
 * 
 * @details This function process data specific to mode 5. 
 *          Checks if minY, maxY and reading are valid.
 *          Then it checks if the reading is on the Readings struct and fetches the proper Label and Function, which are
 *          then updated in the global variables.  
 * 
 * @param data The JSON object containing mode 5 data.
 */
void TelecommandsManager::processMode5(JsonObject data) {
     if (!data.containsKey("minY") || !data.containsKey("maxY") || !data.containsKey("reading")) {
        Serial.println("Error: Missing mode 5 data in received data.");
        return;
    }

    String minYLabel = data["minY"];
    String maxYLabel = data["maxY"];
    double minYValue = data["minY"];
    double maxYValue = data["maxY"];
    String receivedReading = data["reading"];

    if (!data["minY"].is<String>() || 
        !data["maxY"].is<String>() || 
        !data["reading"].is<String>()) {
        Serial.println("Error: Invalid mode 5 data.");
        return;
    }
    minYLimitLabel = minYLabel;
    maxYLimitLabel = maxYLabel;
    minYLimitValue = minYValue;
    maxYLimitValue = maxYValue;
    receivedReading.toUpperCase();
    for (const auto& item : TelecommandsManager::readings) {
        if (item.label.equals(receivedReading)) {
            reading = item.label;
            mode5Function = item.function;
        }
    }

}