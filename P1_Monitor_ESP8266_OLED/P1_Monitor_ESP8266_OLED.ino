


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <U8g2lib.h>
#include <PubSubClient.h>   
#include <ArduinoOTA.h>     
#include "wifi_management.h"
#include <time.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

// --- Hardveres beállítások ---
#define OLED_SDA            14 // D5
#define OLED_SCL            12 // D6
#define OLED_RESET          U8X8_PIN_NONE
#define MODE_BTN_PIN        0  // D3 (GPIO0) - Reset gomb
#include "secrets.h"
// --- MQTT beállítások ---
#define MQTT_SERVER         SECRET_MQTT_SERVER
#define MQTT_PORT           SECRET_MQTT_PORT
#define MQTT_USER           SECRET_MQTT_USER
#define MQTT_PASSWORD       SECRET_MQTT_PASSWORD

#define MQTT_BASE_TOPIC     "power/EON/smartmeter"

// --- Működési beállítások ---
#define P1_READ_INTERVAL_MS 10000 

// --- Globális objektumok ---
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_SCL, OLED_SDA);



ESP8266WebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// --- Globális változók ---
const float CURRENT_VERSION = SECRET_VERSION;

const char* wifiConfigPath = "/wifi.txt";
unsigned long lastP1ReadTime = 0;
String lastPowerBE = "0";
String lastPowerKI = "0";
String chipId = "";
String mqttBaseTopic = "";
String savedMqttServer = "";
int savedMqttPort = MQTT_PORT;
String savedMqttUser = "";
String savedMqttPass = "";
String allObisData = "";
bool inMenuMode = false;
int selectedMenuItem = 1; 
unsigned long lastMenuActivity = 0;
bool startupUpdateChecked = false; 
extern DNSServer dnsServer;

String formatWatt(const String& kwString) {
    if (kwString.length() == 0 || kwString == "N/A") return "0 W";
    
    // String átalakítása float számmá (kW)
    float kw = kwString.toFloat();
    
    // Átváltás Watt-ra és egész számmá kerekítés
    int watt = round(kw * 1000.0);
    
    return String(watt) + " W";
}

String getFormattedTime() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);

    // Ha még nem szinkronizált az NTP szerverrel
    if (timeinfo->tm_year < (2020 - 1900)) {
        return "ONLINE (NTP hiányzik)";
    }

    char timeBuffer[25];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    return String(timeBuffer);
}

void waitForNtpSync() {
    // Legfeljebb 3 másodpercig vár az NTP szinkronra
    int timeout = 30; 
    while (time(nullptr) < 1000000000 && timeout > 0) {
        delay(100);
        timeout--;
    }
}

