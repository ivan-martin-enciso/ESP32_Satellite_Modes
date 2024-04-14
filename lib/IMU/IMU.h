#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>

/**
 * @file IMU.h
 * @brief Header class for the IMU sensor.
 * @author Ivan Martin Enciso 
 */

#define SPI2_IMU_CS 10      ///< Chip select 10 for IMU. 

class IMU {
public:
  IMU();
  ~IMU();

  void initializeIMU();
  void readIMUValues();
  sensors_event_t getAcceleration();
  sensors_event_t getGyroscope();
  sensors_event_t getTemperature();
  void printIMUData();

private:
  Adafruit_LSM6DSO32 imu;         ///< Adafruit_LSM6DSO32 IMU object.
  sensors_event_t acceleration;   ///< Acceleration data 
  sensors_event_t gyroscope;      ///< Gyroscope data 
  sensors_event_t temperature;    ///< Temperature data 
};

extern IMU imu; ///< Global instance of the IMU class.

#endif
