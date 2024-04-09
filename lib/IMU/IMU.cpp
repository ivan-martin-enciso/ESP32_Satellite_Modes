#include "IMU.h"
#include "Strings.h"

IMU::IMU() {}

IMU::~IMU() {}

void IMU::initializeIMU() {
  Serial.print(INITIALIZE_IMU);
  bool status = imu.begin_SPI(SPI2_IMU_CS);
  if (!status) {
    Serial.println(IMU_SETUP_ERROR);
    while (1) delay(10);
  }
  Serial.println(COMPLETE);
}

void IMU::readIMUValues() {
  imu.getEvent(&acceleration, &gyroscope, &temperature);
}

sensors_event_t IMU::getAcceleration() {
  return acceleration;
}

sensors_event_t IMU::getGyroscope() {
  return gyroscope;
}

sensors_event_t IMU::getTemperature() {
  return temperature;
}

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
