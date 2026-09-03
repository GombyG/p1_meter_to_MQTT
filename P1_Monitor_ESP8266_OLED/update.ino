
void checkForUpdates() {
    Serial.print("Jelenlegi verzió: ");
    Serial.println(CURRENT_VERSION);
    WiFiClientSecure client;
    client.setInsecure();
    ESPhttpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    // FENTRŐL LEFELÉ KERESÉS: Elindulunk +10 verziólépéssel feljebbről 
    for (int i = 10; i >= 1; i--) {
        float test_version = CURRENT_VERSION + (i * 0.01);
        // BOMBABIZTOS STRING FORMÁZÁS: 
        // A float kerekítési hibák elkerülésére fixen 2 tizedesjeggyel alakítjuk szöveggé (pl. "1.24") 
        char version_str[10];
        dtostrf(test_version, 1, 2, version_str);
        // Összerakjuk a te pontos, működő GitHub elérésedet: 
         String test_url = String(SECRET_GITHUB_URL) + "firmware_" + String(version_str) + ".bin?t=" + String(millis());
        Serial.print("Keresés a GitHubon: ");
        Serial.println(test_url);
        // Elindítjuk a letöltési kísérletet 
        t_httpUpdate_return ret = ESPhttpUpdate.update(client, test_url);
        // Ha a fájl létezik, az ESP letölti és itt ÚJRAINDUL.
        // Ha nem létezik (404), a ciklus megy tovább lefelé a következő verzióra. 
        if (ret == HTTP_UPDATE_FAILED) {
            if (ESPhttpUpdate.getLastError() != 404 && ESPhttpUpdate.getLastError() != HTTPC_ERROR_CONNECTION_REFUSED) {
                Serial.printf("Hiba a(z) %s verziónál: %d\n", version_str, ESPhttpUpdate.getLastError());
                u8g2.clearBuffer();
                u8g2.setFont(u8g2_font_ncenB08_tf);
                u8g2.drawUTF8(0, 10, (String("ver. ") + String(atof(version_str) - 0.01, 2)).c_str());
                u8g2.drawUTF8(0, 24, "Verzió keresése..");
                u8g2.drawUTF8(0, 40, "Várjon!");
                u8g2.sendBuffer();
            }
        }
    } // Ha a ciklus teljesen lefutott és nem indult újra a chip, akkor nincs újabb verzió fent 
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tf);
    u8g2.drawUTF8(0, 25, "A szoftver");
    u8g2.drawUTF8(0, 40, "naprakész!");
    u8g2.sendBuffer();
    Serial.println("Nincs újabb firmware a GitHubon.");
    delay(3000);
}