
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

// Hivatalos E.ON OBIS kódok azonosítása
String getObisName(String code) {
  // Általános és állapot kódok
  if (code.endsWith("1.0.0") || code.endsWith("1.0.0.255")) return "Idő";
  if (code.endsWith("42.0.0") || code.endsWith("42.0.0.255")) return "COSEM logikai eszköznév";
  if (code.endsWith("96.1.0") || code.endsWith("96.1.0.255")) return "Mérő gyári szám";
  if (code.endsWith("96.14.0") || code.endsWith("96.14.0.255")) return "Aktuális tarifa";
  if (code.endsWith("96.50.68") || code.endsWith("96.50.68.255") || code.endsWith("96.3.10") || code.endsWith("96.3.10.255")) return "Megszakító státusz";
  if (code.endsWith("17.0.0") || code.endsWith("17.0.0.255")) return "Limiter határérték";
  if (code.endsWith("96.13.0") || code.endsWith("96.13.0.255")) return "Áramszolgáltatói üzenet";

  // Hatásos import energiák (+A)
  if (code.endsWith("1.8.0") || code.endsWith("1.8.0.255")) return "Hatásos import energia (+A) totál";
  if (code.endsWith("1.8.1") || code.endsWith("1.8.1.255")) return "Hatásos import energia T1";
  if (code.endsWith("1.8.2") || code.endsWith("1.8.2.255")) return "Hatásos import energia T2";
  if (code.endsWith("1.8.3") || code.endsWith("1.8.3.255")) return "Hatásos import energia T3";
  if (code.endsWith("1.8.4") || code.endsWith("1.8.4.255")) return "Hatásos import energia T4";

  // Hatásos export energiák (-A)
  if (code.endsWith("2.8.0") || code.endsWith("2.8.0.255")) return "Hatásos export energia (-A) totál";
  if (code.endsWith("2.8.1") || code.endsWith("2.8.1.255")) return "Hatásos export energia T1";
  if (code.endsWith("2.8.2") || code.endsWith("2.8.2.255")) return "Hatásos export energia T2";
  if (code.endsWith("2.8.3") || code.endsWith("2.8.3.255")) return "Hatásos export energia T3";
  if (code.endsWith("2.8.4") || code.endsWith("2.8.4.255")) return "Hatásos export energia T4";

  // Kombinált és meddő energiák
  if (code.endsWith("15.8.0") || code.endsWith("15.8.0.255")) return "Abszolút hatásos energia (|+A|+|-A|)";
  if (code.endsWith("3.8.0") || code.endsWith("3.8.0.255")) return "Import meddő energia (+R)";
  if (code.endsWith("4.8.0") || code.endsWith("4.8.0.255")) return "Export meddő energia (-R)";
  if (code.endsWith("5.8.0") || code.endsWith("5.8.0.255")) return "Meddő energia QI (Import ind.)";
  if (code.endsWith("6.8.0") || code.endsWith("6.8.0.255")) return "Meddő energia QII (Import kap.)";
  if (code.endsWith("7.8.0") || code.endsWith("7.8.0.255")) return "Meddő energia QIII (Export ind.)";
  if (code.endsWith("8.8.0") || code.endsWith("8.8.0.255")) return "Meddő energia QIV (Export kap.)";

  // Pillanatnyi teljesítmények (Összesített & Fázisonkénti)
  if (code.endsWith("1.7.0") || code.endsWith("1.7.0.255")) return "Pillanatnyi import teljesítmény (+A)";
  if (code.endsWith("2.7.0") || code.endsWith("2.7.0.255")) return "Pillanatnyi export teljesítmény (-A)";
  if (code.endsWith("21.7.0") || code.endsWith("21.7.0.255")) return "Import teljesítmény (+A) L1";
  if (code.endsWith("41.7.0") || code.endsWith("41.7.0.255")) return "Import teljesítmény (+A) L2";
  if (code.endsWith("61.7.0") || code.endsWith("61.7.0.255")) return "Import teljesítmény (+A) L3";
  if (code.endsWith("22.7.0") || code.endsWith("22.7.0.255")) return "Export teljesítmény (-A) L1";
  if (code.endsWith("42.7.0") || code.endsWith("42.7.0.255")) return "Export teljesítmény (-A) L2";
  if (code.endsWith("62.7.0") || code.endsWith("62.7.0.255")) return "Export teljesítmény (-A) L3";

  // Pillanatnyi meddő teljesítmények
  if (code.endsWith("5.7.0") || code.endsWith("5.7.0.255")) return "Pillanatnyi meddő QI";
  if (code.endsWith("6.7.0") || code.endsWith("6.7.0.255")) return "Pillanatnyi meddő QII";
  if (code.endsWith("7.7.0") || code.endsWith("7.7.0.255")) return "Pillanatnyi meddő QIII";
  if (code.endsWith("8.7.0") || code.endsWith("8.7.0.255")) return "Pillanatnyi meddő QIV";

  // Feszültségek és Áramok
  if (code.endsWith("32.7.0") || code.endsWith("32.7.0.255")) return "Pillanatnyi feszültség L1";
  if (code.endsWith("52.7.0") || code.endsWith("52.7.0.255")) return "Pillanatnyi feszültség L2";
  if (code.endsWith("72.7.0") || code.endsWith("72.7.0.255")) return "Pillanatnyi feszültség L3";
  if (code.endsWith("31.7.0") || code.endsWith("31.7.0.255")) return "Pillanatnyi áram L1";
  if (code.endsWith("51.7.0") || code.endsWith("51.7.0.255")) return "Pillanatnyi áram L2";
  if (code.endsWith("71.7.0") || code.endsWith("71.7.0.255")) return "Pillanatnyi áram L3";

  // Áram korlátozás határértékek
  if (code.endsWith("31.4.0") || code.endsWith("31.4.0.255")) return "Áram korlátozás küszöb L1";
  if (code.endsWith("51.4.0") || code.endsWith("51.4.0.255")) return "Áram korlátozás küszöb L2";
  if (code.endsWith("71.4.0") || code.endsWith("71.4.0.255")) return "Áram korlátozás küszöb L3";

  // Teljesítmény tényezők és Frekvencia
  if (code.endsWith("13.7.0") || code.endsWith("13.7.0.255")) return "Teljesítmény tényező (CosPhi)";
  if (code.endsWith("33.7.0") || code.endsWith("33.7.0.255")) return "Teljesítmény tényező L1";
  if (code.endsWith("53.7.0") || code.endsWith("53.7.0.255")) return "Teljesítmény tényező L2";
  if (code.endsWith("73.7.0") || code.endsWith("73.7.0.255")) return "Teljesítmény tényező L3";
  if (code.endsWith("14.7.0") || code.endsWith("14.7.0.255")) return "Hálózati frekvencia";

  return code;
}

