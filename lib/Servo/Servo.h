#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include "Strings.h"
#include <ESP32Servo.h>

/**
 * @file Servo.h
 * @brief Header class for controlling the servo.
 * @author Ivan Martin Enciso 
 */

extern int initialServoPosition;            ///< Servo initial position
extern int finalServoPosition;              ///< Servo final position
#define PIN_SG90 7                          ///< Servo pin

class ServoController {
public:
    ServoController();
    ~ServoController();

    void initializeServo();
    void testServo();
    void setServoPosition(int position);
    int getServoPosition();
private:
    Servo sg90;                          ///< Object for the servo. 
};

extern ServoController servoController;  ///< Global instance of the servo class.

#endif
