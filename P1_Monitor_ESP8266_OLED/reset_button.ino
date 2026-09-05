void handleResetButton() {
    void checkForUpdates(); // Jelzés a fordítónak
    void drawMyMenu();      // Jelzés a fordítónak

    static unsigned long pressStartTime = 0;
    static bool longPressExecuted = false;
    static bool lastButtonState = HIGH;

    bool currentButtonState = digitalRead(MODE_BTN_PIN);

    // --- IDŐTÚLLÉPÉS (TIMEOUT) KEZELÉSE ---
    if (inMenuMode && (millis() - lastMenuActivity >= 10000)) {
        inMenuMode = false;
        updateOledDisplay(); 
    }

    // --- ÚJ GOMBNYOMÁS INDUL ---
    if (currentButtonState == LOW && lastButtonState == HIGH) {
        delay(50); // Debounce
        if (digitalRead(MODE_BTN_PIN) == LOW) {
            pressStartTime = millis();
            longPressExecuted = false;
        }
    }

    // --- GOMB FOLYAMATOSAN NYOMVA VAN ---
    if (currentButtonState == LOW && pressStartTime != 0) {
        // Ha elérte a 3 másodpercet ÉS ebben a nyomásban még nem futott le az akció
        if (!longPressExecuted && (millis() - pressStartTime >= 2000)) {
            longPressExecuted = true; // Szoftveres retesz
            lastMenuActivity = millis(); 

            if (!inMenuMode) {
                // 1. Belépés a menübe menet közben (nem kell elengedni!)
                inMenuMode = true;
                selectedMenuItem = 1;
                drawMyMenu();
            } else {
                // 2. Funkció indítása menet közben (nem kell elengedni!)
                u8g2.clearBuffer();
               
                
                if (selectedMenuItem == 1) { 
                    u8g2.drawUTF8(0, 10, "IP cím:");
                    u8g2.drawUTF8(0, 25, WiFi.localIP().toString().c_str());
                    u8g2.drawUTF8(0, 40, "Chip ID:"); 
                    u8g2.drawUTF8(0, 55, chipId.c_str()); 
                    u8g2.sendBuffer();
                    
                    // --- JAVÍTÁS: Megvárjuk, amíg elengedi a gombot, és csak utána várunk 10 mp-et ---
                    while(digitalRead(MODE_BTN_PIN) == LOW) { yield(); delay(10); }
                    delay(10000); 
                } 
                else if (selectedMenuItem == 2) { 
                    u8g2.drawUTF8(0, 30, "OTA keresés...");
                    u8g2.sendBuffer();
                    while(digitalRead(MODE_BTN_PIN) == LOW) { yield(); delay(10); } // Elengedés megvárása
                    delay(1000);
                    checkForUpdates();
                } 
                else if (selectedMenuItem == 3) { 
                    u8g2.drawUTF8(0, 30, "WiFi törölve!");
                    u8g2.sendBuffer();
                    SPIFFS.begin();
                    SPIFFS.remove(wifiConfigPath);
                    SPIFFS.end();
                    delay(2000);
                    ESP.restart();
                }
                inMenuMode = false; 
                updateOledDisplay();
                currentButtonState = HIGH; // Kényszerítjük az állapotot, hogy ne zavarjon be
            }
        }
    }

    // --- GOMB ELENGEDÉSE ---
    if (currentButtonState == HIGH && lastButtonState == LOW) {
        unsigned long holdTime = millis() - pressStartTime;
        pressStartTime = 0; // Alaphelyzetbe állítjuk az időzítőt

        // Ha RÖVID nyomás volt ÉS már a menüben vagyunk -> LÉPTETÉS
        if (!longPressExecuted && holdTime < 3000 && inMenuMode) {
            lastMenuActivity = millis(); // Aktiválódik a timeout újra
            selectedMenuItem++;
            if (selectedMenuItem > 3) {
                selectedMenuItem = 1; 
            }
            drawMyMenu();
        }
    }

    lastButtonState = currentButtonState;
}

// A menü kirajzoló segédfüggvény változatlanul tökéletes
void drawMyMenu() {
    u8g2.clearBuffer();
    
    u8g2.drawUTF8(16, 10, "Menü");
    
    if (selectedMenuItem == 1) u8g2.drawUTF8(0, 25, "->");
    u8g2.drawUTF8(16, 25, "1. IP cím");

    if (selectedMenuItem == 2) u8g2.drawUTF8(0, 40, "->");
    u8g2.drawUTF8(16, 40, "2. Firmware letöltés");

    if (selectedMenuItem == 3) u8g2.drawUTF8(0, 55, "->");
    u8g2.drawUTF8(16, 55, "3. WIFI reset");

    u8g2.sendBuffer();
}
