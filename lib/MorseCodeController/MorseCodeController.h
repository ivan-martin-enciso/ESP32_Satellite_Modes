#ifndef MORSECODECONTROLLER_H
#define MORSECODECONTROLLER_H

#include <Arduino.h>
#include "Utils.h"

class MorseCodeController {
public:
    MorseCodeController();
    ~MorseCodeController();
    void translateToMorseCode(String input); 

private:
    static const char* morse[];
    static const char letters[];

    const int dotDelay = 50;            
    const int dashDelay = 150;           
    const int wordDelay = 350; 

    int findLetterIndex(char letter);
    void playMorse(String morseCode);      
    void symbolPause();
    void letterPause();
    void wordPause();
    void dot();
    void dash();
};

extern MorseCodeController morseCodeController;

#endif
