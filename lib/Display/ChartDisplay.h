#ifndef CHARTDISPLAY_H
#define CHARTDISPLAY_H

#include <Arduino.h>
#include <OLED_SSD1306_Chart.h>
#include <Adafruit_I2CDevice.h>
#include "Strings.h"

extern volatile bool startMode5;
extern String minYLimitLabel;
extern String maxYLimitLabel;
extern double minYLimitValue;
extern double maxYLimitValue;
extern String reading;
class ChartDisplay {
public:
  ChartDisplay();
  ~ChartDisplay();
  void initializeChartDisplay();
  void displayChart(double value);
  void setCurrentMode(String currentMode);
  void setChartLabels();
private:
  OLED_SSD1306_Chart chartDisplay;
  char actualThickness;
  String currentModeString;
};

extern ChartDisplay chartDisplay; 

#endif
