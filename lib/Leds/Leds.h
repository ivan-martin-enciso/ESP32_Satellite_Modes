#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include "Strings.h"

/**
 * @file Leds.h
 * @brief Header class for the LEDs control.
 * @author Ivan Martin Enciso 
 */

extern byte greenLed;         ///< Pin number for the green LED.
extern byte yellowLed;        ///< Pin number for the yellow LED.
extern byte redLed;           ///< Pin number for the red LED.

/**
 * @brief Struct for single LED control.
 */
struct Led {
  byte ledPin;            ///< Pin number of the LED.
  int ledFrequency;       ///< Frequency of the LED.
  byte ledChannel;        ///< Channel of the LED.

  void initialize();
  void turnOn();
  void turnOff();
  void fade(int dutyCycle);
};


class LedController {
public:
  LedController(byte ledCount);
  ~LedController();

  void initializeLeds();
  void turnOn(byte ledIndex);
  void turnOff(byte ledIndex);
  void turnAllOn();
  void turnAllOff();
  void fade(byte ledIndex, int brightness);
  void blink(byte ledIndex, byte times);

private:
  byte ledCount;                                ///< The number of LEDs controlled by the LedController.
  Led* leds;                                    ///< Array of LEDs controlled by the LedController.
};

extern LedController ledController;             ///< Global instance of the Leds controller class.

#endif
