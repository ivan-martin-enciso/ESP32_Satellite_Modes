#include "BME280.h"
/**
 * @file BME280.cpp
 * @brief Implementation class for the environmental BME280 sensor.
 * @author Ivan Martin Enciso 
 */

/**
 * @brief Constructor for the BME280 class.
 */
BME280::BME280() {}

/**
 * @brief Destructor for the BME280 class.
 */
BME280::~BME280() {}

/**
 * @brief Initializes the BME280 sensor.
 * 
 * This function initializes the BME280 sensor using Wire.h libray and the sensor address 0x76.
 * If errors occurs it is printed in the serial along with the sensor id/
 */
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

/**
 * @brief Reads the sensor values from the BME280 sensor.
 * 
 * This function reads temperature, pressure, altitude, and humidity
 * values from the BME280 sensor.
 */
void BME280::readBME280Values() {
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity = bme.readHumidity();
}

/**
 * @brief Gets the temperature value.
 * 
 * @return The temperature value.
 */
float BME280::getTemperature() {
  return temperature;
}

/**
 * @brief Gets the pressure value.
 * 
 * @return The pressure value.
 */
float BME280::getPressure() {
  return pressure;
}

/**
 * @brief Gets the altitude value.
 * 
 * @return The altitude value.
 */
float BME280::getAltitude() {
  return altitude;
}

/**
 * @brief Gets the humidity value.
 * 
 * @return The humidity value.
 */
float BME280::getHumidity() {
  return humidity;
}

/**
 * @brief Prints the BME280 sensor data.
 * 
 * This function prints the temperature, pressure, altitude, and humidity
 * values obtained from the BME280 sensor.
 */
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
