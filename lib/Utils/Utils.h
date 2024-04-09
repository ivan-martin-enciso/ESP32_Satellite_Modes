/* Use this file for common code that's shared between the modes
 * 
 * If necessary for better readability, you can also add additional 
 * files for common code (e.g. one contains all I/O related functions and 
 * another contains the sensor readout functionality)
 */
#ifndef UTILS_H
#define UTILS_H
#include "Leds.h"
#include "Display.h"
#include "Strings.h"
#include "IMU.h"
#include "BME280.h"
#include "ComsManager.h"
#include "Buzzer.h"
#include "StorageManager.h"
#include "Servo.h"
#include "Ldr.h"
#include <esp32-hal-timer.h>

// ----- Satellite modes -----
extern int currentMode;   
extern volatile int nextMode;
bool modeChanged();
void handleModeChange();

// ----- Board Initialization -----
void initializeBoard();

// ----- Touch Pads -----
static const byte touchDownPin = 1;
static const byte touchRightPin = 2;
static const byte touchXPin = 3;
static const byte touchUpPin = 4;
static const byte touchLeftPin = 5;
static const int touchThreshold = 40000;

// ------ rest of switches  ------
static const int sw1Pin = 0;

// ----- Ldr thresholds -----
extern volatile int ldrThreshold1;
extern volatile int ldrThreshold2;
extern volatile int ldrThreshold3;

// ----- Temperature threshold -----
extern volatile int temperatureLowerThreshold;
extern volatile int temperatureUpperThreshold;

// ----- Mode 3 -----
extern int volatile mode3Time;
extern bool volatile startTimer;
extern volatile unsigned long mode3RemainingTime;
extern volatile int currentServoStep;
extern volatile int servoStepValue;

// ----- Housekeeping -----
String collectHousekeepingData();

#endif