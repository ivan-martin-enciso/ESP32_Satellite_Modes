#include "ComsManager.h"

/**
 * @file ComsManager.cpp
 * @brief Implementation class for handling MQTT, WiFi, Serial and LoRa communications. 
 * @author Ivan Martin Enciso 
 */

// ----- Wifi -----
const String wifiSSID        = SECRET_WIFI_SSID;            ///< WiFi SSID.
const char wifiPassword[]    = SECRET_WIFI_PASSWORD;        ///< WiFi password.
const char wifiAnonymousId[] = SECRET_WIFI_ANONYMOUSID;     ///< WiFi anonymous ID.
const char wifiEduroamId[]   = SECRET_WIFI_EDUROAMID;       ///< WiFi eduroam ID.
const int serialPort              = 23;                     ///< Serial port number.
const int maximumServerClients    = 1;                      ///< Maximum number of server clients.
volatile bool receivedWifi = false;                         ///< Flag indicating if WiFi message has been received.

WiFiServer server(serialPort);                              ///< WiFi server object.
WiFiClient serverClients[maximumServerClients];             ///< Array of WiFi clients objects.
WiFiClientSecure wifiClient;                                ///< Secure WiFi client object.

// ----- MQTT -----
const char   mqttBroker[]    = SECRET_MQTT_BROKER;          ///< MQTT broker.
const  int   mqttPort        = SECRET_MQTT_PORT;            ///< MQTT port.
const char   mqttUser[]      = SECRET_MQTT_USER;            ///< MQTT user.
const char   mqttPassword[]  = SECRET_MQTT_PASSWORD;        ///< MQTT password.
const String mqttPrefix      = SECRET_MQTT_PREFIX;          ///< MQTT prefix.
const int    mqttYear        = SECRET_MQTT_YEAR;            ///< MQTT year.
const byte   mqttBoardId     = SECRET_MQTT_BOARDID;         ///< MQTT board ID.
String mqttPublish;                                         ///< MQTT publish string.
String mqttSubscribe;                                       ///< MQTT subscribe string.
String mqttTopic;                                           ///< MQTT topic.
PubSubClient mqttClient(wifiClient);                        ///< MQTT client object.

// Encryption using https://letsencrypt.org/certs/lets-encrypt-r3.pem
const char tlsPublicCertificate[] = ("\
-----BEGIN CERTIFICATE-----\n\
MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n\
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n\
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n\
WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n\
RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n\
AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n\
R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n\
sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n\
NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n\
Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n\
/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n\
Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n\
FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n\
AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n\
Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n\
gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n\
PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n\
ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n\
CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n\
lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n\
avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n\
yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n\
yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n\
hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n\
HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n\
MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n\
nLRbwHOoq7hHwg==\n\
-----END CERTIFICATE-----\n");

// ----- LoRa -----
static const int loraFrequency = 868.0, loraCSPin = 38, loraRSTPin = 48, loraIrqPin = 47;   ///< LoRa frequency, CS pin, RST pin, and IRQ pin.
SX1276 radioClient = new Module(loraCSPin, loraIrqPin, loraRSTPin, RADIOLIB_NC);            ///< LoRa radio client object.
volatile bool receivedLora = false;                                                         ///< Flag indicating if LoRa message has been received.

// ----- Telnet to Serial Communication -----
const char* welcomeMsg = SERIAL_WELCOME;            ///< Char array with welcome message.
const char* promptMsg = PROMPT_USER;                ///< Char array with prompt message.
size_t welcomeLen = strlen(welcomeMsg);             ///< Length of welcome message.
size_t promptLen = strlen(promptMsg);               ///< Length of prompt message.
const size_t incomingDatabufferSize = 128;          ///< Size of incoming data buffer.
char incomingDataBuffer[incomingDatabufferSize];    ///< Incoming data buffer.
int incomingDataIndex = 0;                          ///< Index of incoming data.

// ----- Telemetry & Telecommand-----
volatile bool sendTelemetry;                        ///< Flag indicating if telemetry data should be sent.
String receivedPayloadWiFi;                         ///< String where information over WiFi is received.
String receivedPayloadLoRa;                         ///< String where information over LoRa is received.


// ----- Private functions -----

