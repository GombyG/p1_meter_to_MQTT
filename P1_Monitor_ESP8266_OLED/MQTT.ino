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

            // A konstansok helyett a elmentett/beállított usert és jelszót használjuk!
            const char* user = (savedMqttUser.length() > 0) ? savedMqttUser.c_str() : nullptr;
            const char* pass = (savedMqttPass.length() > 0) ? savedMqttPass.c_str() : nullptr;

            if (mqttClient.connect(clientId.c_str(), user, pass)) {
                String statusTopic = mqttBaseTopic + "/status";
                
                String currentTime = getFormattedTime();
                mqttClient.publish(statusTopic.c_str(), currentTime.c_str(), true);

                String versionTopic = mqttBaseTopic + "/ESP_software_version";                          
                mqttClient.publish(versionTopic.c_str(), String(CURRENT_VERSION).c_str(), true);

                // --- ÚJ: Térerő küldése közvetlenül a sikeres csatlakozás után ---
                publishMetric("WIFI_signal", String(WiFi.RSSI()));
                lastWifiSignalPublish = millis();
            }
        }
    } else {
        mqttClient.loop();

        // --- ÚJ: Térerő időszakos frissítése (pl. 60 másodpercenként) ---
        if (millis() - lastWifiSignalPublish > 60000) {
            lastWifiSignalPublish = millis();
            //addObisCard("WIFI_signal", String(WiFi.RSSI()));
            publishMetric("WIFI_signal", String(WiFi.RSSI()));

            String versionTopic = mqttBaseTopic + "/ESP_software_version";                          
            mqttClient.publish(versionTopic.c_str(), String(CURRENT_VERSION).c_str(), true);
        }
    }
}

void publishMetric(const char* subtopic, const String& value) {
    if (value.length() == 0) return;

    // --- 1. MQTT KÜLDÉS ---
    if (mqttClient.connected()) {
        String fullTopic = mqttBaseTopic + "/" + String(subtopic);
        mqttClient.publish(fullTopic.c_str(), value.c_str(), true);
    }

    // --- 2. HTML KÁRTYA GENERÁLÁSA ---
    String cleanCode = String(subtopic);
    cleanCode.trim();

    String obisName = getObisName(cleanCode);
    if (obisName.length() == 0) obisName = cleanCode;

    String displayVal = value;

    if (displayVal.indexOf('*') != -1) {
        displayVal.replace("*", " ");
    } else {
        if (cleanCode == "1.0.0") displayVal = formatP1Time(displayVal);
        else if (cleanCode.startsWith("1.8.") || cleanCode.startsWith("2.8.") || cleanCode.startsWith("15.8.")) displayVal += " kWh";
        else if (cleanCode.startsWith("3.8.") || cleanCode.startsWith("4.8.") || cleanCode.startsWith("5.8.") || cleanCode.startsWith("6.8.")) displayVal += " kvarh";
        else if (cleanCode == "32.7.0" || cleanCode == "52.7.0" || cleanCode == "72.7.0") displayVal += " V";
        else if (cleanCode == "31.7.0" || cleanCode == "51.7.0" || cleanCode == "71.7.0" || cleanCode == "31.4.0" || cleanCode == "51.4.0" || cleanCode == "71.4.0") displayVal += " A";
        else if (cleanCode == "1.7.0" || cleanCode == "2.7.0" || cleanCode == "21.7.0" || cleanCode == "41.7.0" || cleanCode == "61.7.0" || cleanCode == "22.7.0" || cleanCode == "42.7.0" || cleanCode == "62.7.0") displayVal += " kW";
        else if (cleanCode == "14.7.0") displayVal += " Hz";
        else if (cleanCode == "WIFI_signal") displayVal += " dBm";
    }

    // A csillag SPAN már eleve be van építve a kártyába!
    tempObisData += "<div class='card' data-code='" + cleanCode + "'>";
    tempObisData += "<div class='card-title'>" + obisName + "</div>";
    tempObisData += "<span class='star' onclick='tG(\"" + cleanCode + "\", this)'>★</span>";
    tempObisData += "<div class='card-value'>" + displayVal + "</div>";
    tempObisData += "<div class='card-code'>" + cleanCode + "</div>";
    tempObisData += "</div>";
}