String formatObisToRow(String line) {
  line.replace("\r", "");
  line.replace("\n", "");
  line.trim();

  if (line.length() == 0 || line.startsWith("/") || line.startsWith("!") || line.indexOf("98.1.0") != -1) {
    return "";
  }

  String obisCode = "";
  String obisVal = "";

  int eqPos = line.indexOf('=');
  if (eqPos != -1) {
    obisCode = line.substring(0, eqPos);
    obisVal = line.substring(eqPos + 1);
  } else {
    int openParen = line.indexOf('(');
    if (openParen == -1) return "";

    obisCode = line.substring(0, openParen);
    obisVal = line.substring(openParen + 1);
    if (obisVal.endsWith(")")) obisVal = obisVal.substring(0, obisVal.length() - 1);
  }

  obisCode.trim();
  obisVal.trim();

  String obisName = getObisName(obisCode);

  // Mértékegység illesztés
  if (obisVal.indexOf('*') != -1) {
    obisVal.replace("*", " ");
  } else {
    if (obisCode.indexOf("1.0.0") != -1) {
      if (obisVal.indexOf('.') == -1) obisVal = formatP1Time(obisVal);
    } 
    else if (obisCode.indexOf("1.8.") != -1 || obisCode.indexOf("2.8.") != -1 || obisCode.indexOf("15.8.") != -1) obisVal += " kWh";
    else if (obisCode.indexOf("3.8.") != -1 || obisCode.indexOf("4.8.") != -1 || obisCode.indexOf("5.8.") != -1 || obisCode.indexOf("6.8.") != -1 || obisCode.indexOf("7.8.") != -1 || obisCode.indexOf("8.8.") != -1) obisVal += " kvarh";
    else if (obisCode.indexOf("32.7.0") != -1 || obisCode.indexOf("52.7.0") != -1 || obisCode.indexOf("72.7.0") != -1) obisVal += " V";
    else if (obisCode.indexOf("31.7.0") != -1 || obisCode.indexOf("51.7.0") != -1 || obisCode.indexOf("71.7.0") != -1 || obisCode.indexOf("31.4.0") != -1 || obisCode.indexOf("51.4.0") != -1 || obisCode.indexOf("71.4.0") != -1) obisVal += " A";
    else if (obisCode.indexOf("1.7.0") != -1 || obisCode.indexOf("2.7.0") != -1 || obisCode.indexOf("21.7.0") != -1 || obisCode.indexOf("41.7.0") != -1 || obisCode.indexOf("61.7.0") != -1 || obisCode.indexOf("22.7.0") != -1 || obisCode.indexOf("42.7.0") != -1 || obisCode.indexOf("62.7.0") != -1) obisVal += " kW";
    else if (obisCode.indexOf("14.7.0") != -1) obisVal += " Hz";
  }

  // Megtisztított kód eltárolása az attribútumban a gyors JS szűréshez
  String card = "<div class='card' data-code='" + obisCode + "'>";
  card += "<div class='card-title'>" + obisName + "</div>";
  card += "<div class='card-value'>" + obisVal + "</div>";
  card += "<div class='card-code'>" + obisCode + "</div>";
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