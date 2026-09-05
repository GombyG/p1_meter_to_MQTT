// -----------------------------------------------------------------------
//                          P1 monitor ESP9266 OLED
//     Villanyóra P1 portjának segítségével az adatok MQTT-re küldése
//            valamint az ESP weblapján az adatok megjelenítése
// Hardver leírás, forráskód: https://github.com/GombyG/p1_meter_to_MQTT
// -----------------------------------------------------------------------

// --- ESP WIFI ---
#include <ESP8266WiFi.h>
// --- ESP WEB szerver ---
#include <ESP8266WebServer.h>
// ---  ESP WEB kliens ---
#include <ESP8266HTTPClient.h>
// --- HTTP firmware letöltés ---
#include <ESP8266httpUpdate.h>
// --- OLED ---
#include <U8g2lib.h>
// --- MQTT ---
#include <PubSubClient.h>
// --- OTA ---   
#include <ArduinoOTA.h>     
// --- WIFI ---
#include "wifi_management.h"
// --- IDŐ ---
#include <time.h>

// --- Hardveres beállítások ---
#define OLED_SDA            14 // D5
#define OLED_SCL            12 // D6
#define OLED_RESET          U8X8_PIN_NONE
#define MODE_BTN_PIN        0  // D3 (GPIO0) - Reset gomb

// --- Saját adatok betöltése ---
#include "secrets.h"

// --- MQTT beállítások ---
#define MQTT_SERVER         SECRET_MQTT_SERVER
#define MQTT_PORT           SECRET_MQTT_PORT
#define MQTT_USER           SECRET_MQTT_USER
#define MQTT_PASSWORD       SECRET_MQTT_PASSWORD

// --- MQTT topic ahol az adatok megjelennek ---
#define MQTT_BASE_TOPIC     SECRET_MQTT_BASE_TOPIC
#define GITHUB_REPO         SECRET_GITHUB_REPO
#define GITHUB_FW_NAME      SECRET_GITHUB_FW_NAME
#define OTA_PASSWORD        SECRET_OTA_PASSWORD

// --- Működési beállítások ---
#define P1_READ_INTERVAL_MS 10000       // Olvasás 10mp-ként 
#define FW_CHECK_INTERVAL_MS 600000UL // Firmware ellenőrzés óránként (3600000 ms = 1 óra; 600000 ms = 10 perc)
// --- Globális objektumok ---
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_SCL, OLED_SDA);
ESP8266WebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// --- Globális változók ---
const float CURRENT_VERSION = SECRET_VERSION;
const char* wifiConfigPath = "/wifi.txt";
unsigned long lastP1ReadTime = 0;
unsigned long lastFwCheckTime = 0;
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

