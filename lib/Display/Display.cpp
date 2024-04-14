#include "Display.h"

/**
 * @file Display.cpp
 * @brief Implementation class for the OLED Display functions. 
 * @author Ivan Martin Enciso 
 */

#define WIRE Wire
const byte oledAddress = 0x3C;        ///< OLED Display Address
const byte oledHeight = 32;           ///< OLED Display screen height
const byte oledWidth = 128;           ///< OLED Display screen width
const byte oledReset = -1;            ///< OLED Display reset

/**
 * @brief Bitmap data for the 'Satellite' image, 32x32 pixels.
 */
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

/**
 * @brief Constructor for the Display class.
 */
Display::Display() : display(oledWidth, oledHeight, &WIRE, oledReset){}

/**
 * @brief Destructor for the ChartDisplay class.
 */
Display::~Display() {}

/**
 * @brief Sets the current mode of the display.
 * @param currentMode The current mode to be set.
 */
void Display::setCurrentMode(String currentMode){
  currentModeString = currentMode;
}

/**
 * @brief Prints a string on the OLED display at a given position.
 * @param string The string to be printed.
 * @param y The Y-coordinate position.
 * @param x The X-coordinate position.
 * @param isClearDisplay Flag indicating whether to clear the display before printing.
 */
void Display::printInDisplay(String string, byte y, byte x, bool isClearDisplay) {
  if(isClearDisplay) display.clearDisplay();
  display.setCursor(0, 0);
  display.print(currentModeString);
  display.setCursor(x, y);
  display.print(string);
  display.display();
}

/**
 * @brief Draws a single pixel on the OLED display at the specified position. 
 * @note  Not used in any mode anymore. 
 * @param x The X-coordinate position of the pixel.
 * @param y The Y-coordinate position of the pixel.
 */
void Display::drawInDisplay(int16_t x, int16_t y){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(currentModeString);
  display.drawPixel(x, y, SSD1306_WHITE);
  display.display();
}

/**
 * @brief Draws a bitmap image on the OLED display at the specified position.
 * @param positionX The X-coordinate position 
 * @param positionY The Y-coordinate position
 * @param bitmap The array containing the bitmap image data.
 * @param width The width of the bitmap image.
 * @param height The height of the bitmap image.
 */
void Display::drawBitmap(int16_t positionX, int16_t positionY, unsigned char bitmap [], int width, int height){
  display.drawBitmap(positionX, positionY, bitmap, width, height, SSD1306_WHITE);
  display.display();
}

/**
 * @brief Updates the progress bar for the initialization process. 
 * @param percentage The percentage value to be displayed on the progress bar.
 */
void Display::updateProgressBar(int percentage) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(BOARD_SETUP);
  display.setCursor(0, 20);
  int numDashes = map(percentage, 0, 100, 0, (oledWidth - 60)/5);

  for (int i = 0; i < numDashes; ++i) {
    display.print("-");
  }
  display.print(">");
  display.print(percentage);
  display.print("%");
  display.display();
  delay(100);
}

/**
 * @brief Displays the welcome message and a satellite bitmap.
 */
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

/**
 * @brief Draws the satellite bitmap on the OLED display at the specified position.
 * @note Used for Mode 1.
 * @param positionX The x-coordinate.
 * @param positionY The y-coordinate.
 * @param bitmap[] The bitmap array representing the satellite bitmap.
 * @param width The width of the satellite bitmap.
 * @param height The height of the satellite bitmap.
 */
void Display::drawSatellite(int16_t positionX, int16_t positionY, unsigned char bitmap [], int width, int height){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(currentModeString);
  drawBitmap(positionX, positionY, bitmap, width, height);
}

/**
 * @brief Initializes the OLED display.
 */
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
  delay(100);
  display.clearDisplay();
  display.display();
  Serial.println(COMPLETE);
}

/**
 * @brief Clears the OLED display.
 */
void Display::clearDisplay(){
  display.clearDisplay();
  display.display();
}

/**
 * @brief Fills the OLED display.
 */
void Display::fillScreen() {
  display.fillScreen(SSD1306_WHITE);
  display.display();
}

/**
 * @brief Prompt message to Choose LoRa communication.
 */
void Display::promptLoRa(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Choose communication:");
  display.setCursor(10,10);
  display.print("LoRa <--");
  display.setCursor(70,10);
  display.print("WiFi");
  display.setCursor(0,20);
  display.print("Press X to confirm");
  display.display();
}

/**
 * @brief Prompt message to Choose WiFi communication.
 */
void Display::promptWiFi(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Choose communication:");
  display.setCursor(10,10);
  display.print("LoRa");
  display.setCursor(70,10);
  display.print("WiFi <--");
  display.setCursor(0,20);
  display.print("Press X to confirm");
  display.display();
}

/**
 * @brief Returns the width of the bounding box for the input text.
 *
 * @param text The text for which the bounding box width needs to be calculated.
 * @return The width of the bounding box for the text.
 */
int Display::getTextBounds(String text){
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return w;
}

