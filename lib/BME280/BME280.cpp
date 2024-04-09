#include "BME280.h"

BME280::BME280() {}

BME280::~BME280() {}

void BME280::initializeBME280() {
  Serial.print(INITIALIZE_BME280);
  unsigned status;
  status = bme.begin(BME280_HEX_ADDRESS);
  if (!status) {
    Serial.println(BME280_SENSOR_ERROR);
    Serial.print(BME280_SENSOR_ID_ADDRESS); 
    Serial.println(bme.sensorID(), 16);
    Serial.println(BME280_SENSOR_ID_BAD_ADDRESS);
    Serial.println(BME280_SENSOR_ID_BMP_280);
    Serial.println(BME280_SENSOR_ID_BME_280);
    Serial.println(BME280_SENSOR_ID_BME_680);
    while (1) delay(10);
  }
  Serial.println(COMPLETE);
}

void BME280::readBME280Values() {
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity = bme.readHumidity();
}

float BME280::getTemperature() {
  return temperature;
}

float BME280::getPressure() {
  return pressure;
}

float BME280::getAltitude() {
  return altitude;
}

float BME280::getHumidity() {
  return humidity;
}

void BME280::printBME280Data() {
  Serial.println(SEPARATOR);
  Serial.print(TEMPERATURE);
  Serial.print(temperature);
  Serial.println(CELSIUS);
  Serial.print(PRESSURE);
  Serial.print(pressure);
  Serial.println(PASCALS);
  Serial.print(ALTITUDE);
  Serial.print(altitude);
  Serial.println(METERS);
  Serial.print(HUMIDITY);
  Serial.print(humidity);
  Serial.println(PERCENTAGE);
  Serial.println(SEPARATOR);
}
