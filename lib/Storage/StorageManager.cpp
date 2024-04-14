#include "StorageManager.h"

/**
 * @file StorageManager.cpp
 * @brief Implementation class for storing data in the EEPROM memory.
 * @author Ivan Martin Enciso 
 */

int defaultModeAddress = 0;         ///< Address for the default mode.
int defaultLdrThreshold1Address = sizeof(uint8_t);      ///< Address for the default LDR threshold 1.
int defaultLdrThreshold2Address = defaultLdrThreshold1Address + sizeof(uint16_t);           ///< Address for the default LDR threshold 2.
int defaultLdrThreshold3Address = defaultLdrThreshold2Address + sizeof(uint16_t);           ///< Address for the default LDR threshold 3.
int defaultTemperatureLowerThresholdAddress = defaultLdrThreshold3Address + sizeof(uint16_t);       ///< Address for the default lower temperature threshold.
int defaultTemperatureUpperThresholdAddress = defaultTemperatureLowerThresholdAddress + sizeof(uint8_t);        ///< Address for the default upper temperature threshold.

/**
 * @brief Constructor for StorageManager class.
 */
StorageManager::StorageManager() {}

/**
 * @brief Destructor for StorageManager class.
 */
StorageManager::~StorageManager() {}

/**
 * @brief Initializes the EEPROM memory.
 */
void StorageManager::initializeEEPROM() {
    Serial.print(INITIALIZE_EEPROM);
    if (!EEPROM.begin(eepromSize)) {
        Serial.println(INITIALIZE_EEPROM_ERROR);
        while (1) delay(1000); 
    }
    Serial.println(COMPLETE);
}

/**
 * @brief Stores an 8-bit integer data at the given EEPROM address.
 * @details If the commit is not successful, prints in the Serial monitor. 
 * @param address The address where the data will be stored.
 * @param data The 8-bit integer data to be stored.
 */
void StorageManager::storeInt8Data(int address, const uint8_t &data) {
    EEPROM.write(address, data);
    if (!EEPROM.commit()) {
        Serial.println(EEPROM_COMMIT_ERROR);
    }
}

/**
 * @brief Reads an 8-bit integer data from the given EEPROM address.
 * 
 * @param address The address from which the data will be read.
 * @return The 8-bit integer data read from the EEPROM.
 */
uint8_t StorageManager::readInt8Data(int address) {
    return EEPROM.read(address);
}

/**
 * @brief Stores an 16-bit integer data at the given EEPROM address.
 * @details If the commit is not successful, prints in the Serial monitor. 
 * @param address The address where the data will be stored.
 * @param data The 16-bit integer data to be stored.
 */
void StorageManager::storeInt16Data(int address, const uint16_t &data) {
    EEPROM.put(address, data);
    if (!EEPROM.commit()) {
        Serial.println(EEPROM_COMMIT_ERROR);
    }
}

/**
 * @brief Reads an 16-bit integer data from the given EEPROM address.
 * 
 * @param address The address from which the data will be read.
 * @return The 16-bit integer data read from the EEPROM.
 */
uint16_t StorageManager::readInt16Data(int address) {
    uint16_t data;
    EEPROM.get(address, data);  // Use EEPROM.get() to read uint16_t data
    return data;
}

/**
 * @brief Stores the default mode in EEPROM.
 * 
 * @param defaultMode The default mode to be stored.
 */
void StorageManager::storeDefaultMode(uint8_t defaultMode){
    storeInt8Data(defaultModeAddress, defaultMode);
}

/**
 * @brief Stores the default temperature thresholds in EEPROM.
 * 
 * @param threshold1 The lower temperature threshold to be stored.
 * @param threshold2 The upper temperature threshold to be stored.
 */
void StorageManager::storeTemperatureDefaultValues(uint8_t threshold1, uint8_t threshold2){
    storeInt8Data(defaultTemperatureLowerThresholdAddress, threshold1);
    storeInt8Data(defaultTemperatureUpperThresholdAddress, threshold2);
}

/**
 * @brief Stores the default LDR thresholds in EEPROM.
 * 
 * @param threshold1 The first LDR threshold to be stored.
 * @param threshold2 The second LDR threshold to be stored.
 * @param threshold3 The third LDR threshold to be stored.
 */
void StorageManager::storeLdrDefaultValues(uint16_t threshold1, uint16_t threshold2, uint16_t threshold3){
    storeInt16Data(defaultLdrThreshold1Address, threshold1);
    storeInt16Data(defaultLdrThreshold2Address, threshold2);
    storeInt16Data(defaultLdrThreshold3Address, threshold3);
}

/**
 * @brief Sets default thresholds in the EEPROM.
 */
void StorageManager::setDefaultValues(){
    uint8_t defaultMode = 3;
    uint16_t defaultLdrThreshold1 = 1000;
    uint16_t defaultLdrThreshold2 = 2000;
    uint16_t defaultLdrThreshold3 = 3000;
    uint8_t defaultTemperatureLowerThreshold = 23;
    uint8_t defaultTemperatureUpperThreshold = 35;

    storeDefaultMode(defaultMode);
    storeLdrDefaultValues(defaultLdrThreshold1, defaultLdrThreshold2, defaultLdrThreshold3);
    storeTemperatureDefaultValues(defaultTemperatureLowerThreshold, defaultTemperatureUpperThreshold);
}

/**
 * @brief Completely clears the memory.
 * @details If the commit is not successful, prints in the Serial monitor. 
 */
void StorageManager::clearEEPROM() {
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0); 
    }
    if (!EEPROM.commit()) {
        Serial.println(EEPROM_COMMIT_ERROR);
    } 
}