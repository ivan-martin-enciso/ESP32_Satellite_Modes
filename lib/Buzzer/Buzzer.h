#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>
#include "Strings.h"
/**
 * @file Buzzer.h
 * @brief Header class for the buzzer.
 * @author Ivan Martin Enciso 
 */
static const int buzzerChannel = 2; ///< Channel for the buzzer set to 2. 
/**
 * @brief Constructor for the buzzer class.
 */
class Buzzer {
public:
    Buzzer();
    ~Buzzer();
    void initializeBuzzer();
    void buzzerSoundOn();
    void buzzerSoundOff();
    void beep(int times, int speed);
private:
    static const int buzzerPin = 14; ///< Pin 14 connected to the buzzer.
    static const int buzzerFrequency = 5000; ///< Frequency of the buzzer sound.
    static const byte buzzerResolution = 8; ///< Resolution of the buzzer.
    static const byte SILENCE = 0; ///< Silence value for the buzzer.
};

extern Buzzer buzzer; ///< Global instance of the Buzzer class.

#endif
