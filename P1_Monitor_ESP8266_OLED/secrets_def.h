// --- MQTT beállítások ---


// Cseréld le a saját adataiddal a YOUR_ kezdetű adatokat
// ha nem az alap portot használod azt is változtasd meg.
// MQTT szerver adatai:
#define SECRET_MQTT_SERVER         "YOUR_MQTT_SERVER"   // pld. IP cím vagy domain  (192.168.1.1 /  valamidomain.hu)
#define SECRET_MQTT_PORT           1883                 // nem kell idézőjelbe tenni 
#define SECRET_MQTT_USER           "YOUR_MQTT_USER"     // MQTT Felhasználónév 
#define SECRET_MQTT_PASSWORD       "YOUR_MQTT_PASSWORD" // MQTT Jelszó
#define SECRET_MQTT_BASE_TOPIC     "power/smartmeter"   //  Ebbe a topicba lesz az adat a chipID val kiegészítve pld. power/smartmeter/23d22

// A saját GITHUB REPO-d neve
#define SECRET_GITHUB_REPO         "YOUR_GIT_REPOSTORIES"
// A firmware fájl neve amit le kell töltenie (A REPO nevet hozzáfűzi az előző sorból)
#define SECRET_GITHUB_FW_NAME      "main/P1_Monitor_ESP8266_OLED/build/esp8266.esp8266.generic/P1_Monitor_ESP8266_OLED.ino.bin"
// A firmware ellenőrző ebből a fájlból veszi a verziószámot (a program része)
#define SECRET_GITHUB_VERSION_FILE "main/P1_Monitor_ESP8266_OLED/latest_fw_version.h"

// WIFI OTA frissítés esetén ezt a jelszót kell megadni a feltöltés elött
#define SECRET_OTA_PASSWORD        "YOUR_OTA_PASSWORD" 
