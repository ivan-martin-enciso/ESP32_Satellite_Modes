#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Strings.h"

extern int defaultModeAddress;
extern int defaultLdrThreshold1Address;
extern int defaultLdrThreshold2Address;
extern int defaultLdrThreshold3Address;
extern int defaultTemperatureLowerThresholdAddress;
extern int defaultTemperatureUpperThresholdAddress;

class StorageManager {
public:
    StorageManager();
    ~StorageManager();
    void initializeEEPROM();
    void storeInt8Data(int address, const uint8_t &data);
    void storeInt16Data(int address, const uint16_t &data);
    uint8_t readInt8Data(int address);
    uint16_t readInt16Data(int address);
    void setDefaultValues();
    void clearEEPROM();
private:
    static const int eepromSize = 1024;
};

extern StorageManager storageManager;
#endif
