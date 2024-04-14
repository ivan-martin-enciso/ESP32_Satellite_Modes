#include "Utils.h"
#include "Strings.h"

/**
 * @file Utils.h
 * @brief Implementation class for common code shared between the modes.
 * @author Ivan Martin Enciso 
 */

// ----- Global variable definitions -----
bool setDefaultValues = false;                            ///< True if one wants to set the default memory values
bool clearMemory = false;                                 ///< True to clear the memory
// ----- Modes -----
int currentMode = -1;                                     ///< Current mode
int volatile nextMode = 0;                                ///< Next mode
String currentModeString;                                 ///< Current mode string
int defaultMode = 0;                                      ///< Default mode
bool volatile isComLoRa = true;                           ///< Flag indicating communication mode, set by user. 
bool refreshMode5 = false;                                ///< Indicates if mode 5 is refreshed

// ----- Ldr thresholds ----- 
volatile int ldrThreshold1;                               ///< Ldr threshold 1
volatile int ldrThreshold2;                               ///< Ldr threshold 2
volatile int ldrThreshold3;                               ///< Ldr threshold 3

// ----- Temperature threshold -----                      
volatile int temperatureLowerThreshold;                   ///< Lower temperature threshold
volatile int temperatureUpperThreshold;                   ///< Upper temperature threshold

// ----- Objects -----
Display lcdDisplay;                                       ///< LCD display object
IMU imu;                                                  ///< IMU object
BME280 bme280;                                            ///< BME280 sensor object
ComsManager comsManager;                                  ///< Communication manager object
Buzzer buzzer;                                            ///< Buzzer object
StorageManager storageManager;                            ///< Storage manager object
ServoController servoController;                          ///< Servo controller object
Ldr ldr;                                                  ///< LDR object
TelecommandsManager telecommandsManager;                  ///< Telecommands manager object
ChartDisplay chartDisplay;                                ///< Chart display object

// ----- Timer settings -----
const int timerInterval = 1000000;                        ///< Timer interval in milliseconds
hw_timer_t * houseKeepingTimer = NULL;                    ///< Housekeeping timer
int volatile mode3Time = 10;                              ///< Mode 3 timer setup
bool volatile startTimer = false;                         ///< Indicates if mode 3 timer is started
volatile unsigned long mode3RemainingTime = 1;            ///< Remaining time for mode 3
volatile int currentServoStep = initialServoPosition;     ///< Current servo step
volatile int servoStepValue = 0;                          ///< Servo step value

// -----Leds -----
byte currentLed = -1;                                     ///< Current LED

// -----Housekeeping data -----
const float M_S2_TO_G = 1.0 / 9.81;                       ///< Conversion factor from m/s^2 to g


/**
 * @brief Resets Mode 3 parameters.
 * 
 * @details Resets the mode 3 timer, remaining time, timer status, servo step, 
 *          and sets the servo position to the initial position.
 */
void resetMode3(){
  mode3Time = 10;
  mode3RemainingTime = 1;
  startTimer = false;
  currentServoStep = initialServoPosition;
  servoController.setServoPosition(initialServoPosition);
}

/**
 * @brief Increases the mode number based on touch right reading.
 * 
 * @details If the maximum touch right value is above the threshold and the current 
 *          mode is less than or equal to 5, it increments the next mode by 1. 
 *          If the current mode is 3, it resets mode 3. 
 *          Sets the current LED to green if the next mode is less than or equal to 5, otherwise sets it to red.
 */
void increaseModeNumber(){
  int maxTouch = max(touchRead(touchUpPin), touchRead(touchRightPin));
  if(maxTouch > touchThreshold && currentMode <= 5){
    if(currentMode == 3) resetMode3();
    if(currentMode <= 5){
      nextMode = currentMode + 1;
      currentLed = greenLed;
    }
    if(nextMode > 5){
      currentLed = redLed;
    }
  }
}

