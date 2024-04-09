/* Implement the mode 1 functionality here
 */
#include "Mode1.h"
#include <Arduino.h>
#include <Utils.h>

// Define LCD display dimensions
const int oledStartingPoint = 0;
const int oledHeight = 32;
const int oledWidth = 128;
const int threshold = 11;

// Define satellite symbol dimensions
const int satelliteWidth = 8;
const int satelliteHeight = 8;

// Define satellite symbol bitmap
unsigned char satelliteBitmap[satelliteWidth] = {
  B00111100, B01111110, B11011011, B11111111, 
  B11111111, B11011011, B01111110, B00111100
};

// Mapping function to map current acceleration from the IMU into the height and width of the OLED display 
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