#include "Mode2.h"
#include <Arduino.h>
#include <Utils.h>
#include "MorseCodeController.h"

/**
 * @file Mode2.cpp
 * @brief Implementation class for Mode 2.
 * @note  See README file for explanation of the mode. 
 * @author Ivan Martin Enciso 
 */

float currentTemperature = 0.0;           ///< The current temperature read from the sensor.
MorseCodeController morseCodeController;  ///< Object for Morse code translation.

/**
 * @brief Displays temperature information on the LCD display.
 * 
 * @details This function displays temperature information including thresholds and current temperature on the OLED display.
 */
void displayTemperature(){
  String firstLine = THS1 + String(temperatureLowerThreshold) + CELSIUS + SPACE + THS2 + String(temperatureUpperThreshold) + CELSIUS;
  lcdDisplay.printInDisplay(firstLine, 10, 0, true);
  String temperatureString = TEMPERATURE;
  temperatureString.concat(currentTemperature);
  temperatureString.concat(CELSIUS);
  lcdDisplay.printInDisplay(temperatureString, 20, 0, false);
}

/**
 * @brief Checks the current temperature and thresholds and executes the alarm.
 * 
 * @details This function reads the temperature from the sensor, checks the thresholds,
 *          and either displays the temperature or activates the Morse code for SOS signal.
 */
void checkTemperature(){
  bme280.readBME280Values();
  currentTemperature = bme280.getTemperature();
  
  if(currentTemperature < temperatureLowerThreshold || currentTemperature > temperatureUpperThreshold){
    morseCodeController.translateToMorseCode(SOS);
    if(nextMode == currentMode) delay(350);
  }else{
    displayTemperature();
  }
}

void mode2(){
  try{
    delay(100);
    if( modeChanged() ){
      return;
    }
    checkTemperature();
  }
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}