/**
 * @brief Prints the connected IP address to serial.
 */
void printIp(){
    Serial.print(IP_ADDRESS);
    Serial.print(WiFi.localIP());
    Serial.println(COMPLETE);
}

/**
 * @brief Checks if the current wifiSSID is Eduroam.
 * 
 * @return True if wifiSSID is Eduroam, else false.
 */
bool IsEduroamConnection(){
    return wifiSSID.equals(EDUROAM);
}

/**
 * @brief Initializes the Wi-Fi connection.
 * 
 * @details Function to connect to a normal Wi-Fi network using the credentials in arduino_secrets.h
 *          (wifiSSID and wifiPassword). It prints status messages to Serial during the connection process.
 *          Finally it prints the ip. 
 */
void initializeWifi() {
    delay(10);
    Serial.print(INITIALIZE_WIFI);
    Serial.print(CONNECTING_TO);
    Serial.print(wifiSSID.c_str());
    WiFi.begin(wifiSSID.c_str(), wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(" - ");
    }
    randomSeed(micros());
    server.begin();
    server.setNoDelay(true);
    printIp();
}

/**
 * @brief Initializes Wi-Fi connection to Eduroam.
 * 
 * @details Function to connect to the Eduroam Wi-Fi network using the credentials in arduino_secrets.h
 *          (wifiAnonymousId, wifiEduroamId, and wifiPassword). It prints status messages to Serial during the connection process.
 *          Finally it prints the ip. 
 */
void initializeEduroamWifi() {
  delay(10);
  Serial.println(INITIALIZE_WIFI);
  Serial.print(CONNECTING_TO);
  Serial.println(wifiSSID);
  WiFi.disconnect(true);  
  WiFi.mode(WIFI_STA); 
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)wifiAnonymousId, strlen(wifiAnonymousId)); 
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)wifiEduroamId, strlen(wifiEduroamId)); 
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)wifiPassword, strlen(wifiPassword)); 
  esp_wifi_sta_wpa2_ent_enable(); 
  WiFi.begin(wifiSSID.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  printIp();
}

/**
 * @brief Sets a flag to true when a packet is received through LoRa. 
 * 
 * @details Function is called when a complete packet is received by the LoRa module. 
 *          IMPORTANT: this function MUST be 'void' type and MUST NOT have any arguments!
 */
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setLoraRxFlag(void) {
  receivedLora = true;    
}

/**
 * @brief Initializes the LoRa module.
 * 
 * @details Initializes the SX1278 LoRa module with a frequency of 868.0  Hz. 
 *          If initialization fails, the function enters an infinite loop. 
 *          After initialization, callback function setLoraRxFlag is set when a package is received. 
 *          Finally it starts listening for LoRa packets.
 */
void initializeLora() {
    Serial.print(INITIALIZE_LORA);
    int state = radioClient.begin(loraFrequency);
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print(ERROR);
        Serial.println(state);
        while (true);
    } 

    radioClient.setPacketReceivedAction(setLoraRxFlag);

    Serial.print(LORA_LISTENING);
    state = radioClient.startReceive();

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(COMPLETE);
    } else {
        Serial.print(ERROR);
        Serial.println(state);
        while (true);
    }
}

/**
 * @brief Callback function for handling MQTT subscription to the defined topic in the initialization.
 * 
 * @param topic The topic of the received MQTT message.
 * @param payload The payload of the received MQTT message.
 * @param length The length of the payload.
 * 
 * @details Function handles the reception of MQTT messages,
 *          Converts the received payload to a string and sets a flag to indicate that a message has been received.
 */
void receivePackageUsignMqtt(char* topic, byte *payload, unsigned int length) {
    char str[length + 1];
    memcpy(str, payload, length + 1);
    str[length + 1] = '\0';
    receivedPayloadWiFi = String(str);
    receivedWifi = true; 
}

/**
 * @brief Initializes the MQTT connection.
 * 
 * @details Attempts to establish a connection to the MQTT broker defined in arduino_secrets
 *          printing the process in the Serial. 
 *          If the connection attempt fails, the function retries every 5 seconds 
 *          until a connection is established.
 */
