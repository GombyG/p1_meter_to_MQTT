// ArduinoOTA beállítása
void setupOTA() {
    String otaHost = "P1-Smartmeter_" + chipId;
    ArduinoOTA.setHostname(otaHost.c_str());
    ArduinoOTA.setPassword("admin"); 

    ArduinoOTA.onStart([]() {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tf);
        u8g2.drawUTF8(0, 20, "OTA Frissités...");
        u8g2.sendBuffer();
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tf);
        u8g2.drawUTF8(0, 10, "OTA Frissités...");
        String pct = String(progress / (total / 100)) + "%";
        u8g2.drawUTF8(0, 40, pct.c_str());
        u8g2.sendBuffer();
    });

    ArduinoOTA.onEnd([]() {
        u8g2.clearBuffer();
        u8g2.drawUTF8(0, 30, "Frissités Kész!");
        u8g2.sendBuffer();
    });

    ArduinoOTA.onError([](ota_error_t error) {
        u8g2.clearBuffer();
        u8g2.drawStr(0, 30, "OTA Hiba!");
        u8g2.sendBuffer();
    });

    ArduinoOTA.begin();
}


