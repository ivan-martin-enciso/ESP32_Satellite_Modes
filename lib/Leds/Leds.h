#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include "Strings.h"

extern byte greenLed;
extern byte yellowLed; 
extern byte redLed;

//Single Led struct
struct Led {
  byte ledPin;
  int ledFrequency;
  byte ledChannel;

  void initialize();
  void turnOn();
  void turnOff();
  void fade(int dutyCycle);
};

// Led Class Controller
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
  byte ledCount;
  Led* leds;
};

extern LedController ledController;             // Instance of Leds Controller class
#endif