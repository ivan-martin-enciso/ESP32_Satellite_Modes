#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include "Strings.h"
#include <ESP32Servo.h>

extern int initialServoPosition;
extern int finalServoPosition;
#define PIN_SG90 7 

class ServoController {
public:
    ServoController();
    ~ServoController();

    void initializeServo();
    void testServo();
    void setServoPosition(int position);
    int getServoPosition();
private:
    Servo sg90;
};

extern ServoController servoController;

#endif
