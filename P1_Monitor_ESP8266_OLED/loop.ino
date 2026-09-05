void loop() {
     
    // DNS szerver kéréseinek feldolgozása (Captive Portal)
    dnsServer.processNextRequest();
    
    // 1. AP webszerver és OTA kezelése
    server.handleClient();
    ArduinoOTA.handle(); 

    if (!startupUpdateChecked && WiFi.status() == WL_CONNECTED) {
        startupUpdateChecked = true; // Azonnal letiltjuk, hogy a loop következő köreiben ne fusson le újra!
        lastFwCheckTime = millis();

        u8g2.clearBuffer();
        u8g2.drawUTF8(0, 25, "Automata");
        u8g2.drawUTF8(0, 40, "Firmware keresés..");
        u8g2.sendBuffer();
        
        delay(1000); // Rövid szünet, hogy a kijelző olvasható legyen
        checkForUpdates(); // Meghívja az update.ino-ban lévő keresőt
        
        // Ha nem talált frissítést és visszatér a kód, visszaállítjuk a rendes kijelzőt
        updateOledDisplay(); 
    }

    // --- ÚJ: Periodikus firmware-ellenőrzés (csak ha már túl vagyunk az induláskori ellenőrzésen) ---
    if (startupUpdateChecked && WiFi.status() == WL_CONNECTED && !inMenuMode) {
        if (millis() - lastFwCheckTime > FW_CHECK_INTERVAL_MS) {
            lastFwCheckTime = millis();
            
            u8g2.clearBuffer();
            u8g2.drawUTF8(0, 25, "Időzitett");
            u8g2.drawUTF8(0, 40, "Firmware keresés..");
            u8g2.sendBuffer();
            
            delay(1000);
            checkForUpdates();
            
            updateOledDisplay();
        }
    }
    
    // 2. Reset gomb figyelése
   
    
    handleResetButton();

    // 3. Soros port folyamatos olvasása (NEM blokkoló)
    handleP1Port();

    // 4. MQTT kapcsolat kezelése (újrapróbálkozás a háttérben)
    handleMQTTConnection();

    // 5. Mérés és OLED frissítés 10 másodpercenként (KIVÉVE ha menüben vagyunk)
    if (millis() - lastP1ReadTime > P1_READ_INTERVAL_MS) {
        lastP1ReadTime = millis();
        if (!inMenuMode) { // Csak akkor rajzoljuk felül a kijelzőt, ha nincs menü!
            updateOledDisplay();
        }
    }
}
