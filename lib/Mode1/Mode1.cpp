#include "Mode1.h"
#include <Arduino.h>
#include <Utils.h>

/**
 * @file Mode1.cpp
 * @brief Implementation class for Mode 1.
 * @note  See README file for explanation of the mode. 
 * @author Ivan Martin Enciso 
 */

const int oledStartingPoint = 0;                       ///< Starting point for the display.
const int oledHeight = 32;                             ///< Display height.
const int oledWidth = 128;                             ///< Display width.
const int threshold = 11;                              ///< IMU threshold.
const int satelliteWidth = 8;                          ///< Bitmap width.
const int satelliteHeight = 8;                         ///< Bitmap height.

unsigned char satelliteBitmap[satelliteWidth] = {      ///< Satellite symbol bitmap
  B00111100, B01111110, B11011011, B11111111, 
  B11111111, B11011011, B01111110, B00111100
};

/**
 * @brief Maps the value from the IMU to the display.
 * 
 * @param value The value to map.
 * @param inMin The minimum value of the input range. In this case the threshold. 
 * @param inMax The maximum value of the input range. In this case the threshold. 
 * @param outMin The minimum value of the output range. 
 * @param outMax The maximum value of the output range.
 * @return The mapped value.
 */
int mapValue(float value, float inMin, float inMax, int outMin, int outMax) {
  return (int)((value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin);
}

void mode1(){
  try{
    delay(100);
    if( modeChanged() ){
      return;
    }
    imu.readIMUValues();
    sensors_event_t acceleration = imu.getAcceleration();
    float x = acceleration.acceleration.x;
    float y = acceleration.acceleration.y;
    imu.printIMUData();
    int newX = mapValue(y, -threshold, threshold, oledStartingPoint, oledWidth);
    int newY = mapValue(x, threshold, -threshold, oledStartingPoint, oledHeight);

    lcdDisplay.drawSatellite(newX, newY, satelliteBitmap, satelliteWidth, satelliteHeight);
  }
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}