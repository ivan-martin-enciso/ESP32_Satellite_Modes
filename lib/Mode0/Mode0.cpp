/* Mode 0
  Idle Mode: Waiting for commands. 
 */
#include "Mode0.h"
#include <Arduino.h>
#include <Utils.h>

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