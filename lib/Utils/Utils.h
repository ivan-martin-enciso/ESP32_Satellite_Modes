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
#include "TelecommandsManager.h"
#include "ChartDisplay.h"

/**
 * @file Utils.h
 * @brief Header class for common code shared between the modes.
 * @author Ivan Martin Enciso 
 */

// ----- Satellite modes -----
extern int currentMode;                             ///< Current mode of the satellite
extern volatile int nextMode;                       ///< Next mode of the satellite
extern bool volatile isComLoRa;                     ///< Flag indicating communication mode, set by user. 

// ----- Mode 3 -----
extern int volatile mode3Time;                      ///< Mode 3 time
extern bool volatile startTimer;                    ///< Flag indicating if the timer has started
extern volatile unsigned long mode3RemainingTime;   ///< Remaining time for Mode 3
extern volatile int currentServoStep;               ///< Current servo step for Mode 3
extern volatile int servoStepValue;                 ///< Servo step value for Mode 3

// ----- Mode 5 -----
extern bool refreshMode5;                           ///< Flag to refresh Mode 5 data

// ----- Touch Pads -----
static const byte touchDownPin = 1;                 ///< Pin number for the touch down pad
static const byte touchRightPin = 2;                ///< Pin number for the touch right pad
static const byte touchXPin = 3;                    ///< Pin number for the touch X pad
static const byte touchUpPin = 4;                   ///< Pin number for the touch up pad
static const byte touchLeftPin = 5;                 ///< Pin number for the touch left pad
static const int touchThreshold = 40000;            ///< Threshold value for touch sensitivity

// ----- Switch -----
static const int sw1Pin = 0;                        ///< Pin number for switch 1

// ----- Ldr thresholds -----
extern volatile int ldrThreshold1;                  ///< LDR threshold 1 value
extern volatile int ldrThreshold2;                  ///< LDR threshold 2 value
extern volatile int ldrThreshold3;                  ///< LDR threshold 3 value

// ----- Temperature threshold -----
extern volatile int temperatureLowerThreshold;      ///< Lower temperature threshold value
extern volatile int temperatureUpperThreshold;      ///< Upper temperature threshold value

// ----- Functions -----
bool modeChanged();
void handleModeChange();
double readMode5Data();
void initializeBoard();
void handleCommunications();

#endif
