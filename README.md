
# Space Electronics II Final Project 

This project represents a scenario in which a satellite engineering model is tested in the lab environment. The CADSE board will be referred to as the “satellite”. The satellite sends housekeeping data to the ground station network and receives commands. The satellite has two communication links, a lower band (868MHz LoRa, SF9, BW 125kHz) for receiving the commands, and a higher band (2.4GHz WiFi) for downlinking the data.

The satellite has 5 different modes to switch to. 

## Table of Contents
- [Mode 0](#mode-0)
- [Mode 1](#mode-1)
- [Mode 2](#mode-2)
- [Mode 3](#mode-3)
- [Mode 4](#mode-4)
- [Mode 5](#mode-5)
- [Usage](#usage)

## Mode 0
Idle mode where the satellite is just waiting for telecommands, but still sending telemetry data every second. 

## Mode 1
A bitmap image of the satellite is used to represent the gravitational "fall" using the IMU acceleration data. 

## Mode 2 
An alarm system for critical errors is implemented. When the environmental sensor reading of the temperature exceeds or drops below certain configurable thresholds, a buzzer as well as the OLED blinking Morse-coded “SOS” are called. 

## Mode 3
A configurable timer is implemented. The timer can be started using the touchpad 'X' and the remaining time is visualized in the display. Furthermore, a servo acts as a physical visualization of the time left where 100% is 90 degrees and 0% time left is -90 degrees. 

## Mode 4 
Light intensity is represented in using the LEDs of the board where each of them is lit up if a certain threshold is met. There are three configurable thresholds for this mode. 

## Mode 5
A graphical representation of all the sensors reading is implemented using a customizable graphics library. The readings are configurable via telecommand. 

## Usage

For Telecommands the topic is: cadse/2023/broadcast

Json used to set Default mode in persistent memory:
```JSON
{
  "type": "setDefaultMode",
  "cadse_id": 16,
  "mode": 0
}
```
Json used to change between modes and set thresholds:
```JSON
{
  "type": "telecommand",
  "cadse_id": 16,
  "mode": 1,
  "data": {
    "tempLower": 15,
    "tempUpper": 40,
    "defLdrT1": 1100,
    "defLdrT2": 2100,
    "defLdrT3": 3100
  }
}
```
Json used to change readings in mode 5:
```JSON
{
  "type": "telecommand",
  "cadse_id": 16,
  "mode": 5,
  "data": {
    "minY": "0",
    "maxY": "5000",
    "reading": "Ldr"
  }
}
```
The possible readings are:
- Temperature
- LDR
- Humidity
- Altitude
- Pressure

Recommended thresholds for each reading:
- Temperature (0 - 40)
- LDR         (0 - 5000)
- Humidity    (0 - 100)
- Altitude    (-200 - 1000)
- Pressure    (0 - 2000)