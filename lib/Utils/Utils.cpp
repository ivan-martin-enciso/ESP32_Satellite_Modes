#include "Utils.h"
#include "Strings.h"

// Definitions 
bool setDefaultValues = false;
bool clearMemory = false;
// Modes
int currentMode = -1;   
int volatile nextMode = 0;
String currentModeString;
int defaultMode = 0;


// ----- Ldr thresholds -----
volatile int ldrThreshold1;
volatile int ldrThreshold2;
volatile int ldrThreshold3; 

// ----- Temperature threshold -----
volatile int temperatureLowerThreshold;
volatile int temperatureUpperThreshold;

// Objects
Display lcdDisplay;
IMU imu;
BME280 bme280;
ComsManager comsManager;
Buzzer buzzer;
StorageManager storageManager;
ServoController servoController;
Ldr ldr;

// Timer settings
const int timerInterval = 1000000; // Interval in milliseconds
hw_timer_t * houseKeepingTimer = NULL;
int volatile mode3Time = 10;
bool volatile startTimer = false;
volatile unsigned long mode3RemainingTime = 1;
volatile int currentServoStep = initialServoPosition;
volatile int servoStepValue = 0;

// Leds
byte currentLed = -1;

// ***** Housekeeping data *****
const float M_S2_TO_G = 1.0 / 9.81;

/* The following two functions are used as ISRs for the touch buttons
 * The additional check for the touchRead value prevents the touch 
 * buttons to cause two mode changes since the interrupt is 
 * triggered on both falling and rising edges.
 */

void resetMode3(){
  mode3Time = 10;
  mode3RemainingTime = 1;
  startTimer = false;
  currentServoStep = initialServoPosition;
}

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

void initializeTouchbuttons(){ 
  Serial.print(INITIALIZE_TOUCHPADS);
  touchAttachInterrupt(touchRightPin, increaseModeNumber, touchThreshold);
  touchAttachInterrupt(touchLeftPin, decreaseModeNumber, touchThreshold);
  Serial.println(COMPLETE);
}

void handleMode3Timer() {
  if (startTimer && currentMode == 3 && mode3RemainingTime > 0) {
    mode3RemainingTime--;
    currentServoStep += servoStepValue;
  }
}


void handleHousekeepingData(){
  sendTelemetry = true;
  handleMode3Timer();
}

void initializeHousekeepingTimer(){ 
  Serial.print(INITIALIZE_TIMER);
  houseKeepingTimer = timerBegin(0, 80, true);                              // Timer 0, prescaler 80 (1MHz tick), count up
  timerAttachInterrupt(houseKeepingTimer, &handleHousekeepingData, true);   // Attach callback
  timerAlarmWrite(houseKeepingTimer, timerInterval, true);                  // Set interval in microseconds
  timerAlarmEnable(houseKeepingTimer);                                      // Enable timer
  Serial.println(COMPLETE);
}

bool modeChanged(){
  return (currentMode != nextMode);
}

void handleModeChange(){
  Serial.println("Handle mode change");
  ledController.turnAllOff();
  ledController.blink(currentLed, currentLed == redLed ? 3 : 1);     // Blink according to the mode change
  if (nextMode < 0 || nextMode > 5) buzzer.beep(3, 50);
  nextMode = constrain(currentMode, 0, 5);                           // Limit mode to 0-5
  currentModeString = MODE;
  currentModeString.concat(nextMode);                                // Append current mode
  lcdDisplay.setCurrentMode(currentModeString);
  Serial.println(currentModeString);                                 // Print in Serial
}

void fetchInitialValuesFromMemory(){
  try{
    defaultMode = storageManager.readInt8Data(defaultModeAddress);
    nextMode = defaultMode;
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

void initializeBoard() {
  try {
    Serial.println(BOARD_SETUP);                           // Print in Serial
    // Initialize Display
    lcdDisplay.initializeDisplay();                        
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
    lcdDisplay.updateProgressBar(30);                      // Update progress bar with 20%
    // Initialize buzzer
    buzzer.initializeBuzzer();                             
    lcdDisplay.updateProgressBar(40);                      // Update progress bar with 40%
    // Initialize touchbuttons
    initializeTouchbuttons();
    lcdDisplay.updateProgressBar(50);                      // Update progress bar with 65%
    // Initialize IMU
    imu.initializeIMU();
    lcdDisplay.updateProgressBar(60);                      // Update progress bar with 80%
    // Initialize BME280
    bme280.initializeBME280();
    lcdDisplay.updateProgressBar(70);                      // Update progress bar with 95%
    // Initialize communications
    comsManager.initializeComs(true);
    lcdDisplay.updateProgressBar(80);                      // Update progress bar with 100%
    // Initialize timer
    initializeHousekeepingTimer();  
    lcdDisplay.updateProgressBar(100);                     // Update progress bar with 100
    
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

String getAngularRate(){
  sensors_event_t gyroscope = imu.getGyroscope();
  float xInDeg = gyroscope.gyro.x * RAD_TO_DEG; // Convert to degrees
  float yInDeg = gyroscope.gyro.y * RAD_TO_DEG; // Convert to degrees
  float zInDeg = gyroscope.gyro.z * RAD_TO_DEG; // Convert to degrees
  return X + String(xInDeg, 1) + DEG_PER_S + COMMA + Y + String(yInDeg, 1) + DEG_PER_S + COMMA + Z + String(zInDeg, 1) + DEG_PER_S;
}

String getAcceleration(){
  sensors_event_t acceleration = imu.getAcceleration();
  float xInG = acceleration.acceleration.x * M_S2_TO_G; // Convert to g
  float yInG = acceleration.acceleration.y * M_S2_TO_G; // Convert to g
  float zInG = acceleration.acceleration.z * M_S2_TO_G; // Convert to g
  return X + String(xInG, 1) + G_UNIT + COMMA + Y + String(yInG, 1) + G_UNIT + COMMA + Z + String(zInG, 1) + G_UNIT;
}

String getServoPosition(){
  //Serial.println(servoController.getServoPosition());
  return String(map(servoController.getServoPosition(), 179, -1, -90, 90)) + DEGREES;
}

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

String collectHousekeepingData() {
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
  doc["WiFi RSSI"] = String(comsManager.getWiFiRSSI(true)) + DB;
  doc["LoRA RSSI"] = String(comsManager.getLoRaRSSI(), 1) + DB;
  doc["LoRA SNR"] = String(comsManager.getLoRaSNR(), 1) + DB;
  doc["Light intensity"] = String(ldr.readLdr());
  doc["Touchpads"] = getTouchpadsValues();
  doc["Default LDR Threshold 1"] = String(ldrThreshold1);
  doc["Default LDR Threshold 2"] = String(ldrThreshold2);
  doc["Default LDR Threshold 3"] = String(ldrThreshold3);
  doc["Default Temperature Lower Threshold"] = String(temperatureLowerThreshold) + DEGCELSIUS;
  doc["Default Temperature Upper Threshold"] = String(temperatureUpperThreshold) + DEGCELSIUS;
  String jsonData;
  serializeJson(doc, jsonData);

  return jsonData; 
} 


  
