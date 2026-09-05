
// Segédfüggvény a P1 YYMMDDhhmmss időbélyeg átalakítására (pl. 260905214500S -> 2026.09.05 21:45:00)
String formatP1Time(String rawTime) {
  if (rawTime.length() < 12) return rawTime;
  
  String yy = rawTime.substring(0, 2);
  String mm = rawTime.substring(2, 4);
  String dd = rawTime.substring(4, 6);
  String hh = rawTime.substring(6, 8);
  String min = rawTime.substring(8, 10);
  String ss = rawTime.substring(10, 12);

  return "20" + yy + "." + mm + "." + dd + " " + hh + ":" + min + ":" + ss;
}

String getObisName(String code) {
  code.trim();

  // Helper lambda / logika a pontos egyezéshez (sima vs .255)
  #define IS_OBIS(c, target) (c == target || c == target ".255")

  // --- 1. FESZÜLTSÉGEK ÉS ÁRAMOK (Előre sorolva, hogy ne csússzanak bele a 2.7.0-ba!) ---
  if (IS_OBIS(code, "32.7.0")) return "Pillanatnyi feszültség L1";
  if (IS_OBIS(code, "52.7.0")) return "Pillanatnyi feszültség L2";
  if (IS_OBIS(code, "72.7.0")) return "Pillanatnyi feszültség L3";
  if (IS_OBIS(code, "31.7.0")) return "Pillanatnyi áram L1";
  if (IS_OBIS(code, "51.7.0")) return "Pillanatnyi áram L2";
  if (IS_OBIS(code, "71.7.0")) return "Pillanatnyi áram L3";

  // --- 2. ÁLTALÁNOS ÉS ÁLLAPOT KÓDOK ---
  if (IS_OBIS(code, "1.0.0"))   return "Idő";
  if (IS_OBIS(code, "42.0.0"))  return "COSEM logikai eszköznév";
  if (IS_OBIS(code, "96.1.0"))  return "Mérő gyári szám";
  if (IS_OBIS(code, "96.14.0")) return "Aktuális tarifa";
  if (IS_OBIS(code, "96.50.68") || IS_OBIS(code, "96.3.10")) return "Megszakító státusz";
  if (IS_OBIS(code, "17.0.0"))  return "Limiter határérték";
  if (IS_OBIS(code, "96.13.0")) return "Áramszolgáltatói üzenet";

  // --- 3. HATÁSOS IMPORT ENERGIÁK (+A) ---
  if (IS_OBIS(code, "1.8.0")) return "Hatásos import energia (+A) totál";
  if (IS_OBIS(code, "1.8.1")) return "Hatásos import energia T1";
  if (IS_OBIS(code, "1.8.2")) return "Hatásos import energia T2";
  if (IS_OBIS(code, "1.8.3")) return "Hatásos import energia T3";
  if (IS_OBIS(code, "1.8.4")) return "Hatásos import energia T4";

  // --- 4. HATÁSOS EXPORT ENERGIÁK (-A) ---
  if (IS_OBIS(code, "2.8.0")) return "Hatásos export energia (-A) totál";
  if (IS_OBIS(code, "2.8.1")) return "Hatásos export energia T1";
  if (IS_OBIS(code, "2.8.2")) return "Hatásos export energia T2";
  if (IS_OBIS(code, "2.8.3")) return "Hatásos export energia T3";
  if (IS_OBIS(code, "2.8.4")) return "Hatásos export energia T4";

  // --- 5. KOMBINÁLT ÉS MEDDŐ ENERGIÁK ---
  if (IS_OBIS(code, "15.8.0")) return "Abszolút hatásos energia (|+A|+|-A|)";
  if (IS_OBIS(code, "3.8.0"))  return "Import meddő energia (+R)";
  if (IS_OBIS(code, "4.8.0"))  return "Export meddő energia (-R)";
  if (IS_OBIS(code, "5.8.0"))  return "Meddő energia QI (Import ind.)";
  if (IS_OBIS(code, "6.8.0"))  return "Meddő energia QII (Import kap.)";
  if (IS_OBIS(code, "7.8.0"))  return "Meddő energia QIII (Export ind.)";
  if (IS_OBIS(code, "8.8.0"))  return "Meddő energia QIV (Export kap.)";

  // --- 6. PILLANATNYI TELJESÍTMÉNYEK ---
  if (IS_OBIS(code, "1.7.0"))  return "Pillanatnyi import teljesítmény (+A)";
  if (IS_OBIS(code, "2.7.0"))  return "Pillanatnyi export teljesítmény (-A)";
  if (IS_OBIS(code, "21.7.0")) return "Import teljesítmény (+A) L1";
  if (IS_OBIS(code, "41.7.0")) return "Import teljesítmény (+A) L2";
  if (IS_OBIS(code, "61.7.0")) return "Import teljesítmény (+A) L3";
  if (IS_OBIS(code, "22.7.0")) return "Export teljesítmény (-A) L1";
  if (IS_OBIS(code, "42.7.0")) return "Export teljesítmény (-A) L2";
  if (IS_OBIS(code, "62.7.0")) return "Export teljesítmény (-A) L3";

  // --- 7. PILLANATNYI MEDDŐ TELJESÍTMÉNYEK ---
  if (IS_OBIS(code, "5.7.0")) return "Pillanatnyi meddő QI";
  if (IS_OBIS(code, "6.7.0")) return "Pillanatnyi meddő QII";
  if (IS_OBIS(code, "7.7.0")) return "Pillanatnyi meddő QIII";
  if (IS_OBIS(code, "8.7.0")) return "Pillanatnyi meddő QIV";

  // --- 8. ÁRAM KORLÁTOZÁS HATÁRÉRTÉKEK ---
  if (IS_OBIS(code, "31.4.0")) return "Áram korlátozás küszöb L1";
  if (IS_OBIS(code, "51.4.0")) return "Áram korlátozás küszöb L2";
  if (IS_OBIS(code, "71.4.0")) return "Áram korlátozás küszöb L3";

  // --- 9. TELJESÍTMÉNY TÉNYEZŐK ÉS FREKVENCIA ---
  if (IS_OBIS(code, "13.7.0")) return "Teljesítmény tényező (CosPhi)";
  if (IS_OBIS(code, "33.7.0")) return "Teljesítmény tényező L1";
  if (IS_OBIS(code, "53.7.0")) return "Teljesítmény tényező L2";
  if (IS_OBIS(code, "73.7.0")) return "Teljesítmény tényező L3";
  if (IS_OBIS(code, "14.7.0")) return "Hálózati frekvencia";

  // --- 10. EGYÉB KIEGÉSZÍTŐK ---
  if (code == "WIFI_signal")   return "Wi-Fi térerő";

  #undef IS_OBIS
  return code;
}

