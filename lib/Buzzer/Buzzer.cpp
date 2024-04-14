#include "Buzzer.h"

/**
 * @file Buzzer.cpp
 * @brief Implementation class for the Buzzer.
 * @author Ivan Martin Enciso
 */

/**
 * @brief Constructor for the Buzzer class.
 */
Buzzer::Buzzer() {}

/**
 * @brief Destructor for the Buzzer class.
 */
Buzzer::~Buzzer() {}

/**
 * @brief Initializes the buzzer.
 * @details This function initializes the buzzer channel, frequency and resolution. Attaches the buzzer pin,
 *          and tests the buzzer by beeping 3 times.
 */
void Buzzer::initializeBuzzer() {
    Serial.print(INITIALIZE_BUZZER);
    ledcSetup(buzzerChannel, buzzerFrequency, buzzerResolution);
    ledcAttachPin(buzzerPin, buzzerChannel);
    beep(3, 50); 
    Serial.println(COMPLETE);
}

/**
 * @brief Turns on the buzzer sound with a tone of 523 Hz (C5).
 */
void Buzzer::buzzerSoundOn() {
    ledcWriteTone(buzzerChannel, 523); 
}
 
/**
 * @brief Turns off the buzzer sound.
 */
void Buzzer::buzzerSoundOff() {
    ledcWrite(buzzerChannel, SILENCE); 
}

/**
 * @brief Generates a beeping sound for a number of times and for a specified duration.
 * @param times The number of times to beep.
 * @param speed The duration of each beep in milliseconds.
 */
void Buzzer::beep(int times, int speed){
    for(int i =  0; i < times; i++){
        buzzerSoundOn(); 
        delay(speed);   
        buzzerSoundOff(); 
        delay(speed);    
    }
}
