#include <Leds.h>

/**
 * @file Leds.cpp
 * @brief Implementation class for the LEDs control.
 * @author Ivan Martin Enciso 
 */

byte ledsPins [] = {35, 36, 37};                    ///< Array for LEDs GPIOS (Green, Yellow, Red)
int ledFrequencies [] ={5000, 5000, 5000};          ///< Array for LEDs frequencies
byte ledChannels [] = {3, 4, 5};                    ///< Array for LEDs channels 
#define RESOLUTION 8                                ///< Resolution
#define TOTAL_LEDS 3                                ///< Number of LEDs 
LedController ledController(TOTAL_LEDS);            ///< LED controller object
byte greenLed = 0;                                  ///< Green LED pin
byte yellowLed = 1;                                 ///< Yellow LED pin
byte redLed = 2;                                    ///< Red LED pin

/**
 * @brief Initializes a signle LED.
 */
void Led::initialize() {
  ledcSetup(ledChannel, ledFrequency, RESOLUTION);   ///< Channel, frequency, Resolution
  ledcAttachPin(ledPin, ledChannel);                 ///< Pin, Channel
}

/**
 * @brief Turns on a signle LED.
 */
void Led::turnOn() {
  ledcWrite(ledChannel, 255); // 100% duty cycle - full brightness
}

/**
 * @brief Turns off a signle LED.
 */
void Led::turnOff() {
  ledcWrite(ledChannel, 0); // 0% duty cycle - off
}

/**
 * @brief Fades a signle LED.
 * @param dutyCycle The duty cycle for fading.
 */
void Led::fade(int dutyCycle) {
  ledcWrite(ledChannel, dutyCycle); // Set duty cycle for fading
}

/**
 * @brief Constructor for the LedController class.
 * @param ledCount The number of LEDs.
 */
LedController::LedController(byte ledCount) {
  this->ledCount = ledCount;
  leds = new Led[ledCount];
}

/**
 * @brief Destructor for the LedController class.
 */
LedController::~LedController() {
  delete[] leds;
}

/**
 * @brief Initializes all LEDs.
 * @details For each initialization it blinks the led 1 time. 
 */
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

/**
 * @brief Turns on all LEDs.
 */
void LedController::turnAllOn() {
  for (byte i = 0; i < ledCount; i++) {
    leds[i].turnOn();
  }
}

/**
 * @brief Turns off all LEDs.
 */
void LedController::turnAllOff() {
  for (byte i = 0; i < ledCount; i++) {
    leds[i].turnOff();
  }
}

/**
 * @brief Turns on a specific LED.
 * @param ledIndex The index of the LED to turn on.
 */
void LedController::turnOn(byte ledIndex) {
  if (ledIndex >= 0 && ledIndex < ledCount) {
    leds[ledIndex].turnOn();
  }
}

/**
 * @brief Turns off a specific LED.
 * @param ledIndex The index of the LED to turn off.
 */
void LedController::turnOff(byte ledIndex) {
  if (ledIndex >= 0 && ledIndex < ledCount) {
    leds[ledIndex].turnOff();
  }
}

/**
 * @brief Fades a specific LED.
 * @param ledIndex The index of the LED to fade.
 * @param brightness The brightness level for fading.
 */
void LedController::fade(byte ledIndex, int brightness) {
  if (ledIndex >= 0 && ledIndex < ledCount) {
    leds[ledIndex].fade(brightness);
  }
}

/**
 * @brief Blinks a specific LED.
 * @param ledIndex The index of the LED to blink.
 * @param times The number of times to blink.
 */
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