/**
 * @brief Decreases the mode number based on touch left reading.
 * 
 * @details If the maximum touch left value is above the threshold and the current 
 *          mode is less than or equal to 5, it increments the next mode by 1. 
 *          If the current mode is 3, it resets mode 3. 
 *          Sets the current LED to yellow if the next mode is less than or equal to 5, otherwise sets it to red.
 */
void decreaseModeNumber(){
  int maxTouch = max(touchRead(touchDownPin), touchRead(touchLeftPin));
  if(maxTouch > touchThreshold){
    if(currentMode == 3) resetMode3();
    if(currentMode >= 0){
      nextMode = currentMode - 1;
      currentLed = yellowLed;
    }
    if(nextMode < 0){
      currentLed = redLed;
    }
  }
}

/**
 * @brief Initializes the touch buttons.
 * 
 * @details Attaches interrupts to the touch pins for mode increment (right) and decrement (left).
 */
void initializeTouchbuttons(){ 
  Serial.print(INITIALIZE_TOUCHPADS);
  touchAttachInterrupt(touchRightPin, increaseModeNumber, touchThreshold);
  touchAttachInterrupt(touchLeftPin, decreaseModeNumber, touchThreshold);
  Serial.println(COMPLETE);
}

/**
 * @brief Handles Mode 3 Timer.
 * 
 * @details If the timer is started, if mode 3 and if the remaining time is greater than 0.
 *          It decrements the time and the servo step. 
 */
void handleMode3Timer() {
  if (startTimer && currentMode == 3 && mode3RemainingTime > 0) {
    mode3RemainingTime--;
    currentServoStep += servoStepValue;
  }
}

/**
 * @brief Callback for Houskeeping Data.
 * 
 * @details Sets telemetry sending to true, refreshes mode 5 and calls for mode 3 timer function. 
 */
void handleHousekeepingData(){
  sendTelemetry = true;
  if(currentMode == 5) refreshMode5 = true;
  handleMode3Timer();
}

/**
 * @brief 1 second timer initialization for housekeeping data.
 * 
 * @details Sets a 1 second timer and attaches the handleHousekeepingData callback function. 
 */
void initializeHousekeepingTimer(){ 
  Serial.print(INITIALIZE_TIMER);
  houseKeepingTimer = timerBegin(0, 80, true);                              
  timerAttachInterrupt(houseKeepingTimer, &handleHousekeepingData, true);   
  timerAlarmWrite(houseKeepingTimer, timerInterval, true);                  
  timerAlarmEnable(houseKeepingTimer);                                     
  Serial.println(COMPLETE);
}

/**
 * @brief Checks if current mode has changed.
 * @return True if the current mode has changed, else false. 
 */
bool modeChanged(){
  return (currentMode != nextMode);
}

/**
 * @brief Handles mode change.
 * 
 * @details Turns off all LEDs, blinks the specified LED according to the mode change, 
 *          plays a beep sound if the next mode is out of range, 
 *          limits the mode to 0-5, sets startMode5 flag if next mode is 5,
 *          updates the current mode string, and prints current mode string to Serial and OLED display.
 */
void handleModeChange(){
  Serial.println("Handle mode change");
  ledController.turnAllOff();
  ledController.blink(currentLed, currentLed == redLed ? 3 : 1);    
  if (nextMode < 0 || nextMode > 5) buzzer.beep(3, 50);
  nextMode = constrain(currentMode, 0, 5);                          
  if (nextMode == 5) startMode5 = true;
  currentModeString = MODE;
  currentModeString.concat(nextMode);                                
  lcdDisplay.setCurrentMode(currentModeString);
  Serial.println(currentModeString);                                
}

/**
 * @brief Fetches the initial values from EEPROM memory.
 * 
 * @param fetchDefaultMode Flag indicating whether to fetch default mode value.
 * 
 * @details Reads initial values from EEPROM memory locations and assigns them to global variables. 
 *          Finaly it prints fetched values to Serial.
 *          If there is an error it also prints in Serial. 
 */
