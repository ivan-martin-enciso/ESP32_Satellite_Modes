#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include "Strings.h"

class Display {
public:
  Display();
  ~Display();

  void setCurrentMode(String currentMode);
  void printInDisplay(String string, byte y = 10, byte x = 0, bool isClearDisplay = true);
  void drawInDisplay(int16_t x, int16_t y);
  void drawBitmap(int16_t positionX, int16_t positionY, unsigned char bitmap [], int width, int height);
  void drawSatellite(int16_t positionX, int16_t positionY, unsigned char bitmap [], int width, int height);
  void drawMainScreen();
  void updateProgressBar(int percentage);
  void initializeDisplay();
  void clearDisplay();
  void fillScreen();
  void promptLoRa();
  void promptWiFi();
private:
  Adafruit_SSD1306 display;
  String currentModeString;
};

extern Display lcdDisplay; 

#endif