void initializeMQTT() {
    Serial.print(INITIALIZE_MQTT);
    wifiClient.setCACert(tlsPublicCertificate);

    String mqttClientId = mqttPrefix + "-" + String(mqttBoardId);

    while (!mqttClient.connected()) {
        Serial.print(MQTT_ATTEMPT);
        Serial.print(mqttBroker);
        Serial.println(MQTT_WAIT);
        
        if (mqttClient.connect(mqttClientId.c_str(), mqttUser, mqttPassword)) {
            Serial.print(MQTT_CLIENT_ID);
            Serial.println(mqttClientId);
            mqttClient.subscribe(mqttSubscribe.c_str());
        } else {
            Serial.print(MQTT_FAILED);
            Serial.print(mqttClient.state());
            delay(5000);
        }
    }
    Serial.println(COMPLETE);
}

/**
 * @brief Constructor for the ComsManager class.
 */
ComsManager::ComsManager() {}

/**
 * @brief Destructor for the ComsManager class.
 */
ComsManager::~ComsManager() {}

/**
 * @brief Initializes all communications.
 * 
 * @details Creates MQTT topics ("cadse/2023/broadcast" and "cadse/2023/16/").
 *          Initializes Wi-Fi or eduroam connection.
 *          Initializes LoRa communication.
 *          Finally it sets up MQTT server and the callback function for receiving packages via MQTT subscription, and initializes MQTT.
 */
void ComsManager::initializeComs() {
    mqttTopic = mqttPrefix + "/" + String(mqttYear) + "/" + String(mqttBoardId) + "/" + TELEMETRY_TOPIC;
    mqttSubscribe = TELECOMMAND_TOPIC;
    if (IsEduroamConnection()) {
        initializeEduroamWifi();
    } else {
        initializeWifi();
    }

    initializeLora();
    mqttClient.setServer(mqttBroker, mqttPort);
    mqttClient.setCallback(receivePackageUsignMqtt);
    initializeMQTT();
}

/**
 * @brief Gets the Wi-Fi RSSI (Received Signal Strength Indication).
 * 
 * @return The Wi-Fi RSSI value if connected, if it is not connected it attempt a reconnection and returns 0.
 */
float ComsManager::getWiFiRSSI() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.RSSI();
    } else {
        if (IsEduroamConnection()) {
            initializeEduroamWifi();
        } else {
            initializeWifi();
        }
    }
    return 0;
}

/**
 * @brief Sets the LoRa RSSI (Received Signal Strength Indication).
 */
void ComsManager::setLoRaRSSI() {
    loraRSSI = radioClient.getRSSI();
}

/**
 * @brief Sets the LoRa SNR (Signal-to-Noise Ratio).
 */
void ComsManager::setLoRaSNR() {
    loraSNR = radioClient.getSNR();
}

/**
 * @brief Sets the LoRa frequency error.
 */
void ComsManager::setLoRaFreqError() {
    loraFrequencyError = radioClient.getFrequencyError();
}

/**
 * @brief Gets the LoRa RSSI (Received Signal Strength Indication).
 * 
 * @return The LoRa RSSI value in dB.
 */
float ComsManager::getLoRaRSSI() {
    return loraRSSI;
}

/**
 * @brief Gets the LoRa SNR (Signal-to-Noise Ratio).
 * 
 * @return The LoRa SNR value in dB.
 */
float ComsManager::getLoRaSNR() {
    return loraSNR;
}

/**
 * @brief Gets the LoRa frequency error.
 * 
 * @return The LoRa frequency error value in Hz.
 */
float ComsManager::getLoRaFreqError() {
    return loraFrequencyError;
}

/**
 * @brief Creates the bridge for serial to WiFi communication.
 * 
 * @details Function that stablishes a connection to possible multiple WiFi clients between Serial and WiFi.
 *          If success the connection is printed in the Serial. 
 *          Using telnet, the communication between the satellite serial and WiFi is possible. 
 *          IMPORTANT: This method is only implemented for testing purposes. Not used in proyect final deployment. 
 */