void fetchInitialValuesFromMemory(bool fetchDefaultMode = true){
  try{
    defaultMode = storageManager.readInt8Data(defaultModeAddress);
    if(fetchDefaultMode) nextMode = defaultMode;
    ldrThreshold1 = storageManager.readInt16Data(defaultLdrThreshold1Address);
    ldrThreshold2 = storageManager.readInt16Data(defaultLdrThreshold2Address);
    ldrThreshold3 = storageManager.readInt16Data(defaultLdrThreshold3Address);
    temperatureLowerThreshold = storageManager.readInt8Data(defaultTemperatureLowerThresholdAddress);
    temperatureUpperThreshold = storageManager.readInt8Data(defaultTemperatureUpperThresholdAddress);

    Serial.println("Initial Values from Memory:");
    Serial.print("Default Mode: ");
    Serial.println(nextMode);
    Serial.print("LDR Threshold 1: ");
    Serial.println(ldrThreshold1);
    Serial.print("LDR Threshold 2: ");
    Serial.println(ldrThreshold2);
    Serial.print("LDR Threshold 3: ");
    Serial.println(ldrThreshold3);
    Serial.print("Temperature lower limit: ");
    Serial.println(temperatureLowerThreshold);
    Serial.print("Temperature upper limit: ");
    Serial.println(temperatureUpperThreshold);
  }
  catch (int error) {
    Serial.print(EEPROM_READ_ERROR);
    Serial.println(error);
  }
}

/**
 * @brief Prompts the user to change between WiFi and LoRa communication.
 */
void promptUser() {
  Serial.println("Waiting for User Prompt...");
  lcdDisplay.promptLoRa();
  while (true) {
    if (touchRead(touchXPin) > touchThreshold) break;
    if (touchRead(touchLeftPin) > touchThreshold) {
      lcdDisplay.promptLoRa();
      isComLoRa = true;
    }
    else if (touchRead(touchRightPin) > touchThreshold) {
      lcdDisplay.promptWiFi();
      isComLoRa = false;
    }
    delay(100);
  }
  Serial.print("User set communications to LoRa: ");
  Serial.println(isComLoRa);
}

/**
 * @brief Initializes all board sensors and features and prints in OLED a progress bar accordingly.
 */
void initializeBoard() {
  try {
    Serial.println(BOARD_SETUP);                          
    // Initialize Display
    lcdDisplay.initializeDisplay();    
    promptUser();                    
    lcdDisplay.updateProgressBar(0);                       // Update progress bar with 0%
     // Initialize memory
    storageManager.initializeEEPROM();
    if(clearMemory) storageManager.clearEEPROM();
    if(setDefaultValues) storageManager.setDefaultValues();
    fetchInitialValuesFromMemory();
    lcdDisplay.updateProgressBar(10);                      // Update progress bar with 20%
    // Initialize Leds
    ledController.initializeLeds();                        
    lcdDisplay.updateProgressBar(20);                      // Update progress bar with 20%
    // Initialize Servo
    servoController.initializeServo();                               
    lcdDisplay.updateProgressBar(30);                      // Update progress bar with 30%
    // Initialize buzzer
    buzzer.initializeBuzzer();                             
    lcdDisplay.updateProgressBar(40);                      // Update progress bar with 40%
    // Initialize touchbuttons
    initializeTouchbuttons();
    lcdDisplay.updateProgressBar(50);                      // Update progress bar with 50%
    // Initialize IMU
    imu.initializeIMU();
    lcdDisplay.updateProgressBar(60);                      // Update progress bar with 60%
    // Initialize BME280
    bme280.initializeBME280();
    lcdDisplay.updateProgressBar(70);                      // Update progress bar with 70%
    // Initialize communications
    comsManager.initializeComs();
    lcdDisplay.updateProgressBar(80);                      // Update progress bar with 80%
    // Initialize timer
    initializeHousekeepingTimer();  
    lcdDisplay.updateProgressBar(100);                     // Update progress bar with 100%
    
    delay(100);
    lcdDisplay.printInDisplay(BOARD_SETUP_COMPLETE, 10);   // Print in Display
    Serial.println(BOARD_SETUP_COMPLETE);                  // Print in Serial
    delay(1000);
    lcdDisplay.drawMainScreen();
  } 
  catch (int error) {
    Serial.print(ERROR);
    Serial.println(error);
    ESP.restart();
  }
}

