#ifndef BME280_H
#define BME280_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "Strings.h"

/**
 * @file BME280.h
 * @brief Header class for the environmental BME280 sensor.
 * @author Ivan Martin Enciso 
 */
#define SEALEVELPRESSURE_HPA (1013.25) ///< Sea level pressure definition.
#define BME280_HEX_ADDRESS 0x76 ///< Address of the BME280 sensor in the Cadse board.

/**
 * @brief Constructor for the BME280 class.
 */
class BME280 {
public:
  BME280();
  ~BME280();
\
  void initializeBME280();
  void readBME280Values();
  float getTemperature();
  float getPressure();
  float getAltitude();
  float getHumidity();
  void printBME280Data();

private:
  Adafruit_BME280 bme; ///< Adafruit_BME280 object for the sensor.
  float temperature;   ///< Temperature value read from the sensor.
  float pressure;      ///< Pressure value read from the sensor.
  float altitude;      ///< Altitude value read from the sensor.
  float humidity;      ///< Humidity value read from the sensor.
};

extern BME280 bme280; ///< Global instance of the BME280 sensor.

#endif
