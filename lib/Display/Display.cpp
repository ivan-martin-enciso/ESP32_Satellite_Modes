#include "Display.h"

#define WIRE Wire
const byte oledAddress = 0x3C;
const byte oledHeight = 32;
const byte oledWidth = 128;
const byte oledReset = -1;

// Bitmaps
// 'Satellite', 32x32px
unsigned char satellite [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 
	0xf3, 0x07, 0xff, 0xff, 0xf4, 0x21, 0xff, 0xff, 0xe4, 0x02, 0x7f, 0xff, 0xe0, 0x03, 0x1f, 0xff, 
	0xe0, 0x43, 0xe7, 0xff, 0xe0, 0x03, 0xf9, 0xff, 0xf0, 0x83, 0xfe, 0xff, 0xf2, 0x01, 0xff, 0xff, 
	0xf8, 0x00, 0xff, 0xff, 0xfc, 0x0e, 0x3f, 0xff, 0xfe, 0x3f, 0x9f, 0xff, 0xff, 0x9f, 0xe7, 0xff, 
	0xff, 0xcf, 0xfb, 0xff, 0xff, 0xe7, 0xfd, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 
	0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

Display::Display() : display(oledWidth, oledHeight, &WIRE, oledReset){}

Display::~Display() {}

void Display::setCurrentMode(String currentMode){
  currentModeString = currentMode;
}

void Display::printInDisplay(String string, byte y, byte x, bool isClearDisplay) {
  if(isClearDisplay) display.clearDisplay();
  display.setCursor(0, 0);
  display.print(currentModeString);
  display.setCursor(x, y);
  display.print(string);
  display.display();
}

void Display::drawInDisplay(int16_t x, int16_t y){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(currentModeString);
  display.drawPixel(x, y, SSD1306_WHITE);
  display.display();
}

void Display::drawBitmap(int16_t positionX, int16_t positionY, unsigned char bitmap [], int width, int height){
  display.drawBitmap(positionX, positionY, bitmap, width, height, SSD1306_WHITE);
  display.display();
}

void Display::updateProgressBar(int percentage) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(BOARD_SETUP);
  display.setCursor(0, 20);
  int numDashes = map(percentage, 0, 100, 0, (oledWidth - 60)/5); // 20: space for other characters

  for (int i = 0; i < numDashes; ++i) {
    display.print("-");
  }
  display.print(">");
  display.print(percentage);
  display.print("%");
  display.display();
  delay(100);
}

void Display::drawMainScreen(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Welcome to");
  display.setCursor(0, 10);
  display.print("  Ivan's");
  display.setCursor(0, 20);
  display.print("Satellite");
  drawBitmap(oledWidth - 32, 0, satellite, 32, 32);
  delay(1500);
}

void Display::drawSatellite(int16_t positionX, int16_t positionY, unsigned char bitmap [], int width, int height){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(currentModeString);

  drawBitmap(positionX, positionY, bitmap, width, height);
}

void Display::initializeDisplay() {
  Serial.print(INITIALIZE_DISPLAY);
  bool status = display.begin(SSD1306_SWITCHCAPVCC, oledAddress);
  if (!status) {
    Serial.println(DISPLAY_SETUP_ERROR);
    while (1)
      delay(10);
  }
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(2);
  display.display();
  delay(1000);

  display.clearDisplay();
  display.display();
  Serial.println(COMPLETE);
}

void Display::clearDisplay(){
  display.clearDisplay();
  display.display();
}

void Display::fillScreen() {
  display.fillScreen(SSD1306_WHITE);
  display.display();
}

void Display::drawMode5(float roll, float pitch){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(currentModeString);
  display.drawLine(oledWidth / 2, oledHeight / 2, oledWidth / 2 + roll, oledHeight / 2 + pitch, SSD1306_WHITE);
  display.display();
}
