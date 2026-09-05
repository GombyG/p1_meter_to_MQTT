void handleP1Port() {
    static String currentLine = "";
    static String p1_telegram = "";
    static String tempObisData = "";
    static bool inTelegram = false;

    while (Serial.available() > 0) {
        char c = Serial.read();

        // 1. Telegram kezdete
        if (c == '/') { 
            inTelegram = true;
            p1_telegram = "";
            tempObisData = "";
            currentLine = "";
        }

        if (inTelegram) {
            p1_telegram += c;

            // 2. Sorok gyűjtése és lezárása (\n vagy \r esetén)
            if (c == '\n' || c == '\r') {
                if (currentLine.length() > 0) {
                    String tableRow = formatObisToRow(currentLine);
                    if (tableRow.length() > 0) {
                        tempObisData += tableRow;
                    }
                    currentLine = ""; // Sor kiürítése
                }
            } else {
                currentLine += c; // Karakter hozzáadása a sorhoz
            }

            // 3. Telegram vége (!)
            if (c == '!') { 
                addObisCard(kod, ertek);
                // Ha az utolsó sorban (a ! karakter előtt/után) maradt feldolgozatlan adat
                if (tempObisData.length() > 0) {
                    allObisData = tempObisData; // Átadja a friss HTML-t a webszervernek
                    tempObisData = "";          // Kiüríti a következőknek
                    }
                }

                // Adatok áttöltése a webes globális változóba
                if (tempObisData.length() > 0) {
                    allObisData = tempObisData;
                }

                // MQTT küldés
                parseP1Telegram(p1_telegram); 

                inTelegram = false;
                currentLine = "";
                break;
            }
        }
    }
}