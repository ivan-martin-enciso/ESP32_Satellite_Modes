#ifndef MORSECODECONTROLLER_H
#define MORSECODECONTROLLER_H

#include <Arduino.h>
#include "Utils.h"

/**
 * @file MorseCodeController.h
 * @brief Header class for the Morse Code control.
 * @author Ivan Martin Enciso 
 */
class MorseCodeController {
public:
    MorseCodeController();
    ~MorseCodeController();
    void translateToMorseCode(String input); 

private:
    static const char* morse[];         ///< Array containing Morse code representations for characters.
    static const char letters[];        ///< Array containing Alphabet.

    const int dotDelay = 50;            ///< Duration of a dot in milliseconds.
    const int dashDelay = 150;          ///< Duration of a dash in milliseconds.
    const int wordDelay = 350;          ///< Duration of a pause between words in milliseconds.

    int findLetterIndex(char letter);   
    void playMorse(String morseCode);  
    void symbolPause();                
    void letterPause();              
    void wordPause();                  
    void dot();                         
    void dash();                       

};

extern MorseCodeController morseCodeController; //< Global instance of the Morse Code controller class.

#endif
