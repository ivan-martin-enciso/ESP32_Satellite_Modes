/* Implement the mode 3 functionality here
 */
#include "Mode3.h"
#include "Melodies.h"
#include <Arduino.h>
#include <Utils.h>

void handleEndOfTimer(){
  servoController.setServoPosition(finalServoPosition);
  Serial.println(TIMER_DONE);
  lcdDisplay.printInDisplay(TIMER_DONE, 10);
  play_melody(the_lick, sizeof(the_lick), buzzerChannel);
  mode3Time = 10;
  mode3RemainingTime = 1;
  startTimer = false;
  currentServoStep = initialServoPosition;
}

void idling(){
  servoController.setServoPosition(initialServoPosition);
  String setTimeString = SET_TIME;
  setTimeString.concat(mode3Time);
  setTimeString.concat(SECONDS);
  lcdDisplay.printInDisplay(setTimeString, 10);
  if( touchRead(touchUpPin) > touchThreshold ){
    if(mode3Time > 1) mode3Time++;
  }
  if( touchRead(touchDownPin) > touchThreshold ){
    if(mode3Time > 1) mode3Time--;
  }
  if( !startTimer && touchRead(touchXPin) > touchThreshold ){
    servoStepValue = int(finalServoPosition / mode3Time);
    startTimer = true;
    mode3RemainingTime = mode3Time;
  }
}

void updateTimer(){
  if(mode3RemainingTime > 0){
    String remainingTimeString = REMAINING_TIME;
    remainingTimeString.concat(mode3RemainingTime);
    remainingTimeString.concat(SECONDS);
    lcdDisplay.printInDisplay(remainingTimeString, 10);
    servoController.setServoPosition(currentServoStep);
  }
  else{
    handleEndOfTimer();
  }
}

void mode3(){
  try{
    delay(100);
    if( modeChanged() ){
      servoController.setServoPosition(initialServoPosition);
      return;
    }
    if(startTimer){
      updateTimer();
    }
    else{
      idling();
    }
  }
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}