/**
 * @brief Gets angular rate from imu sensor.
 * 
 * @details Retrieves angular rate from imu sensor and converts the value to degrees.
 * 
 * @return String Formatted string containing the angular rates along the X, Y, and Z axes.
 */
String getAngularRate(){
  sensors_event_t gyroscope = imu.getGyroscope();
  float xInDeg = gyroscope.gyro.x * RAD_TO_DEG; 
  float yInDeg = gyroscope.gyro.y * RAD_TO_DEG; 
  float zInDeg = gyroscope.gyro.z * RAD_TO_DEG;
  return X + String(xInDeg, 1) + DEG_PER_S + COMMA + Y + String(yInDeg, 1) + DEG_PER_S + COMMA + Z + String(zInDeg, 1) + DEG_PER_S;
}

/**
 * @brief Gets acceleration from imu sensor.
 * 
 * @details Retrieves acceleration from imu sensor and converts the value to g.
 * 
 * @return String Formatted string containing the acceleration along the X, Y, and Z axes.
 */
String getAcceleration(){
  sensors_event_t acceleration = imu.getAcceleration();
  float xInG = acceleration.acceleration.x * M_S2_TO_G; 
  float yInG = acceleration.acceleration.y * M_S2_TO_G; 
  float zInG = acceleration.acceleration.z * M_S2_TO_G; 
  return X + String(xInG, 1) + G_UNIT + COMMA + Y + String(yInG, 1) + G_UNIT + COMMA + Z + String(zInG, 1) + G_UNIT;
}

/**
 * @brief Get servo position.
 * 
 * @details Retrieves the current servo positiona and maps it to the range of -90 to 90 degrees.
 * 
 * @return String Formatted string containing the servo position in degrees.
 */
String getServoPosition(){
  int currentPosition = servoController.getServoPosition();
  currentPosition = mode3RemainingTime == 0 ? 179 : currentPosition; 
  return String(map(currentPosition, 179, -1, -90, 90)) + DEGREES;
}

/**
 * @brief Get touchpad values.
 * 
 * @details Retrieves the touchpad states in format 0 or 1 (for touched).
 * 
 * @return String Formatted string containing all touchpads states.
 */
String getTouchpadsValues(){
  bool up = touchRead(touchUpPin) > touchThreshold ? true : false;
  bool down = touchRead(touchDownPin) > touchThreshold ? true : false;
  bool x = touchRead(touchXPin) > touchThreshold ? true : false;
  bool left = touchRead(touchLeftPin) > touchThreshold ? true : false;
  bool right = touchRead(touchRightPin) > touchThreshold ? true : false;
  return TOUCH_UP + String(up) + COMMA + 
         TOUCH_DOWN + String(down) + COMMA +
         TOUCH_X + String(x) + COMMA +
         TOUCH_LEFT + String(left) + COMMA +
         TOUCH_RIGHT + String(right);
}

/**
 * @brief Read data for Mode 5.
 * 
 * @details Reads data based on the selected function for Mode 5 and returns the corresponding value.
 * 
 * @return The data value read for Mode 5, meaning the value which is going to be displayed in the chart.
 */
