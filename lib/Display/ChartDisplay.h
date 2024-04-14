#ifndef CHARTDISPLAY_H
#define CHARTDISPLAY_H

#include <Arduino.h>
#include <OLED_SSD1306_Chart.h>
#include <Adafruit_I2CDevice.h>
#include "Strings.h"

/**
 * @file ChartDisplay.h
 * @brief Header class for displaying charts in the OLED Display. 
 * @author Ivan Martin Enciso 
 */

extern volatile bool startMode5;  ///< External variable to start mode 5. 
extern String minYLimitLabel;     ///< External variable for the minimum Y-axis limit label. 
extern String maxYLimitLabel;     ///< External variable for the maximum Y-axis limit label. 
extern double minYLimitValue;     ///< External variable for the minimum Y-axis limit value. 
extern double maxYLimitValue;     ///< External variable for the maximum Y-axis limit value. 
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
  OLED_SSD1306_Chart chartDisplay;            ///< OLED display object for chart display. 
  char actualThickness;                       ///< Character for the thickness of the line. 
  String currentModeString;                   ///< String for the current mode. 
};

extern ChartDisplay chartDisplay;            ///< Global instance for Chart Display class.

#endif