String formatObisToRow(String line) {
  line.replace("\r", "");
  line.replace("\n", "");
  line.trim();

  if (line.length() == 0 || line.startsWith("/") || line.startsWith("!") || line.indexOf("98.1.0") != -1) {
    return "";
  }

  String rawCode = "";
  String obisVal = "";

  int eqPos = line.indexOf('=');
  if (eqPos != -1) {
    rawCode = line.substring(0, eqPos);
    obisVal = line.substring(eqPos + 1);
  } else {
    int openParen = line.indexOf('(');
    if (openParen == -1) return "";

    rawCode = line.substring(0, openParen);
    obisVal = line.substring(openParen + 1);
    if (obisVal.endsWith(")")) obisVal = obisVal.substring(0, obisVal.length() - 1);
  }

  rawCode.trim();
  obisVal.trim();

  // OBIS kód megtisztítása az előtagoktól (pl. "1-0:32.7.0*255" -> "32.7.0")
  String cleanCode = rawCode;
  if (cleanCode.indexOf(':') != -1) {
    cleanCode = cleanCode.substring(cleanCode.indexOf(':') + 1);
  }
  if (cleanCode.indexOf('*') != -1) {
    cleanCode = cleanCode.substring(0, cleanCode.indexOf('*'));
  }

  // A megtisztított kóddal kérjük le a nevet
  String obisName = getObisName(cleanCode);

  // Mértékegység illesztés a tisztított kód alapján
  if (obisVal.indexOf('*') != -1) {
    obisVal.replace("*", " ");
  } else {
    if (cleanCode.indexOf("1.0.0") != -1) {
      if (obisVal.indexOf('.') == -1) obisVal = formatP1Time(obisVal);
    } 
    else if (cleanCode.indexOf("1.8.") != -1 || cleanCode.indexOf("2.8.") != -1 || cleanCode.indexOf("15.8.") != -1) obisVal += " kWh";
    else if (cleanCode.indexOf("3.8.") != -1 || cleanCode.indexOf("4.8.") != -1 || cleanCode.indexOf("5.8.") != -1 || cleanCode.indexOf("6.8.") != -1 || cleanCode.indexOf("7.8.") != -1 || cleanCode.indexOf("8.8.") != -1) obisVal += " kvarh";
    else if (cleanCode.indexOf("32.7.0") != -1 || cleanCode.indexOf("52.7.0") != -1 || cleanCode.indexOf("72.7.0") != -1) obisVal += " V";
    else if (cleanCode.indexOf("31.7.0") != -1 || cleanCode.indexOf("51.7.0") != -1 || cleanCode.indexOf("71.7.0") != -1 || cleanCode.indexOf("31.4.0") != -1 || cleanCode.indexOf("51.4.0") != -1 || cleanCode.indexOf("71.4.0") != -1) obisVal += " A";
    else if (cleanCode.indexOf("1.7.0") != -1 || cleanCode.indexOf("2.7.0") != -1 || cleanCode.indexOf("21.7.0") != -1 || cleanCode.indexOf("41.7.0") != -1 || cleanCode.indexOf("61.7.0") != -1 || cleanCode.indexOf("22.7.0") != -1 || cleanCode.indexOf("42.7.0") != -1 || cleanCode.indexOf("62.7.0") != -1) obisVal += " kW";
    else if (cleanCode.indexOf("14.7.0") != -1) obisVal += " Hz";
  }

  // Megtisztított data-code attribútum a gyors szűréshez
  String card = "<div class='card' data-code='" + cleanCode + "'>";
  card += "<div class='card-title'>" + obisName + "</div>";
  card += "<div class='card-value'>" + obisVal + "</div>";
  card += "<div class='card-code'>" + cleanCode + "</div>";
  card += "</div>";

  return card;
}
void parseP1Telegram(String telegram) {
    int startIdx = 0;
    bool inHistoricalBlock = false;

    while (startIdx < telegram.length()) {
        int endIdx = telegram.indexOf('\n', startIdx);
        if (endIdx == -1) endIdx = telegram.length();
        
        String line = telegram.substring(startIdx, endIdx);
        line.trim();

        // 1. Történeti / Elszámolási blokk (0-0:98.1.0) kiszűrése
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

            // Érték megtisztítása a mértékegységtől (csak a szám/érték marad)
            int starIdx = rawVal.indexOf('*');
            if (starIdx != -1) {
                rawVal = rawVal.substring(0, starIdx);
            }

            // Időbélyeg formázás
            if (rawCode.indexOf("1.0.0") != -1) {
                rawVal = formatP1Time(rawVal);
            }

            // MQTT Topic név kinyerése az OBIS kódból (pl. "1-0:1.8.0" -> "1.8.0")
            String topicName = rawCode;
            topicName.replace("1-0:", "");
            topicName.replace("0-0:", "");

            // Publikálás MQTT-re (.c_str() használatával a típus hiba javítására)
            publishMetric(topicName.c_str(), rawVal);

            // Segédváltozók frissítése
            if (rawCode.indexOf("1.7.0") != -1) lastPowerBE = rawVal;
            if (rawCode.indexOf("2.7.0") != -1) lastPowerKI = rawVal;
        }

        startIdx = endIdx + 1;
    }
}