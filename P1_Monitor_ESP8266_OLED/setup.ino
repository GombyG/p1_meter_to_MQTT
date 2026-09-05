void setup() 
    {
        // NTP szerver adatai
        configTime("CET-1CEST,M3.5.0,M10.5.0/3", "hu.pool.ntp.org", "pool.ntp.org");
        
        // GOMB bemenet felhúzva
        pinMode(MODE_BTN_PIN, INPUT_PULLUP);
        
        // MQTT buffer méret
        mqttClient.setBufferSize(2048);

        // Egyedi chipId 
        chipId = String(ESP.getChipId(), HEX);
        chipId.toLowerCase();

        // MQTT alap topic
        mqttBaseTopic = String(MQTT_BASE_TOPIC) + "/" + chipId;

        // Hardveres Serial (115200 Baud, RX invertálva)
        Serial.begin(115200); 
        Serial.setRxBufferSize(1024);
        USC0(UART0) = USC0(UART0) | BIT(UCRXI);
        
        // OLED kijelző
        u8g2.begin();
        showLogo(5000);
        u8g2.setFont(u8g2_font_ncenB08_tf);

        handleResetButton();

        // Változók deklarálása (CSAK EGYSZER)
        String ssid, password, ipstr, gatewaystr, subnetstr;
        String mqttServer, mqttPort, mqttUser, mqttPass;
        
        // WIFI AP indítása
        if (!loadConfig(ssid, password, ipstr, gatewaystr, subnetstr, mqttServer, mqttPort, mqttUser, mqttPass)) {
            startAPMode();
            return; // Ha nincs mentett konfig, megáll az AP módban
        }

        // Mentett MQTT paraméterek átadása a globális változóknak
        savedMqttServer = (mqttServer.length() > 0) ? mqttServer : MQTT_SERVER;
        savedMqttPort = (mqttPort.length() > 0) ? mqttPort.toInt() : MQTT_PORT;
        savedMqttUser = (mqttUser.length() > 0) ? mqttUser : MQTT_USER;
        savedMqttPass = (mqttPass.length() > 0) ? mqttPass : MQTT_PASSWORD;
        
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tf);
        u8g2.drawUTF8(0, 30, "WiFi csatlakozás...");
        u8g2.sendBuffer();
        
        bool fixip = ipstr.length() > 0 && gatewaystr.length() > 0 && subnetstr.length() > 0;
        if (fixip) {
            IPAddress ip, gateway, subnet;
            IPAddress dns1(8, 8, 8, 8);
            IPAddress dns2(8, 8, 4, 4);
            if (ip.fromString(ipstr) && gateway.fromString(gatewaystr) && subnet.fromString(subnetstr)) {
                WiFi.config(ip, gateway, subnet, dns1, dns2);
            }
        }
        
        String hostName = "P1-SmartMeter_" + chipId;
        WiFi.hostname(hostName);
        WiFi.begin(ssid.c_str(), password.c_str());

        int timeout = 0;
        while (WiFi.status() != WL_CONNECTED && timeout < 30) {
            delay(500);
            timeout++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            setupOTA();
            // A betöltött MQTT szerver beállítása
            mqttClient.setServer(savedMqttServer.c_str(), savedMqttPort);
        } else {
            startAPMode();
        }
        server.on("/", handleRoot);
        server.on("/data", handleData);
        server.on("/raw", handleRaw);
        server.begin();
        updateOledDisplay();
}