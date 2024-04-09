#include "Servo.h"

int finalServoPosition = 180;
int initialServoPosition = 0;

ServoController::ServoController() : sg90() {}

ServoController::~ServoController() {}

void ServoController::testServo() {
    // Rotation from 0 to 180°.
    for (int pos = initialServoPosition; pos <= finalServoPosition; pos += 1) {
        sg90.write(pos);
        delayMicroseconds(1000);
    }
    delay(100);
    // Rotation from 180° to 0.
    for (int pos = finalServoPosition; pos >= initialServoPosition; pos -= 1) {
        sg90.write(pos);
        delayMicroseconds(1000);
    }
    delay(100);
}

void ServoController::initializeServo() {
    Serial.print(INITIALIZE_SERVO);
    sg90.setPeriodHertz(50);                // PWM frequency for SG90
    sg90.attach(PIN_SG90, 400, 2350);       // Minimum and maximum pulse width (in µs) to go from 0° to 180°.
    testServo();
    Serial.println(COMPLETE);
}

void ServoController::setServoPosition(int position) {
    position = constrain(position, initialServoPosition, finalServoPosition);
    int currentServoPosition = getServoPosition();

    if (position > currentServoPosition) {
        // Increase servo position
        for (int pos = currentServoPosition; pos <= position; pos += 1) {
            sg90.write(pos);
            delayMicroseconds(1000);
        }
    } else if (position < currentServoPosition) {
        // Decrease servo position
        for (int pos = currentServoPosition; pos >= position; pos -= 1) {
            sg90.write(pos);
            delayMicroseconds(1000);
        }
    }
}

int ServoController::getServoPosition() {
    return sg90.read();
}
