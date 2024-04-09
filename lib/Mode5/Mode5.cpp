/* Implement the mode 5 functionality here
 */
#include "Mode5.h"

#include <Arduino.h>
#include <Utils.h>
#include <Melodies.h>

void mode5(){
  try{
    delay(100);
    if( modeChanged() ){
      return;
    }
    lcdDisplay.printInDisplay("", 10);
  }
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}