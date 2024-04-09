#include "Buzzer.h"

Buzzer::Buzzer() {}

Buzzer::~Buzzer() {}

void Buzzer::initializeBuzzer() {
    Serial.print(INITIALIZE_BUZZER);
    ledcSetup(buzzerChannel, buzzerFrequency, buzzerResolution);
    ledcAttachPin(buzzerPin, buzzerChannel);
    beep(3, 50);
    Serial.println(COMPLETE);
}

void Buzzer::buzzerSoundOn() {
    ledcWriteTone(buzzerChannel, 523);
}
 
void Buzzer::buzzerSoundOff() {
    ledcWrite(buzzerChannel, SILENCE);   // Turn off the buzzer
}

void Buzzer::beep(int times, int speed){
    for(int i =  0; i < times; i++){
        buzzerSoundOn();
        delay(speed);
        buzzerSoundOff();
        delay(speed);
    }
}