void ComsManager::serialWifiComunication(){
    uint8_t i;
    if (WiFi.status() == WL_CONNECTED){
        if(server.hasClient()) {
            for (i = 0; i < maximumServerClients; i++){
                if(!serverClients[i] || !serverClients[i].connected()){
                    if(serverClients[i]) serverClients[i].stop();
                    serverClients[i] = server.available();
                    if (!serverClients[i]) Serial.println("Availability broken");
                    Serial.print(NEW_CLIENT);
                    Serial.print(i); Serial.print(' ');
                    Serial.println(serverClients[i].remoteIP());
                    serverClients[i].write((const uint8_t*)welcomeMsg, welcomeLen);
                    serverClients[i].write("\r\n");
                    serverClients[i].write((const uint8_t*)promptMsg, promptLen);
                    serverClients[i].write("\r\n");
                    memset(incomingDataBuffer, 0, sizeof(incomingDataBuffer));
                    break;
                }
            }
            if(i >= maximumServerClients){
                server.available().stop();
            }
        }
        for (i = 0; i < maximumServerClients; i++) {
            if (serverClients[i] && serverClients[i].connected()) {
                while (serverClients[i].available()) {
                    char c = serverClients[i].read();
                    if (c == '\n') {
                        // End of command, print received data
                        Serial.print(RECEIVED_DATA);
                        Serial.println(incomingDataBuffer);
                        memset(incomingDataBuffer, 0, sizeof(incomingDataBuffer));
                        incomingDataIndex = 0;
                    } 
                    else {
                        if (incomingDataIndex < incomingDatabufferSize) {
                            incomingDataBuffer[incomingDataIndex++] = c;
                        } 
                        else {
                            memset(incomingDataBuffer, 0, sizeof(incomingDataBuffer));
                            incomingDataIndex = 0;
                        }
                    }
                }
            } else {
                if (serverClients[i]) {
                    serverClients[i].stop();
                }
            }
        }

        // Check UART for data
        if(Serial.available()){
            size_t len = Serial.available();
            uint8_t buffer[len];
            Serial.readBytes(buffer, len);
            // Push UART data to all connected telnet clients
            for (i = 0; i < maximumServerClients; i++){
                if(serverClients[i] && serverClients[i].connected()){
                    serverClients[i].write(buffer, len);
                    delay(1);
                }
            }
        }
    }
    else{
        Serial.println(WIFI_NOT_CONNECTED);
        for (i = 0; i < maximumServerClients; i++){
            if(serverClients[i])serverClients[i].stop();
        }
        delay(1000);
    }
}

/**
 * @brief Sends telemetry data over MQTT publish topic.
 * 
 * @param telemetryJson The JSON document containing telemetry data.
 * 
 * @details The function receives a JsonDocument and per Key creates a topic to publish the value. 
 *          If the sendTelemetry flag is true, it check the MQTT conection and publishes the data. 
 *          Finally it sets back the flag to false. 
 */
void ComsManager::sendTelemetryData(JsonDocument telemetryJson) {
    if (sendTelemetry) {
        mqttClient.loop();
        if (!mqttClient.connected()) {
            initializeMQTT();
        }
        Serial.println(SENDING_HOUSEKEEPING_DATA);

        for (JsonPair keyValue : telemetryJson.as<JsonObject>()) {
            String mqttPublish = mqttTopic + "/";
            const char* key = keyValue.key().c_str();
            mqttPublish.concat(key);
            String jsonData = String(keyValue.value().as<String>());
            mqttClient.publish(mqttPublish.c_str(), jsonData.c_str());
        }

        sendTelemetry = false;
    }
}

/**
 * @brief Receives a package using LoRa communication.
 * 
 * @details If the receivedLora flag is true, the function reaads the data from the LoRa module and converts it to String. 
 *          It then stores the data in a global variable and sets the flag back to false. 
 *          If an error ocurrs it print in the Serial. 
 */
void ComsManager::receivePackageUsingLora() {
    if (receivedLora) {
        receivedLora = false;
        Serial.println(RECEIVED_LORA);
        String receivedString;
        int state = radioClient.readData(receivedString);

        if (state == RADIOLIB_ERR_NONE) {
            setLoRaRSSI();
            setLoRaSNR();
            setLoRaFreqError();

            receivedPayloadLoRa = receivedString;
        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            Serial.println(LORA_CRC_ERROR);
        } else {
            Serial.print(LORA_ERROR);
            Serial.println(state);
        }
    }
}
