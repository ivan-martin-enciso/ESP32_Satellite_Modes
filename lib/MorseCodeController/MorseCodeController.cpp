#include "MorseCodeController.h"

const char* MorseCodeController::morse[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--",
    "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-",
    "--..--", "..--.."
};

const char MorseCodeController::letters[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', ',', '?'
};

MorseCodeController::MorseCodeController() {}

MorseCodeController::~MorseCodeController() {}

void MorseCodeController::translateToMorseCode(String input) {
  input.toUpperCase();
  for (int i = 0; i < input.length(); i++) {
    if(modeChanged()) return;
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

int MorseCodeController::findLetterIndex(char letter) {
  for (int i = 0; i < (sizeof(letters) / sizeof(letters[0])); i++) {
    if(modeChanged()) break;
    if (letter == letters[i]) {
      return i;
    }
  }
  return -1;
}

void MorseCodeController::playMorse(String morseCode) {
  for (int i = 0; i < morseCode.length(); i++) {
    if(modeChanged()) return;
    if (morseCode.charAt(i) == '.') {
      dot();
    } else {
      dash();
    }
  }
}

void MorseCodeController::symbolPause() {
  delay(dotDelay);
}

void MorseCodeController::letterPause() {
  delay(dashDelay);
}
void MorseCodeController::wordPause() {
  delay(wordDelay);
}

void MorseCodeController::dot(){
  lcdDisplay.fillScreen();   
  //buzzer.buzzerSoundOn(); 
  symbolPause();            
  lcdDisplay.clearDisplay();      
  //buzzer.buzzerSoundOff(); 
  symbolPause();                
}
void MorseCodeController::dash(){
  lcdDisplay.fillScreen();   
  //buzzer.buzzerSoundOn();   
  letterPause();               
  lcdDisplay.clearDisplay();      
  //buzzer.buzzerSoundOff();    
  letterPause();               
}
