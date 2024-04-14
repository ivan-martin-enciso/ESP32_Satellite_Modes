#include "Servo.h"

/**
 * @file Servo.cpp
 * @brief Implementation class for controlling the servo.
 * @author Ivan Martin Enciso 
 */

int finalServoPosition = 180;                       ///< Servo final position.
int initialServoPosition = 0;                       ///< Servo initial position.

/**
 * @brief Constructor for ServoController class.
 */
ServoController::ServoController() : sg90() {}

/**
 * @brief Destructor for ServoController class.
 */
ServoController::~ServoController() {}

/**
 * @brief Test function to check the servo range (90 degrees to -90 degrees).
 */
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

/**
 * @brief Initializes the servo.
 * @details Uses 50 Hz frequency and Minimum and maximum PWM (in µs) to go from -90° to 90°.
 *          It also calls the testServo function. 
 */
void ServoController::initializeServo() {
    Serial.print(INITIALIZE_SERVO);
    sg90.setPeriodHertz(50);               
    sg90.attach(PIN_SG90, 400, 2350);       
    testServo();
    Serial.println(COMPLETE);
}
/**
 * @brief Sets the position of the servo.
 * 
 * @param position The desired position of the servo motor.
 */
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

/**
 * @brief Retrieves the current position of the servo.
 * 
 * @return The current position of the servo.
 */
int ServoController::getServoPosition() {
    return sg90.read();
}
