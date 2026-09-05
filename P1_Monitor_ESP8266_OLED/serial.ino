void parseP1Telegram(String telegram) {
    int startIdx = 0;
    bool inHistoricalBlock = false;

    while (startIdx < telegram.length()) {
        int endIdx = telegram.indexOf('\n', startIdx);
        if (endIdx == -1) endIdx = telegram.length();
        
        String line = telegram.substring(startIdx, endIdx);
        line.trim();

        // 1. Történeti / Elszámolási blokk kiszűrése
        if (line.indexOf("98.1.0") != -1) {
            inHistoricalBlock = true;
        }

        if (inHistoricalBlock) {
            if (line.startsWith(")")) inHistoricalBlock = false;
            startIdx = endIdx + 1;
            continue;
        }

        // 2. Érvénytelen sorok, fejlécek és CRC eldobása
        if (line.length() == 0 || line.startsWith("!") || line.startsWith("AUX")) {
            startIdx = endIdx + 1;
            continue;
        }

        // 3. OBIS kód és érték szétválasztása
        int openParen = line.indexOf('(');
        if (openParen != -1) {
            String rawCode = line.substring(0, openParen);
            String rawVal = line.substring(openParen + 1);

            if (rawVal.endsWith(")")) {
                rawVal = rawVal.substring(0, rawVal.length() - 1);
            }

            int starIdx = rawVal.indexOf('*');
            if (starIdx != -1) {
                rawVal = rawVal.substring(0, starIdx);
            }

            String topicName = rawCode;
            topicName.replace("1-0:", "");
            topicName.replace("0-0:", "");

            // Javítva: topicName.c_str() használata
            publishMetric(topicName.c_str(), rawVal);

            if (rawCode.indexOf("1.7.0") != -1) lastPowerBE = rawVal;
            if (rawCode.indexOf("2.7.0") != -1) lastPowerKI = rawVal;
        }

        startIdx = endIdx + 1;
    }
}

void handleP1Port() {
    static String p1_telegram = "";
    static bool inTelegram = false;

    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == '/') { 
            inTelegram = true;
            p1_telegram = "";
            tempObisData = "";
        }

        if (inTelegram) {
            p1_telegram += c;

            if (c == '!') { 
                addObisCard("WIFI_signal", String(WiFi.RSSI()));
                parseP1Telegram(p1_telegram); 

                if (tempObisData.length() > 0) {
                    allObisData = tempObisData; 
                    tempObisData = "";
                }

                inTelegram = false;
                break;
            }
        }
    }
}