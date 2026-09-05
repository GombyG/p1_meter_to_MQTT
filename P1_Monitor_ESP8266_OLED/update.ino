void checkForUpdates() {
    Serial.print("Jelenlegi verzió: ");
    Serial.println(CURRENT_VERSION);

    WiFiClientSecure client;
    client.setInsecure();

    // --- 1. Legfrissebb verziószám lekérése a latest.txt fájlból ---
    String versionUrl = "https://raw.githubusercontent.com/" + String(GITHUB_REPO) + GITHUB_VERSION_FILE +"?nocache=" + String(millis());
    
    Serial.println(versionUrl);  

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

    String payload = http.getString();
    http.end();

    Serial.print("Payload: '");
    Serial.print(payload);
    Serial.println("'");

    // --- A "LATEST_VERSION" utáni szöveg kinyerése (string formában) ---
    int verPos = payload.indexOf("LATEST_VERSION");
    if (verPos == -1) {
        Serial.println("LATEST_VERSION nem található a válaszban.");
        return;
    }

    int i = verPos + strlen("LATEST_VERSION");
    while (i < (int)payload.length() && !isDigit(payload[i])) i++;

    int start = i;
    while (i < (int)payload.length() && (isDigit(payload[i]) || payload[i] == '.')) i++;

    String verStr = payload.substring(start, i);
    Serial.print("Kinyert verzió string: '");
    Serial.print(verStr);
    Serial.println("'");

    // --- Átváltás EGÉSZ SZÁMRA (pl. "1.58" -> 158), pontos, kerekítésmentes összehasonlításhoz ---
    int dotPos = verStr.indexOf('.');
    int remoteVersionInt;
    if (dotPos == -1) {
        remoteVersionInt = verStr.toInt() * 100;
    } else {
        String intPart = verStr.substring(0, dotPos);
        String fracPart = verStr.substring(dotPos + 1);
        while (fracPart.length() < 2) fracPart += "0";   // "5" -> "50"
        fracPart = fracPart.substring(0, 2);              // ha véletlenül 3+ tizedes lenne
        remoteVersionInt = intPart.toInt() * 100 + fracPart.toInt();
    }

    int currentVersionInt = round(CURRENT_VERSION * 100);

    Serial.print("Távoli verzió (int): ");
    Serial.print(remoteVersionInt);
    Serial.print("   Helyi verzió (int): ");
    Serial.println(currentVersionInt);

    // --- Összehasonlítás EGÉSZ SZÁMOKKAL ---
    if (remoteVersionInt <= currentVersionInt) {
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
    u8g2.drawUTF8(0, 10, (String("Új verzió: ") + verStr).c_str());
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