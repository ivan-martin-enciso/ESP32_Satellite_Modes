#include "Mode4.h"
#include <Arduino.h>
#include <Utils.h>

/**
 * @file Mode4.cpp
 * @brief Implementation class for Mode 4.
 * @note  See README file for explanation of the mode. 
 * @author Ivan Martin Enciso 
 */

/**
 * @brief Prints the Ldr values on the Serial.
 * @note  Only used for testing purposes. 
 */
void printLdrValues(int ldrValue) {
  Serial.print(LDR);
  Serial.println(ldrValue);
  Serial.print(THS1);
  Serial.println(ldrThreshold1);
  Serial.print(THS2);
  Serial.println(ldrThreshold2);
  Serial.print(THS3);
  Serial.println(ldrThreshold3);
}

/**
 * @brief Displays the Ldr values on the OLED.
 */
void displayLdrValues(int ldrValue) {
  String firstLine = LDR + String(ldrValue) + SPACE + THS1  + String(ldrThreshold1);
  lcdDisplay.printInDisplay(firstLine, 10, 0, true);
  String secondLine = THS2  + String(ldrThreshold2) + SPACE + THS3  + String(ldrThreshold3);
  lcdDisplay.printInDisplay(secondLine, 20, 0, false);
}

void mode4() {
  try {
    delay(100);
    if (modeChanged()) {
      ledController.turnAllOff();
      return;
    }
    int ldrValue = ldr.readLdr();
    displayLdrValues(ldrValue);

    if (ldrValue < ldrThreshold1) {
      ledController.turnAllOff();
    } else if (ldrValue < ldrThreshold2) {
      ledController.turnOn(greenLed);
      ledController.turnOff(yellowLed);
      ledController.turnOff(redLed);
    } else if (ldrValue < ldrThreshold3) {
      ledController.turnOn(greenLed);
      ledController.turnOn(yellowLed);
      ledController.turnOff(redLed);
    } else {
      ledController.turnAllOn();
    }
  } catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}
