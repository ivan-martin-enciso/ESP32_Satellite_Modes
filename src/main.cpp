/* This is an example multi-file structure for the SE project
 * 
 *
 * While the separation into a file per mode should be kept, 
 * all the code within this example can be changed. 
 * While the simple structure to managing the tasks shown here 
 * should be sufficient to implement the required project work 
 * it should not be understood as an optimal solution at all.
 * There are a number of well-studied approaches to multi-tasking 
 * on a single core that should probably be used instead for 
 * real-life applications instead of re-inventing the wheel ;)
 */

#include <Arduino.h>
#include <Utils.h>
#include <Mode0.h>
#include <Mode1.h>
#include <Mode2.h>
#include <Mode3.h>
#include <Mode4.h>
#include <Mode5.h>

typedef void (*ModeFunction)();
ModeFunction modeFunctions[] = {mode0, mode1, mode2, mode3, mode4, mode5};

void setup() {
  // Pins 35-37 which are used for the LEDs, are also connected to the internal octal SPI (OSPI) interface which is used for PSRAM.
  // Since we don't need the PSRAM in our project, we disable it via the chip select pin on GPIO 26.
  // This helps to prevent side effects with other libraries, which might result in odd behavior. 
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);

  Serial.begin(115200);
  delay(1000);
  initializeBoard();
}


void loop() {
  if( nextMode != currentMode ){
    currentMode = nextMode;
    handleModeChange();
  }
  if (currentMode >= 0 && currentMode < sizeof(modeFunctions) / sizeof(modeFunctions[0])) {
    modeFunctions[currentMode]();
  }
  if(sendTelemetry){
    String telemetryData = collectHousekeepingData();
    comsManager.sendTelemetryData(telemetryData);
  }
  if(receivedFlag) {
    String receivedCommand = comsManager.receivePackage();
  }
}
