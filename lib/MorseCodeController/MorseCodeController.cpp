#include "MorseCodeController.h"

/**
 * @file MorseCodeController.cpp
 * @brief Implementation class for the Morse Code control.
 * @author Ivan Martin Enciso 
 */

const char* MorseCodeController::morse[] = {                                    ///< Array containing Morse code representations for characters.
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--",
    "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-",
    "--..--", "..--.."
};

const char MorseCodeController::letters[] = {                                   ///< Array containing Alphabet.
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', ',', '?'
};

/**
 * @brief Checks if the mode has changed.
 * 
 * @return If the mode changes, check communications manager for telemetry and telecommands.
 */
bool hasModeChanged(){
  handleCommunications();
  return modeChanged(); 
}

/**
 * @brief Constructor for MorseCodeController class.
 */
MorseCodeController::MorseCodeController() {}

/**
 * @brief Destructor for MorseCodeController class.
 */
MorseCodeController::~MorseCodeController() {}

/**
 * @brief Translates the input string into Morse code and calls the playMorse function.
 * @details For each character in the received string, translates the character to morse and plays it. 
 *          The function takes into account the word and letter pause using Morse standards. 
 * @param input The input string to be translated.
 */
void MorseCodeController::translateToMorseCode(String input) {
  input.toUpperCase();
  for (int i = 0; i < input.length(); i++) {
    if(hasModeChanged()) return;
    char letter = input.charAt(i);
    int index = findLetterIndex(letter);
    if (index != -1) {
      String morseCode = morse[index];
      playMorse(morseCode);
    } 
    else {
      wordPause();
    }
    letterPause();
  }
}

/**
 * @brief Finds the index of the received letter in the Morse code alphabet.
 * 
 * @param letter The letter to find the index for.
 * @return The index of the letter in the Morse code alphabet, or -1 if not found.
 */
int MorseCodeController::findLetterIndex(char letter) {
  for (int i = 0; i < (sizeof(letters) / sizeof(letters[0])); i++) {
    if(hasModeChanged()) break;
    if (letter == letters[i]) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief Plays Morse code.
 * 
 * @details For each of the characters of the Morse Code received, it plays each individualy checking if 
 *          it is a dash or a dot. 
 * 
 * @param morseCode The Morse code to be played.
 */
void MorseCodeController::playMorse(String morseCode) {
  for (int i = 0; i < morseCode.length(); i++) {
    if(hasModeChanged()) return;
    if (morseCode.charAt(i) == '.') {
      dot();
    } else {
      dash();
    }
  }
}

/**
 * @brief Pauses between symbols.
 */
void MorseCodeController::symbolPause() {
  delay(dotDelay);
}

/**
 * @brief Pauses between letters.
 */
void MorseCodeController::letterPause() {
  delay(dashDelay);
}

/**
 * @brief Pauses between words.
 */
void MorseCodeController::wordPause() {
  delay(wordDelay);
}

/**
 * @brief Plays a dot in Morse code using the fillScreen of the OLED and also using the buzzer. 
 */
void MorseCodeController::dot(){
  lcdDisplay.fillScreen();   
  buzzer.buzzerSoundOn(); 
  symbolPause();            
  lcdDisplay.clearDisplay();      
  buzzer.buzzerSoundOff(); 
  symbolPause();                
}

/**
 * @brief Plays a dash in Morse code using the fillScreen of the OLED and also using the buzzer. 
 */
void MorseCodeController::dash(){
  lcdDisplay.fillScreen();   
  buzzer.buzzerSoundOn();   
  letterPause();               
  lcdDisplay.clearDisplay();      
  buzzer.buzzerSoundOff();    
  letterPause();               
}
