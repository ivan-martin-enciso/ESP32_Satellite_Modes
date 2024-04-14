#include "ChartDisplay.h"

/**
 * @file ChartDisplay.cpp
 * @brief Implementation class for displaying charts in the OLED Display. 
 * @author Ivan Martin Enciso 
 */

#define SCREEN_WIDTH 128              ///< OLED display width, in pixels
#define SCREEN_HEIGHT 32              ///< OLED display height, in pixels
#define OLED_RESET -1                 ///< Reset pin 
#define BAUDRATE 115200               ///< Serial Baud Rate
#define SDA_PIN D1                    ///< SDA Pin
#define SCL_PIN D2                    ///< SCL Pin

// Definitions 
volatile bool startMode5 = false;     ///< Global variable to start mode 5. 
String minYLimitLabel = "0";          ///< Global variable for the minimum Y-axis limit label. 
String maxYLimitLabel = "5000";       ///< Global variable for the maximum Y-axis limit label. 
double minYLimitValue = 0.0;          ///< Global variable for the minimum Y-axis limit value. 
double maxYLimitValue = 5000.0;       ///< Global variable for the maximum Y-axis limit value.
String reading = "LDR";               ///< Global variable for the current reading. 
const byte oledAddress = 0x3C;        ///< OLED Address

/**
 * @brief Constructor for the ChartDisplay class.
 */
ChartDisplay::ChartDisplay() : chartDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET){}

/**
 * @brief Destructor for the ChartDisplay class.
 */
ChartDisplay::~ChartDisplay() {}

/**
 * @brief Sets the labels and values for the Y-axis limits on the chart.
 * @details This method sets the labels and values for the Y-axis limits on the chart display.
 */
void ChartDisplay::setChartLabels(){
  char charArray1[minYLimitLabel.length() + 1]; 
  minYLimitLabel.toCharArray(charArray1, sizeof(charArray1));
  char charArray2[maxYLimitLabel.length() + 1]; 
  maxYLimitLabel.toCharArray(charArray2, sizeof(charArray2));
  chartDisplay.setYLimitLabels(charArray1, charArray2);    
  chartDisplay.setYLabelsVisible(true);
  chartDisplay.setYLimits(minYLimitValue, maxYLimitValue);
}

/**
 * @brief Sets the current mode string.
 * @param currentMode The string representing the current mode, always M5 for this case.
 */
void ChartDisplay::setCurrentMode(String currentMode){
  currentModeString = currentMode;
}

/**
 * @brief Initializes the OLED chart display.
 * @details This method initializes another instance of the OLED display using a different library specific for graphics.
 *          It then sets all the necessary x and y values for the chart coordinates, the Oled size boundries and the labels. 
 *          Also sets the thickness and displays the chart. 
 */
void ChartDisplay::initializeChartDisplay() {
  if(startMode5){
    Serial.print(INITIALIZE_CHART_DISPLAY);
    Wire.begin();
    bool status = chartDisplay.begin(SSD1306_SWITCHCAPVCC, oledAddress);
    if (!status) {
      Serial.println(CHART_DISPLAY_SETUP_ERROR);
      while (1)
        delay(10);
    }
    chartDisplay.clearDisplay();
    chartDisplay.setRotation(2);
    chartDisplay.setChartCoordinates(0, 28);     
    chartDisplay.setChartWidthAndHeight(123, 26); 
    chartDisplay.setXIncrement(10);              
    setChartLabels();
    chartDisplay.setAxisDivisionsInc(12, 6);    
    chartDisplay.setPlotMode(SINGLE_PLOT_MODE); 
    chartDisplay.setPointGeometry(POINT_GEOMETRY_CIRCLE);
    actualThickness = NORMAL_LINE;
    chartDisplay.setLineThickness(actualThickness);
    chartDisplay.drawChart(); 
    chartDisplay.display();
    Serial.println(COMPLETE);
    startMode5 = false;
  }
}

/**
 * @brief Displays a value on the chart.
 * @details This method updates the chart display with a new value and if the chart has reach the end of the OLED screen it draws it again.
 * @param value The value to be displayed on the chart.
 */
void ChartDisplay::displayChart(double value){
  if (!chartDisplay.updateChart(value)) 
  {
    chartDisplay.clearDisplay(); 
    if (actualThickness == NORMAL_LINE)
    {
      actualThickness = LIGHT_LINE;
    }
    else if (actualThickness == LIGHT_LINE)
    {
      actualThickness = NORMAL_LINE;
    }
    setChartLabels();
    chartDisplay.setLineThickness(actualThickness);
    chartDisplay.drawChart();
  }
}
