#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>
#include "Strings.h"

static const int buzzerChannel = 2; // 0 is used by servo

class Buzzer {
public:
    Buzzer();
    ~Buzzer();
    void initializeBuzzer();
    void buzzerSoundOn();
    void buzzerSoundOff();
    void beep(int times, int speed);
private:
    // ----- Buzzer definitions ------
    static const int buzzerPin = 14;
    static const int buzzerFrequency = 5000;
    static const byte buzzerResolution = 8;
    static const byte SILENCE = 0;
};

extern Buzzer buzzer;

#endif
