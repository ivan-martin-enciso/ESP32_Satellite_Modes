#ifndef LDR_H
#define LDR_H

#include <Arduino.h>
/**
 * @file Ldr.h
 * @brief Header class for the Ldr sensor.
 * @author Ivan Martin Enciso 
 */
class Ldr {
public:
    Ldr();
    ~Ldr();

    int readLdr();

private:
    static const int ldrPin = 6;         ///< Ldr pin
};

extern Ldr ldr;                          ///< Global instance of the Ldr class.

#endif
