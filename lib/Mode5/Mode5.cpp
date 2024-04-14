#include "Mode5.h"
#include <Arduino.h>
#include <Utils.h>

/**
 * @file Mode5.cpp
 * @brief Implementation class for Mode 5.
 * @note  See README file for explanation of the mode. 
 * @author Ivan Martin Enciso 
 */
String currentReading = "TEMPERATURE";
bool firstLoop = true;


/**
 * @brief When M5 data is received by telecommand, prints the received reading.
 * 
 * @details The function reads the current reading and the received reading, if this changes
 *          it briefly prints the new reading the the display. 
 */
void changeReading(){
  if(!currentReading.equals(reading) || firstLoop){
    firstLoop = false;
    currentReading = reading;
    Serial.println(lcdDisplay.getTextBounds(currentReading));
    int xAxis = (128 - lcdDisplay.getTextBounds(currentReading)) / 2;
    lcdDisplay.printInDisplay(currentReading, 15, xAxis);
    delay(700);
  }
}

void mode5(){
  try{
    delay(100);
    if( modeChanged() ){
      startMode5 = false;
      firstLoop = true;
      return;
    }
    changeReading();
    if(refreshMode5){
      chartDisplay.initializeChartDisplay();
      chartDisplay.displayChart(readMode5Data());
    }
  }
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}