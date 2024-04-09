/*
 * Copy this template into arduino_secrets.h and change values accordingly.
 *
 * This file holds settings following the Arduino convention, see
 * https://docs.arduino.cc/arduino-cloud/tutorials/store-your-sensitive-data-safely-when-sharing
 *
 */

// Settings: Wi-Fi credentials
#define SECRET_OTHER_WIFI_SSID "FRITZ!Box 7520 HL"                      // Wifi SSID
#define SECRET_OTHER_WIFI_PASSWORD "79946432715246805999"               // Wifi Password
#define SECRET_EDUROAM_WIFI_SSID "eduroam"                              // Eduroam SSID
#define SECRET_EDUROAM_WIFI_PASSWORD    "********"                      // University account password
#define SECRET_EDUROAM_WIFI_ANONYMOUSID "wlan@tu-berlin.de"             // Don't change, only used for Eduroam
#define SECRET_EDUROAM_WIFI_EDUROAMID   "********@tu-berlin.de"         // TU account name, only used for Eduroam

// Settings: MQTT
#define SECRET_MQTT_BROKER   "heide.bastla.net"                         // Server hostname (FQDN)
#define SECRET_MQTT_PORT     8883                                       // Server TLS port
#define SECRET_MQTT_USER     "mse23"                                    // Server credentials
#define SECRET_MQTT_PASSWORD "goofy"
#define SECRET_MQTT_PREFIX   "cadse"                                    // Prefix for topic and client ID
#define SECRET_MQTT_YEAR     2023                                       // Current year (beginning of course)
#define SECRET_MQTT_BOARDID  16                                         // [ID]: Assigned PCB number