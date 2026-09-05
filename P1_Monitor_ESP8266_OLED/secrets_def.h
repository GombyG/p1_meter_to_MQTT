// --- MQTT beállítások ---

// MQTT szerver elérés címe:
// Cseréld le a saját adataiddal a YOUR_ kezdetű adatokat
// ha nem az alap portot használod azt is változtasd meg.

#define SECRET_MQTT_SERVER         "YOUR_MQTT_SERVER"
#define SECRET_MQTT_PORT           1883
#define SECRET_MQTT_USER           "YOUR_MQTT_USER"
#define SECRET_MQTT_PASSWORD       "YOUR_MQTT_PASSWORD"
// Alap topic amibe küldi az adatokat
#define SECRET_MQTT_BASE_TOPIC     "power/smartmeter"
// A saját GITHUB REPO-d neve
#define SECRET_GITHUB_REPO         "YOUR_GIT_REPOSTORIES"
// A firmware fájl neve amit le kell töltenie (A REPO nevet hozzáfűzi az előző sorból)
#define SECRET_GITHUB_FW_NAME      "main/P1_Monitor_ESP8266_OLED/build/esp8266.esp8266.generic/P1_Monitor_ESP8266_OLED.ino.bin"
// WIFI OTA frissítés esetén ezt a jelszót kell megadni a feltültés elött
#define SECRET_OTA_PASSWORD        "YOUR_OTA_PASSWORD"