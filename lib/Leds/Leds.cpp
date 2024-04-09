/* Leds.cpp
 * File containing all LEDs related functions 
 */

//Libraries
#include <Leds.h>

//Leds Definition 
//Definitions
byte ledsPins [] = {35, 36, 37};                    // Array for leds GPIOS (Green, Yellow, Red)
int ledFrequencies [] ={5000, 5000, 5000};          // Array for leds frequencies
byte ledChannels [] = {3, 4, 5};                    // Array for leds channels 
#define RESOLUTION 8                                // Resolution
#define TOTAL_LEDS 3                                // Number of Leds 
LedController ledController(TOTAL_LEDS);            // Led controller object
byte greenLed = 0;
byte yellowLed = 1; 
byte redLed = 2;

void Led::initialize() {
  ledcSetup(ledChannel, ledFrequency, RESOLUTION);   // Channel, frequency, Resolution
  ledcAttachPin(ledPin, ledChannel);                 // Pin, Channel
}

void Led::turnOn() {
  ledcWrite(ledChannel, 255); // 100% duty cycle - full brightness
}

void Led::turnOff() {
  ledcWrite(ledChannel, 0); // 0% duty cycle - off
}

void Led::fade(int dutyCycle) {
  ledcWrite(ledChannel, dutyCycle); // Set duty cycle for fading
}

LedController::LedController(byte ledCount) {
  this->ledCount = ledCount;
  leds = new Led[ledCount];
}

LedController::~LedController() {
  delete[] leds;
}

void LedController::initializeLeds() {
  Serial.print(INITIALIZE_LEDS);
  for (byte i = 0; i < ledCount; i++) {
    leds[i].ledPin = ledsPins[i];
    leds[i].ledFrequency = ledFrequencies[i];
    leds[i].ledChannel = ledChannels[i];
    leds[i].initialize();
    delay(100);
    leds[i].turnOn();
    delay(100);
    leds[i].turnOff();
  }
  Serial.println(COMPLETE);
}

void LedController::turnAllOn() {
  for (byte i = 0; i < ledCount; i++) {
    leds[i].turnOn();
  }
}

void LedController::turnAllOff() {
  for (byte i = 0; i < ledCount; i++) {
    leds[i].turnOff();
  }
}

void LedController::turnOn(byte ledIndex) {
  if (ledIndex >= 0 && ledIndex < ledCount) {
    leds[ledIndex].turnOn();
  }
}

void LedController::turnOff(byte ledIndex) {
  if (ledIndex >= 0 && ledIndex < ledCount) {
    leds[ledIndex].turnOff();
  }
}

void LedController::fade(byte ledIndex, int brightness) {
  if (ledIndex >= 0 && ledIndex < ledCount) {
    leds[ledIndex].fade(brightness);
  }
}

void LedController::blink(byte ledIndex, byte times){
  byte i = 0;
  while(i < times){
    if (ledIndex >= 0 && ledIndex < ledCount) {
      leds[ledIndex].turnOn();
      delay(50);
      leds[ledIndex].turnOff();
      delay(50);
    }
    i++;
  }
}