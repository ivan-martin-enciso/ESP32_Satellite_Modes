/* Implement the mode 3 functionality here
 */
#include "Mode3.h"
#include "Melodies.h"
#include <Arduino.h>
#include <Utils.h>

/**
 * @file Mode3.cpp
 * @brief Implementation class for Mode 3.
 * @note  See README file for explanation of the mode. 
 * @author Ivan Martin Enciso 
 */

/**
 * @brief Handles the end of the timer.
 * 
 * @details This function sets the servo to its final position (-90 degrees).
 *          Then it prints the end of timer in the OLED, 
 *          Plays the cantina melody and reset mode 3.
 */
void handleEndOfTimer(){
  servoController.setServoPosition(finalServoPosition);
  Serial.println(TIMER_DONE);
  lcdDisplay.printInDisplay(TIMER_DONE, 10);
  
  play_melody(cantina_band, sizeof(cantina_band), buzzerChannel);
  
  mode3Time = 10;
  mode3RemainingTime = 1;
  startTimer = false;
  currentServoStep = initialServoPosition;
}

/**
 * @brief Handles the idling state.
 * 
 * @details This function sets the servo to the initial position (90 degrees).
 *          Then it displays the set time on the OLED display, 
 *          If the touch 'UP' or touch 'Down' are pressed, it adjusts the timer
 *          If the touch 'X' is pressed, it starts the timer when touched.
 */
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

/**
 * @brief After the touch 'X' is pressed, updates the display and the servo position
 * 
 * @details This function updates the timer by displaying the remaining time on the OLED display. 
 *          It also updates the servo position until the timer reaches zero, then it calls handleEndOfTimer().
 */
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