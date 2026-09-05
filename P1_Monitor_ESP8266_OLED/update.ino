void checkForUpdates() {
    Serial.print("Jelenlegi verzió: ");
    Serial.println(CURRENT_VERSION);

    WiFiClientSecure client;
    client.setInsecure();

    // --- 1. Legfrissebb verziószám lekérése a latest.txt fájlból ---
    String versionUrl = "https://raw.githubusercontent.com/" + String(GITHUB_REPO) + "main/latest.txt?nocache=" + String(millis());

    HTTPClient http;
    http.begin(client, versionUrl);
    http.addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    http.addHeader("Pragma", "no-cache");
    int httpCode = http.GET();
    Serial.printf("HTTP kód: %d\n", httpCode);

    if (httpCode != HTTP_CODE_OK) {
        Serial.println("Verzió lekérdezés sikertelen.");
        http.end();
        u8g2.clearBuffer();
        u8g2.drawUTF8(0, 25, "Verzióellenőrzés");
        u8g2.drawUTF8(0, 40, "sikertelen!");
        u8g2.sendBuffer();
        delay(3000);
        return;
    }

    String latestVersionStr = http.getString();
    latestVersionStr.trim();
    latestVersionStr.replace("v", "");
    http.end();

    Serial.print("Payload: '");
    Serial.print(latestVersionStr);
    Serial.println("'");

    float latestVersion = latestVersionStr.toFloat();
    Serial.print("Legfrissebb verzió a GitHubon: ");
    Serial.println(latestVersion);

    // --- 2. Összehasonlítás ---
    if (latestVersion <= CURRENT_VERSION || latestVersion <= 0) {
        u8g2.clearBuffer();
        u8g2.drawUTF8(0, 25, "A szoftver");
        u8g2.drawUTF8(0, 40, "naprakész!");
        u8g2.sendBuffer();
        Serial.println("Nincs újabb firmware a GitHubon.");
        delay(3000);
        return;
    }

    // --- 3. Firmware letöltése FIX néven ---
    String firmwareUrl = "https://raw.githubusercontent.com/" + String(GITHUB_REPO) + String(GITHUB_FW_NAME) + "?t=" + String(millis());

    u8g2.clearBuffer();
    u8g2.drawUTF8(0, 10, (String("Új verzió: ") + latestVersionStr).c_str());
    u8g2.drawUTF8(0, 24, "Letöltés...");
    u8g2.sendBuffer();

    Serial.print("Letöltés: ");
    Serial.println(firmwareUrl);

    ESPhttpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmwareUrl);

    if (ret == HTTP_UPDATE_FAILED) {
        Serial.printf("Frissítés hiba: %d - %s\n",
            ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        u8g2.clearBuffer();
        u8g2.drawUTF8(0, 25, "Frissites hiba!");
        u8g2.sendBuffer();
        delay(3000);
    }
    // Siker esetén az ESP automatikusan újraindul.
}