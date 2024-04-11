/* Implement the mode 5 functionality here
 */
#include "Mode5.h"
#include <Arduino.h>
#include <Utils.h>


void mode5(){
  try{
    delay(100);
    if( modeChanged() ){
      startMode5 = false;
      return;
    }
    chartDisplay.initializeChartDisplay();
    chartDisplay.displayChart(mode5Value);
  }
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}