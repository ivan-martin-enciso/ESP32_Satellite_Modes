#ifndef STRINGS_H
#define STRINGS_H
#include "Arduino.h"

// General
const char SEPARATOR[] = "--------------------------";
const char ERROR[] = "Error: ";
const char SPACE[] = " ";
const char COMMA[] = ", ";

// Initialization
const char BOARD_SETUP [] = "Initializing board...";
const char BOARD_SETUP_COMPLETE[] = "Setup complete!";
const char COMPLETE[] = " ----> COMPLETE";

// Leds
const char INITIALIZE_LEDS[] = "Leds initializaiton";

// Touchpads
const char INITIALIZE_TOUCHPADS[] = "Touchpads initialization";
const char TOUCHPADS_ERROR[] = "Touchpads error: ";
const char TOUCH_RIGHT[] = "Right: ";
const char TOUCH_UP[] = "Up: ";
const char TOUCH_X[] = "X: ";
const char TOUCH_DOWN[] = "Down: ";
const char TOUCH_LEFT[] = "Left: ";

// Communications
const char TELECOMMAND_RECEIVED[] = "Received message from ground. ";
const char TOPIC[] = "Topic: ";
const char DATA[] = "Data: ";
const char TELEMETRY_TOPIC[] = "Ivan's Satellite Telemetry Data";
const char TELECOMMAND_TOPIC[] = "cadse/2023/broadcast";
const char TELECOMMAND_PROCESSING[] = "Processing telecommand for mode: ";
const char UNKNOWN_MODE[] = "Unknown mode received: ";
const char STORE_DEFAULT_MODE_TC[] = "New default mode received: ";

// Wifi
const char INITIALIZE_WIFI[] = "Wifi initialization: ";
const char CONNECTION_COMPLETE[] = "Connection complete!";
const char CONNECTING_TO[] = "Connecting to: ";
const char IP_ADDRESS[] = "IP Address: ";
const char RSSI[] = "WiFi RSSI: ";
const char DB[] = " dB";
const char NEW_CLIENT[] = "New client: ";
const char WIFI_NOT_CONNECTED[] = "Wifi not connected";
const char EDUROAM[] = "eduroam";
const char RECEIVED_WIFI[] = "Received package over WiFi";

// UART
const char SERIAL_WELCOME[] = "Hello, you are connected to Ivan's Satellite!\n";
const char PROMPT_USER[] = "Please enter a command followed by the 'Enter' key:\n";
const char RECEIVED_DATA[] = "Received data: ";
// Lora
const char INITIALIZE_LORA[] = "LoRa initialization: ";
const char LORA_LISTENING[] = "[SX1276] Starting to listen ... ";
const char RECEIVED_LORA[] = "Received package over LoRa";

// MQTT
const char INITIALIZE_MQTT[] = "MQTT initialization: ";

// Satellite modes
const char MODE[] = "Mode: ";
const char HANDLE_MODE_CHANGE[] = "Handling mode change";

// IMU
const char INITIALIZE_IMU[] = "IMU initialization";
const char IMU_SETUP_ERROR[] = "IMU Setup Error";
const char ACCELERATION[] = "Acceleration: ";
const char GYROSCOPE[] = "Gyroscope: ";
const char X[] = " X = ";
const char Y[] = " Y = ";
const char Z[] = " Z = ";
const char DEG_PER_S[] = " °/s";
const char G_UNIT[] = " g";
// BME280
const char TEMPERATURE[] = "Temperature: ";
const char CELSIUS[] = " C";
const char DEGCELSIUS[] = " °C";
const char PRESSURE[] = "Pressure: ";
const char PASCALS[] = " hPa";
const char ALTITUDE[] = "Altitude: ";
const char METERS[] = " m";
const char HUMIDITY[] = "Humidity: ";
const char PERCENTAGE[] = " %";
const char INITIALIZE_BME280[] = "BME280 initialization";
const char BME280_SENSOR_ERROR[] = "Could not find a valid BME280 sensor, check wiring, address, sensor ID!";
const char BME280_SENSOR_ID_ADDRESS[] = "SensorID was: 0x";
const char BME280_SENSOR_ID_BAD_ADDRESS[] = "ID of 0xFF probably means a bad address, a BMP 180 or BMP 085";
const char BME280_SENSOR_ID_BMP_280[] = "ID of 0x56-0x58 represents a BMP 280";
const char BME280_SENSOR_ID_BME_280[] = "ID of 0x60 represents a BME 280";
const char BME280_SENSOR_ID_BME_680[] = "ID of 0x61 represents a BME 680";

// Display
const char INITIALIZE_DISPLAY[] = "LCD Display initialization";
const char DISPLAY_SETUP_ERROR[] = "LCD Display Setup Error";
const char INITIALIZE_CHART_DISPLAY[] = "Chart Display initialization";
const char CHART_DISPLAY_SETUP_ERROR[] = "Chart Display Setup Error";

// Buzzer
const char INITIALIZE_BUZZER[] = "Buzzer initialization";

// Servo
const char INITIALIZE_SERVO[] = "Servo initialization";
const char SET_SERVO_POSITION[] = "Setting servo to position: ";
const char DEGREES[] = " °";

// EEPROM
const char INITIALIZE_EEPROM[] = "EEPROM initialization";
const char INITIALIZE_EEPROM_ERROR[] = "Failed to initialize EEPROM";
const char EEPROM_COMMIT_ERROR[] = "EEPROM commit failed!";
const char EEPROM_READ_ERROR[] = "EEPROM read failed: ";

// Telemetry data
const char WAITING_FOR_COMMAND[] = "Waiting for command!";
const char SENDING_HOUSEKEEPING_DATA[] = "Sending housekeeping data... ";

// Timers
const char INITIALIZE_TIMER[] = "Timer initialization";
const char SET_TIME[] = "Set time: ";
const char REMAINING_TIME[] = "Remaining time: ";
const char SECONDS[] = " s";
const char TIMER_DONE[] = "Timer is done!";

// Ldr
const char LDR[] = "Ldr: ";
const char THS1[] = "T1: ";
const char THS2[] = "T2: ";
const char THS3[] = "T3: ";

// SOS
const char SOS[] = "SOS";
#endif