double readMode5Data(){
  double value = 0.0;
  switch(mode5Function){
    case 0:
      value = bme280.getTemperature();
      break;
    case 1:
      value = ldr.readLdr();
      break;
    case 2:
      value = bme280.getHumidity();
      break;
    case 3:
      value = bme280.getAltitude();
      break;
    case 4:
      value = bme280.getPressure();
      break;
    default:
      break;
  }
  refreshMode5 = false;
  return value;
}

// ----- Communictions -----
/**
 * @brief Collects all houskeeping data values in JsonFormat.
 * 
 * @details Collects various all housekeeping data into a JSON document for transmission.
 * 
 * @return JsonDocument The JSON document containing the collected data.
 */
JsonDocument collectHousekeepingData() {
  JsonDocument doc;  
  doc["Current Mode"] = currentMode;
  doc["Default Mode"] = defaultMode;
  imu.readIMUValues();
  doc["Angular Rate"] = getAngularRate();
  doc["Acceleration"] = getAcceleration();
  doc["Servo Position"] = getServoPosition();
  bme280.readBME280Values();
  doc["Temperature"] = String(bme280.getTemperature(), 1) + DEGCELSIUS;
  doc["Humidity"] = String(bme280.getHumidity(), 1) + PERCENTAGE;
  doc["Pressure"] = String(bme280.getPressure(), 1) + PASCALS;
  doc["Altitude"] = String(bme280.getAltitude(), 1) + METERS;
  doc["WiFi RSSI"] = String(comsManager.getWiFiRSSI(), 1) + DB;
  doc["LoRA RSSI"] = String(comsManager.getLoRaRSSI(), 1) + DB;
  doc["LoRA SNR"] = String(comsManager.getLoRaSNR(), 1) + DB;
  doc["LoRA Frequency Error"] = String(comsManager.getLoRaFreqError(), 1) + HZ;
  doc["Light intensity"] = String(ldr.readLdr());
  doc["Touchpads"] = getTouchpadsValues();
  doc["Default LDR Threshold 1"] = String(ldrThreshold1);
  doc["Default LDR Threshold 2"] = String(ldrThreshold2);
  doc["Default LDR Threshold 3"] = String(ldrThreshold3);
  doc["Default Temperature Lower Threshold"] = String(temperatureLowerThreshold) + DEGCELSIUS;
  doc["Default Temperature Upper Threshold"] = String(temperatureUpperThreshold) + DEGCELSIUS;
  return doc; 
} 

/**
 * @brief Processes the string received from the selected communication.
 * 
 * @details Processes the payload to execute telecommands and update system parameters accordingly.
 * 
 * @param payload The payload received from communication.
 */
void processPayload(String payload) {
  int receivedMode = telecommandsManager.processTelecommand(payload, currentMode); 
  switch (receivedMode) {
    case -1: ///< No action needed
      break;
    case -2: ///< Update default mode if changed
      if (defaultMode != nextDefaultMode)  defaultMode = nextDefaultMode;
      break;
    case -3: ///< Fetch initial values from memory
      fetchInitialValuesFromMemory(false);
      break;
    default: 
      if (currentMode == 3)  resetMode3(); 
      nextMode = receivedMode != currentMode ? receivedMode : currentMode; ///< Update next mode if changed
      break;
  }
  receivedWifi = false; 
  receivedLora = false; 
}

/**
 * @brief Handles telemetry and telecommands.
 * 
 * @details Sends telemetry data every second and processes incoming communication from either LoRa or Wi-Fi.
 */
void handleCommunications(){
  if(sendTelemetry){
    JsonDocument telemetryData = collectHousekeepingData();
    comsManager.sendTelemetryData(telemetryData);
  }
  if(isComLoRa && receivedLora){
    comsManager.receivePackageUsingLora();
    processPayload(receivedPayloadLoRa);
  }
  else if (!isComLoRa && receivedWifi){
    Serial.println(RECEIVED_WIFI);
    processPayload(receivedPayloadWiFi);
  }
}

  
