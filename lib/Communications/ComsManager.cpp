#include "ComsManager.h"

// Definitions 

// ----- Wifi -----
const String wifiSSID        = SECRET_WIFI_SSID;
const char wifiPassword[]    = SECRET_WIFI_PASSWORD;
const char wifiAnonymousId[] = SECRET_WIFI_ANONYMOUSID;
const char wifiEduroamId[]   = SECRET_WIFI_EDUROAMID;
const int serialPort              = 23; 
const int maximumServerClients    = 1;
WiFiServer server(serialPort);
WiFiClient serverClients[maximumServerClients];
WiFiClientSecure wifiClient;
// ----- MQTT -----
const char   mqttBroker[]    = SECRET_MQTT_BROKER;
const  int   mqttPort        = SECRET_MQTT_PORT;
const char   mqttUser[]      = SECRET_MQTT_USER;
const char   mqttPassword[]  = SECRET_MQTT_PASSWORD;
const String mqttPrefix      = SECRET_MQTT_PREFIX;
const int    mqttYear        = SECRET_MQTT_YEAR;
const byte   mqttBoardId     = SECRET_MQTT_BOARDID;
String mqttPublish;  
String mqttSubscribe; 
String mqttTopic;
PubSubClient mqttClient(wifiClient);

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
static const int loraFrequency = 868.0, loraCSPin = 38, loraRSTPin = 48, loraIrqPin = 47;
SX1276 radioClient = new Module(loraCSPin, loraIrqPin, loraRSTPin, RADIOLIB_NC); // GPIO Pins are not connected on CADSE board!
volatile bool receivedFlag = false; // flag to indicate that a packet was received

const char* welcomeMsg = SERIAL_WELCOME;
const char* promptMsg = PROMPT_USER;
// Calculate the length of the welcome string and prompt message
size_t welcomeLen = strlen(welcomeMsg);
size_t promptLen = strlen(promptMsg);
const size_t incomingDatabufferSize = 128; 
char incomingDataBuffer[incomingDatabufferSize];
int incomingDataIndex = 0;

// ----- Telemetry & Telecommand-----
volatile bool sendTelemetry;
 String receivedPayload; 

ComsManager::ComsManager() {}

ComsManager::~ComsManager() {}

void printIp(){
    Serial.print(IP_ADDRESS);
    Serial.print(WiFi.localIP());
    Serial.println(COMPLETE);
}

/*
 * Initialize Wi-Fi connection
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

/*
 * Initialize Wi-Fi connection for eduroam
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

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setLoraRxFlag(void) {
  receivedFlag = true;    // we got a packet, set the flag
}

void initializeLora() {
    // initialize SX1278 with default settings
    Serial.print(INITIALIZE_LORA);
    int state = radioClient.begin(loraFrequency);
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print(ERROR);
        Serial.println(state);
        while (true);
    } 

    // set the function that will be called
    // when new packet is received
    radioClient.setPacketReceivedAction(setLoraRxFlag);

    // start listening for LoRa packets
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
/*
 * Callback function for MQTT subscription handler
 */
void receivePackageUsignMqtt(char* topic, byte *payload, unsigned int length) {
    char str[length + 1];
    memcpy(str, payload, length + 1);
    str[length + 1] = '\0';
    receivedPayload = String(str);
    receivedFlag = true; 
}
/*
 * Initialize MQTT connection
 */
void initializeMQTT() {
    Serial.print(INITIALIZE_MQTT);
    wifiClient.setCACert(tlsPublicCertificate);
    //wifiClient.setCertificate(tlsClientCertificate);
    //wifiClient.setPrivateKey(tlsClientPrivateKey);

    // Generate board-specific MQTT client ID
    String mqttClientId = mqttPrefix + "-" + String(mqttBoardId);

    // Loop until (re)connected
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection to ");
        Serial.print(mqttBroker);
        Serial.print(" ... ");
        // Attempt to connect
        if (mqttClient.connect(mqttClientId.c_str(), mqttUser, mqttPassword)) {
        Serial.print("Done using client ID ");
        Serial.print(mqttClientId);
        mqttClient.subscribe(mqttSubscribe.c_str());
        } else {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        delay(5000);
        }
    }
    Serial.println(COMPLETE);
}

void ComsManager::initializeComs(bool isEduroam) {
    // Generate board-specific MQTT topics using the pattern [prefix]/[year]/[ID]/[variable]
    mqttTopic = mqttPrefix + "/" + String(mqttYear) + "/" + String(mqttBoardId) + "/" + TELEMETRY_TOPIC;
    mqttSubscribe = TELECOMMAND_TOPIC; // Topic telecommand
    if(isEduroam){ initializeEduroamWifi(); }
    else{ initializeWifi(); }

    initializeLora();
    mqttClient.setServer(mqttBroker, mqttPort);
    mqttClient.setCallback(receivePackageUsignMqtt);
    initializeMQTT(); 
}

float ComsManager::getWiFiRSSI(bool isEduroam) {
    if (WiFi.status() == WL_CONNECTED){
        return WiFi.RSSI();
    }
    else{
        if(isEduroam){ initializeEduroamWifi(); }
        else{ initializeWifi(); }
    }
    return 0;
}
float ComsManager::getLoRaRSSI() {
    return radioClient.getRSSI();
}
float ComsManager::getLoRaSNR() {
    return radioClient.getSNR();
}

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
            // push UART data to all connected telnet clients
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

void ComsManager::sendTelemetryData(String telemetryJson){
    if(sendTelemetry){
        mqttClient.loop();
        if (!mqttClient.connected()) {
            
            initializeMQTT();
        }
        Serial.println(SENDING_HOUSEKEEPING_DATA);

        JsonDocument doc;
        deserializeJson(doc, telemetryJson);

        // Extract keys from the JSON object
        for (JsonPair keyValue : doc.as<JsonObject>()) {
            String mqttPublish = mqttTopic + "/"; 
            const char* key = keyValue.key().c_str(); 
            mqttPublish.concat(key);
            String jsonData = String(keyValue.value().as<String>());
            
            // Publish telemetry data on the constructed MQTT topic
            mqttClient.publish(mqttPublish.c_str(), jsonData.c_str());
        }

        sendTelemetry = false;
    }
}


String ComsManager::receivePackageUsignLora(){
    receivedFlag = false;
    // you can read received data as an Arduino String
    Serial.println("receivedFlag");
    String receivedCommand;
    int state = radioClient.readData(receivedCommand);

    if (state == RADIOLIB_ERR_NONE) {
        // packet was successfully received
        Serial.println(F("\n[SX1276] Received packet!"));

        // print RSSI (Received Signal Strength Indicator)
        Serial.print(F("[SX1276] RSSI:\t\t\t"));
        Serial.print(radioClient.getRSSI());
        Serial.println(F(" dBm"));

        // print SNR (Signal-to-Noise Ratio)
        Serial.print(F("[SX1276] SNR:\t\t\t"));
        Serial.print(radioClient.getSNR());
        Serial.println(F(" dB"));

        // print frequency error
        Serial.print(F("[SX1276] Frequency error:\t"));
        Serial.print(radioClient.getFrequencyError());
        Serial.println(F(" Hz"));

        // print data of the packet
        Serial.print(F("[SX1276] Data:\t\t"));
        Serial.println(receivedCommand);

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
        // packet was received, but is malformed
        Serial.println(F("[SX1276] CRC error!"));

    } else {
        // some other error occurred
        Serial.print(F("[SX1276] Failed, code "));
        Serial.println(state);
    }
    return receivedCommand;
}