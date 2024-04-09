#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>

// Definitions
#define SPI2_IMU_CS 10

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
  Adafruit_LSM6DSO32 imu;
  sensors_event_t acceleration;
  sensors_event_t gyroscope;
  sensors_event_t temperature;
};

extern IMU imu;

#endif
