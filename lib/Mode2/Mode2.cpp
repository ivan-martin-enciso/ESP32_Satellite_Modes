/* Implement the mode 2 functionality here
 */
#include "Mode2.h"
#include <Arduino.h>
#include <Utils.h>
#include "MorseCodeController.h"

float currentTemperature = 0.0;
MorseCodeController morseCodeController;

void displayTemperature(){
  String firstLine = THS1 + String(temperatureLowerThreshold) + CELSIUS + SPACE + THS2 + String(temperatureUpperThreshold) + CELSIUS;
  lcdDisplay.printInDisplay(firstLine, 10, 0, true);
  String temperatureString = TEMPERATURE;
  temperatureString.concat(currentTemperature);
  temperatureString.concat(CELSIUS);
  lcdDisplay.printInDisplay(temperatureString, 20, 0, false);
}

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