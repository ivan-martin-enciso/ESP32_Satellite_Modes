#ifndef CHARTDISPLAY_H
#define CHARTDISPLAY_H

#include <Arduino.h>
#include <OLED_SSD1306_Chart.h>
#include <Adafruit_I2CDevice.h>
#include "Strings.h"

extern volatile bool startMode5;
class ChartDisplay {
public:
  ChartDisplay();
  ~ChartDisplay();
  void initializeChartDisplay();
  void displayChart(double value);
  void setCurrentMode(String currentMode);
  
private:
  OLED_SSD1306_Chart chartDisplay;
  char actualThickness;
  String currentModeString;
};

extern ChartDisplay chartDisplay; 

#endif
