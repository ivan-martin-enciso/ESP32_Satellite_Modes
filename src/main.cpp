#include <Arduino.h>
#include <Utils.h>
#include <Mode0.h>
#include <Mode1.h>
#include <Mode2.h>
#include <Mode3.h>
#include <Mode4.h>
#include <Mode5.h>

/**
 * @file main.cpp
 * @brief Main file for the project.
 * @author Ivan Martin Enciso 
 */

/**
 * @brief Definitions to call mode functions in a reflective manner.
 */
typedef void (*ModeFunction)();                                               ///< Type definition for array of mode functions.
ModeFunction modeFunctions[] = {mode0, mode1, mode2, mode3, mode4, mode5};    ///< Array of function pointers to mode functions.

/**
 * @brief Setup function, executed once at the beginning of the program.
 * 
 * @details Pins 35-37, used for the LEDs, are also connected to the internal octal SPI (OSPI) interface,
 *          which is used for PSRAM. Since PSRAM is not needed in the project, it is disabled via the chip
 *          select pin on GPIO 26 to prevent side effects with other libraries.
 *          Serial communication is initialized at 115200 baud rate, and the board is initialized.
 */
void setup() {
  pinMode(26, OUTPUT);    // Disable PSRAM via chip select pin on GPIO 26
  digitalWrite(26, HIGH); // Set PSRAM chip select pin HIGH to disable PSRAM
  Serial.begin(115200);   // Initialize serial communication
  delay(1000);            // Delay for stability
  initializeBoard();      // Initialize the board
}

/**
 * @brief Loop function, executes endlessly after setup().
 * 
 * @details Checks if the next mode is different from the current mode. 
 *          If so, updates the current mode and handles the mode change. 
 *          Then executes the current mode function if it is within range.
 *          Finally it checks for telemetry and telecommand data.
 */
void loop() {
  if (nextMode != currentMode) {  
    currentMode = nextMode;       
    handleModeChange();            
  }
  if (currentMode >= 0 && currentMode < sizeof(modeFunctions) / sizeof(modeFunctions[0])) {
    modeFunctions[currentMode](); 
  }
  handleCommunications();     
}
