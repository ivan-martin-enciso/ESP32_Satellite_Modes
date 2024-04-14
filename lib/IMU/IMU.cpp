#include "IMU.h"
#include "Strings.h"
/**
 * @file IMU.cpp
 * @brief Implementation class for the IMU sensor.
 * @author Ivan Martin Enciso
 */

/**
 * @brief Constructor for the IMU class.
 */
IMU::IMU() {}

/**
 * @brief Destructor for the IMU class.
 */
IMU::~IMU() {}

/**
 * @brief Initialize the IMU sensor using SPI to not interfere with LoRA.
 * @note If IMU is initialized using MISO and MOSI, this interferes with LoRa. 
 * 
 * @details This method initializes the IMU sensor and checks for the status.
 *          If initialization fails, it prints an error message and enters an infinite loop.
 */
void IMU::initializeIMU() {
  Serial.print(INITIALIZE_IMU);
  bool status = imu.begin_SPI(SPI2_IMU_CS);
  if (!status) {
    Serial.println(IMU_SETUP_ERROR);
    while (1) delay(10);
  }
  Serial.println(COMPLETE);
}

/**
 * @brief Read all sensor values from the IMU.
 * 
 * @details This method reads acceleration, gyroscope, and temperature values from the IMU sensor.
 *          It then stores the values in the given private variables. 
 */
void IMU::readIMUValues() {
  imu.getEvent(&acceleration, &gyroscope, &temperature);
}

/**
 * @brief Get the acceleration data from the IMU.
 * 
 * @return Acceleration x, y, and z axis in m/s^2.
 */
sensors_event_t IMU::getAcceleration() {
  return acceleration;
}

/**
 * @brief Get the gyroscope data from the IMU.
 * 
 * @return Gyroscope x, y, and z axis in radians/s.
 */
sensors_event_t IMU::getGyroscope() {
  return gyroscope;
}

/**
 * @brief Get the temperature data from the IMU.
 * 
 * @return Temperature in °C.
 */
sensors_event_t IMU::getTemperature() {
  return temperature;
}

/**
 * @brief Print the IMU sensor data.
 * 
 * @details This method prints the acceleration, gyroscope, and temperature data 
 *          from the IMU sensor in the serial monitor.
 * @note Used only for test purposes. 
 */
void IMU::printIMUData() {
  Serial.println(SEPARATOR);
  Serial.print(ACCELERATION);
  Serial.print(X);
  Serial.print(acceleration.acceleration.x);
  Serial.print(Y);
  Serial.print(acceleration.acceleration.y);
  Serial.print(Z);
  Serial.println(acceleration.acceleration.z);

  Serial.print(GYROSCOPE);
  Serial.print(X);
  Serial.print(gyroscope.gyro.x);
  Serial.print(Y);
  Serial.print(gyroscope.gyro.y);
  Serial.print(Z);
  Serial.println(gyroscope.gyro.z);

  Serial.print(TEMPERATURE);
  Serial.print(temperature.temperature);
  Serial.println(CELSIUS);
  Serial.println(SEPARATOR);
  delay(100);
}
