#include "Ldr.h"

/**
 * @file Ldr.cpp
 * @brief Implementation class for the Ldr sensor.
 * @author Ivan Martin Enciso
 */

/**
 * @brief Constructor for the Ldr class.
 */
Ldr::Ldr() {}

/**
 * @brief Destructor for the Ldr class.
 */
Ldr::~Ldr() {}

/**
 * @brief Read the LDR (Light Dependent Resistor) sensor value.
 * 
 * @return The analog value read from the LDR sensor as an integer.
 */
int Ldr::readLdr() {
    return analogRead(ldrPin);
}
