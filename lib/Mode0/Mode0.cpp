#include "Mode0.h"
#include <Arduino.h>
#include <Utils.h>

/**
 * @file Mode0.cpp
 * @brief Implementation class for Mode 0.
 * @note  See README file for explanation of the mode 
 * @author Ivan Martin Enciso 
 */
void mode0(){
  try{
    delay(100);
    if( modeChanged() ){
      return;
    }
    lcdDisplay.printInDisplay(WAITING_FOR_COMMAND, 10);
  }
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}