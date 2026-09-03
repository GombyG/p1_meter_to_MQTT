void handleP1Port() {
    static String currentLine = "";
    static String p1_telegram = "";
    static bool inTelegram = false;

    while (Serial.available() > 0) {
        char c = Serial.read();

        // 1. Telegram kezdete
        if (c == '/') { 
            inTelegram = true;
            p1_telegram = "";
            allObisData = ""; // Webes adatok törlése az új telegramnál
            currentLine = "";
        }

        if (inTelegram) {
            p1_telegram += c; // Gyűjtjük a teljes telegramot a parseP1Telegram számára
            currentLine += c; // Gyűjtjük az aktuális sort a webes táblázathoz

            // 2. Sor vége (Soremelés) -> Webes táblázat frissítése
            if (c == '\n') {
                String tableRow = formatObisToRow(currentLine);
                if (tableRow.length() > 0) {
                    allObisData += tableRow;
                }
                currentLine = ""; // Sor kiürítése a következőhöz
            }

            // 3. Telegram vége -> Feldolgozás és MQTT küldés
            if (c == '!') { 
                parseP1Telegram(p1_telegram); // Most már a TELJES telegramot kapja meg!
                inTelegram = false;
                currentLine = "";
                break;
            }
        }
    }
}