#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Strings.h"

/**
 * @file StorageManager.h
 * @brief Header class for storing data in the EEPROM memory.
 * @author Ivan Martin Enciso 
 */

extern int defaultModeAddress;                          ///< Address for the default mode.
extern int defaultLdrThreshold1Address;                 ///< Address for the default LDR threshold 1.
extern int defaultLdrThreshold2Address;                 ///< Address for the default LDR threshold 2.
extern int defaultLdrThreshold3Address;                 ///< Address for the default LDR threshold 3.
extern int defaultTemperatureLowerThresholdAddress;     ///< Address for the default lower temperature threshold.
extern int defaultTemperatureUpperThresholdAddress;     ///< Address for the default upper temperature threshold.


class StorageManager {
public:
    StorageManager();
    ~StorageManager();
    void initializeEEPROM();
    void storeInt8Data(int address, const uint8_t &data);
    void storeInt16Data(int address, const uint16_t &data);
    void storeDefaultMode(uint8_t defaultMode);
    void storeTemperatureDefaultValues(uint8_t threshold1, uint8_t threshold2);
    void storeLdrDefaultValues(uint16_t threshold1, uint16_t threshold2, uint16_t threshold3);
    uint8_t readInt8Data(int address);
    uint16_t readInt16Data(int address);
    void setDefaultValues();
    void clearEEPROM();
private:
    static const int eepromSize = 1024;             ///< EEPROM memory size
};

extern StorageManager storageManager;               ///< Global instance of the storage manager class.
#endif
