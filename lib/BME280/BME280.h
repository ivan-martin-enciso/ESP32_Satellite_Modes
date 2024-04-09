#ifndef BME280_H
#define BME280_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "Strings.h"

// Definitions
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_HEX_ADDRESS 0x76

class BME280 {
public:
  BME280();
  ~BME280();

  void initializeBME280();
  void readBME280Values();
  float getTemperature();
  float getPressure();
  float getAltitude();
  float getHumidity();
  void printBME280Data();

private:
  Adafruit_BME280 bme; //I2C
  //Adafruit_BME280 bme(BME_CS); // hardware SPI
  //Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
  float temperature;
  float pressure;
  float altitude;
  float humidity;
};

extern BME280 bme280;

#endif
