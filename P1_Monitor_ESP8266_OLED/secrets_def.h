// --- MQTT beállítások ---

// Verziószám megadása:
// ez jelenik meg a kijelzőn; MQTT-n ; WEBlapon
// Ez alapján dönti el hogy van-e újabb verzió
// Ne használj betűket csak a x.xx formátumot!

#define SECRET_VERSION             1.00

// MQTT szerver elérés címe:
// Cseréld le a saját adataiddal a YOUR_ kezdetű adatokat
// ha nem az alap portot használod azt is változtasd meg.

#define SECRET_MQTT_SERVER         "YOUR_MQTT_SERVER"
#define SECRET_MQTT_PORT           1883
#define SECRET_MQTT_USER           "YOUR_MQTT_USER"
#define SECRET_MQTT_PASSWORD       "YOUR_MQTT_PASSWORD"
#define SECRET_GITHUB_URL          "https://raw.githubusercontent.com/YOUR_USERNAME/YOUR_REPSTORIES/main/firmware_"