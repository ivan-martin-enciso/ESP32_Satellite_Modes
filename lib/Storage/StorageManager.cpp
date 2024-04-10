#include "StorageManager.h"

int defaultModeAddress = 0;
int defaultLdrThreshold1Address = sizeof(uint8_t);
int defaultLdrThreshold2Address = defaultLdrThreshold1Address + sizeof(uint16_t);
int defaultLdrThreshold3Address = defaultLdrThreshold2Address + sizeof(uint16_t);
int defaultTemperatureLowerThresholdAddress = defaultLdrThreshold3Address + sizeof(uint16_t);
int defaultTemperatureUpperThresholdAddress = defaultTemperatureLowerThresholdAddress + sizeof(uint8_t);


StorageManager::StorageManager() {}

StorageManager::~StorageManager() {}

void StorageManager::initializeEEPROM() {
    Serial.print(INITIALIZE_EEPROM);
    if (!EEPROM.begin(eepromSize)) {
        Serial.println(INITIALIZE_EEPROM_ERROR);
        while (1) delay(1000); 
    }
    Serial.println(COMPLETE);
}


void StorageManager::storeInt8Data(int address, const uint8_t &data) {
    EEPROM.write(address, data);
    if (!EEPROM.commit()) {
        Serial.println(EEPROM_COMMIT_ERROR);
    }
}

uint8_t StorageManager::readInt8Data(int address) {
    return EEPROM.read(address);
}


void StorageManager::storeInt16Data(int address, const uint16_t &data) {
    EEPROM.put(address, data);
    if (!EEPROM.commit()) {
        Serial.println(EEPROM_COMMIT_ERROR);
    }
}

uint16_t StorageManager::readInt16Data(int address) {
    uint16_t data;
    EEPROM.get(address, data);  // Use EEPROM.get() to read uint16_t data
    return data;
}

void StorageManager::storeDefaultMode(uint8_t defaultMode){
    storeInt8Data(defaultModeAddress, defaultMode);
}

void StorageManager::storeTemperatureDefaultValues(uint8_t threshold1, uint8_t threshold2){
    storeInt8Data(defaultTemperatureLowerThresholdAddress, threshold1);
    storeInt8Data(defaultTemperatureUpperThresholdAddress, threshold2);
}

void StorageManager::storeLdrDefaultValues(uint16_t threshold1, uint16_t threshold2, uint16_t threshold3){
    storeInt16Data(defaultLdrThreshold1Address, threshold1);
    storeInt16Data(defaultLdrThreshold2Address, threshold2);
    storeInt16Data(defaultLdrThreshold3Address, threshold3);
}

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

void StorageManager::clearEEPROM() {
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0); 
    }
    if (!EEPROM.commit()) {
        Serial.println(EEPROM_COMMIT_ERROR);
    } 
}