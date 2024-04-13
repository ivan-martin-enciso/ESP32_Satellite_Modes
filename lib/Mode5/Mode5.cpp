/* Implement the mode 5 functionality here
 */
#include "Mode5.h"
#include <Arduino.h>
#include <Utils.h>
String currentReading = "TEMPERATURE";
bool firstLoop = true;

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