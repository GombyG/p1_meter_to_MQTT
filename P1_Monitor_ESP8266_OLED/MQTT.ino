// Külső változók elérése
extern String savedMqttUser;
extern String savedMqttPass;

void handleMQTTConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        return; 
    }

    static unsigned long lastMqttRetry = 0;
    static unsigned long lastWifiSignalPublish = 0;

    if (!mqttClient.connected()) {
        if (millis() - lastMqttRetry > 10000) {
            lastMqttRetry = millis();
            waitForNtpSync();
            
            String clientId = "ESP8266_P1_" + chipId;

            const char* user = (savedMqttUser.length() > 0) ? savedMqttUser.c_str() : nullptr;
            const char* pass = (savedMqttPass.length() > 0) ? savedMqttPass.c_str() : nullptr;

            if (mqttClient.connect(clientId.c_str(), user, pass)) {
                String statusTopic = mqttBaseTopic + "/status";
                
                String currentTime = getFormattedTime();
                mqttClient.publish(statusTopic.c_str(), currentTime.c_str(), true);

                String versionTopic = mqttBaseTopic + "/ESP_software_version";                          
                mqttClient.publish(versionTopic.c_str(), String(CURRENT_VERSION).c_str(), true);

                // --- Térerő küldése (Itt ELEGENDŐ csak a publishMetric, mert az meghívja az addObisCard-ot!) ---
                publishMetric("WIFI_signal", String(WiFi.RSSI()));
                lastWifiSignalPublish = millis();
            }
        }
    } else {
        mqttClient.loop();

        if (millis() - lastWifiSignalPublish > 60000) {
            lastWifiSignalPublish = millis();
            publishMetric("WIFI_signal", String(WiFi.RSSI()));

            String versionTopic = mqttBaseTopic + "/ESP_software_version";                          
            mqttClient.publish(versionTopic.c_str(), String(CURRENT_VERSION).c_str(), true);
        }
    }
}

void publishMetric(const char* subtopic, const String& value) {
    if (value.length() == 0) return;

    // 1. MQTT KÜLDÉS
    if (mqttClient.connected()) {
        String fullTopic = mqttBaseTopic + "/" + String(subtopic);
        mqttClient.publish(fullTopic.c_str(), value.c_str(), true);
    }

    // 2. HTML KÁRTYA GENERÁLÁSA KÖZÖS FÜGGVÉNNYEL
    addObisCard(String(subtopic), value);
}