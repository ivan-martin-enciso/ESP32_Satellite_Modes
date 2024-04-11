#include "ChartDisplay.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define BAUDRATE 115200
#define SDA_PIN D1
#define SCL_PIN D2
volatile bool startMode5 = false;
const byte oledAddress = 0x3C;

ChartDisplay::ChartDisplay() : chartDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET){}

ChartDisplay::~ChartDisplay() {}

void ChartDisplay::setCurrentMode(String currentMode){
  currentModeString = currentMode;
}

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
    chartDisplay.setChartCoordinates(0, 28);      //Chart lower left coordinates (X, Y)
    chartDisplay.setChartWidthAndHeight(123, 26); //Chart width = 123 and height = 60
    chartDisplay.setXIncrement(5);                //Distance between Y points will be 5px
    chartDisplay.setYLimits(0, 100);             //Ymin = 0 and Ymax = 100
    chartDisplay.setYLimitLabels("0", "100");    //Setting Y axis labels
    chartDisplay.setYLabelsVisible(true);
    chartDisplay.setAxisDivisionsInc(12, 6);    //Each 12 px a division will be painted in X axis and each 6px in Y axis
    chartDisplay.setPlotMode(SINGLE_PLOT_MODE); //Set single plot mode
    chartDisplay.setPointGeometry(POINT_GEOMETRY_CIRCLE);
    actualThickness = NORMAL_LINE;
    chartDisplay.setLineThickness(actualThickness);
    chartDisplay.drawChart(); 
    chartDisplay.display();
    Serial.println(COMPLETE);
    startMode5 = false;
  }
}

void ChartDisplay::displayChart(double value){
  if (!chartDisplay.updateChart(value)) //Value between Ymin and Ymax will be added to chart
  {
    chartDisplay.clearDisplay(); //If chart is full, it is drawn again
    if (actualThickness == NORMAL_LINE)
    {
      actualThickness = LIGHT_LINE;
    }
    else if (actualThickness == LIGHT_LINE)
    {
      actualThickness = NORMAL_LINE;
    }
    chartDisplay.setLineThickness(actualThickness);
    chartDisplay.drawChart();
  }
}
