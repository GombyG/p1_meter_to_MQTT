// Érték kinyerése az OBIS kódból (mindig az ELSŐ zárójelpár tartalmát adja vissza)
String extractObisParam(const String& line, const String& obisCode) {
    int obisPos = line.indexOf(obisCode);
    if (obisPos == -1) return "";

    // Keresünk az OBIS kód utáni első nyitó zárójelet
    int startPos = line.indexOf('(', obisPos);
    if (startPos == -1) return "";

    // Érték vége: csillag (*), vagy az ELSŐ záró zárójel
    int endPos = line.indexOf('*', startPos);
    if (endPos == -1) {
        endPos = line.indexOf(')', startPos);
    }

    if (endPos != -1 && endPos > startPos) {
        return line.substring(startPos + 1, endPos);
    }

    return "";
}

// P1 telegram feldolgozása a pontos minta alapján
void parseP1Telegram(String telegram) {
    int startIdx = 0;
    bool inHistoricalBlock = false; // Flag az archív (0-0:98.1.0) blokk kiszűrésére

    while (startIdx < telegram.length()) {
        int endIdx = telegram.indexOf('\n', startIdx);
        if (endIdx == -1) endIdx = telegram.length();
        
        String line = telegram.substring(startIdx, endIdx);
        line.trim();

        // --- Történeti / Elszámolási blokk (0-0:98.1.0) felismerése ---
        if (line.indexOf("0-0:98.1.0") != -1) {
            inHistoricalBlock = true;
        }

        // Ha a blokkon belül vagyunk és elérjük a záró zárójelet, kilépünk belőle
        if (inHistoricalBlock && line.startsWith(")")) {
            inHistoricalBlock = false;
            startIdx = endIdx + 1;
            continue;
        }

        // Ha az archív blokkban vagyunk, figyelmen kívül hagyjuk a sorokat
        if (inHistoricalBlock) {
            startIdx = endIdx + 1;
            continue;
        }

        // --- Fejléc / Gyári számok ---
        if (line.indexOf("0-0:96.1.0") != -1) publishMetric("sorozatszam", extractObisParam(line, "0-0:96.1.0"));
        if (line.indexOf("0-0:1.0.0") != -1)  publishMetric("ido", extractObisParam(line, "0-0:1.0.0"));
        if (line.indexOf("0-0:96.14.0") != -1) publishMetric("tarifa", extractObisParam(line, "0-0:96.14.0"));

        // --- Hatásos Import Energia (+A) ---
        if (line.indexOf("1-0:1.8.0") != -1) publishMetric("fogyasztas_full", extractObisParam(line, "1-0:1.8.0"));
        if (line.indexOf("1-0:1.8.1") != -1) publishMetric("fogyasztas_t1", extractObisParam(line, "1-0:1.8.1"));
        if (line.indexOf("1-0:1.8.2") != -1) publishMetric("fogyasztas_t2", extractObisParam(line, "1-0:1.8.2"));
        
        // --- Hatásos Export Energia (-A) ---
        if (line.indexOf("1-0:2.8.0") != -1) publishMetric("visszataplalas_full", extractObisParam(line, "1-0:2.8.0"));
        if (line.indexOf("1-0:2.8.1") != -1) publishMetric("visszataplalas_t1", extractObisParam(line, "1-0:2.8.1"));
        if (line.indexOf("1-0:2.8.2") != -1) publishMetric("visszataplalas_t2", extractObisParam(line, "1-0:2.8.2"));

        // --- Pillanatnyi Teljesítmények & Áram ---
        if (line.indexOf("1-0:31.7.0") != -1)  publishMetric("aram_L1", extractObisParam(line, "1-0:31.7.0"));
        if (line.indexOf("1-0:51.7.0") != -1)  publishMetric("aram_L2", extractObisParam(line, "1-0:51.7.0"));
        if (line.indexOf("1-0:71.7.0") != -1) publishMetric("aram_L3", extractObisParam(line, "1-0:71.7.0"));

        if (line.indexOf("1-0:32.7.0") != -1) publishMetric("feszultseg_L1", extractObisParam(line, "1-0:32.7.0"));
        if (line.indexOf("1-0:52.7.0") != -1) publishMetric("feszultseg_L2", extractObisParam(line, "1-0:52.7.0"));
        if (line.indexOf("1-0:72.7.0") != -1) publishMetric("feszultseg_L3", extractObisParam(line, "1-0:72.7.0"));

        if (line.indexOf("1-0:21.7.0") != -1) publishMetric("power_BE_L1", extractObisParam(line, "1-0:21.7.0"));
        if (line.indexOf("1-0:41.7.0") != -1) publishMetric("power_BE_L2", extractObisParam(line, "1-0:41.7.0"));
        if (line.indexOf("1-0:61.7.0") != -1) publishMetric("power_BE_L3", extractObisParam(line, "1-0:61.7.0"));

        if (line.indexOf("1-0:22.7.0") != -1) publishMetric("power_KI_L1", extractObisParam(line, "1-0:22.7.0"));
        if (line.indexOf("1-0:42.7.0") != -1) publishMetric("power_KI_L2", extractObisParam(line, "1-0:42.7.0"));
        if (line.indexOf("1-0:62.7.0") != -1) publishMetric("power_KI_L3", extractObisParam(line, "1-0:62.7.0"));

        if (line.indexOf("1-0:14.7.0") != -1) publishMetric("frekvencia", extractObisParam(line, "1-0:14.7.0"));

        if (line.indexOf("1-0:13.7.0") != -1) publishMetric("power_faktor_full", extractObisParam(line, "1-0:13.7.0"));
        if (line.indexOf("1-0:33.7.0") != -1) publishMetric("power_faktor_L1", extractObisParam(line, "1-0:33.7.0"));
        if (line.indexOf("1-0:53.7.0") != -1) publishMetric("power_faktor_L2", extractObisParam(line, "1-0:53.7.0"));
        if (line.indexOf("1-0:73.7.0") != -1) publishMetric("power_faktor_L3", extractObisParam(line, "1-0:73.7.0"));

        if (line.indexOf("1-0:15.8.0") != -1)  publishMetric("power_full", extractObisParam(line, "1-0:15.8.0"));
        if (line.indexOf("1-0:1.7.0") != -1) { 
            lastPowerBE = extractObisParam(line, "1-0:1.7.0");
            publishMetric("power_BE", lastPowerBE); 
        }
        if (line.indexOf("1-0:2.7.0") != -1) { 
            lastPowerKI = extractObisParam(line, "1-0:2.7.0");
            publishMetric("power_KI", lastPowerKI); 
        }

        if (line.indexOf("1-0:5.7.0") != -1)  publishMetric("meddo_Q1", extractObisParam(line, "1-0:5.7.0"));
        if (line.indexOf("1-0:6.7.0") != -1)  publishMetric("meddo_Q2", extractObisParam(line, "1-0:6.7.0"));
        if (line.indexOf("1-0:7.7.0") != -1)  publishMetric("meddo_Q3", extractObisParam(line, "1-0:7.7.0"));
        if (line.indexOf("1-0:8.7.0") != -1)  publishMetric("meddo_Q4", extractObisParam(line, "1-0:8.7.0"));

        startIdx = endIdx + 1;
    }
}

// OBIS kódok feliratozása
String getObisName(String code) {
  if (code.endsWith("1.0.0")) return "Idő";
  if (code.endsWith("96.1.0")) return "Gyári szám";
  if (code.endsWith("96.14.0")) return "Aktuális tarifa";
  if (code.endsWith("96.50.68")) return "Kapcsoló állása";
  if (code.endsWith("17.0.0")) return "Limit teljesítmény";
  
  // Összesített energiák
  if (code.endsWith("1.8.0")) return "Összes fogyasztás (A+)";
  if (code.endsWith("1.8.1")) return "Fogyasztás T1 (Csúcs)";
  if (code.endsWith("1.8.2")) return "Fogyasztás T2 (Völgy)";
  if (code.endsWith("2.8.0")) return "Összes visszatáplálás (A-)";
  if (code.endsWith("2.8.1")) return "Visszatáplálás T1";
  if (code.endsWith("2.8.2")) return "Visszatáplálás T2";
  if (code.endsWith("15.8.0")) return "Abszolút energiaforgalom";
  
  // Pillanatnyi értékek
  if (code.endsWith("1.7.0")) return "Pillanatnyi fogyasztás";
  if (code.endsWith("2.7.0")) return "Pillanatnyi visszatáplálás";
  if (code.endsWith("14.7.0")) return "Hálózati frekvencia";
  
  // Feszültségek és Áramok
  if (code.endsWith("32.7.0")) return "Feszültség L1";
  if (code.endsWith("52.7.0")) return "Feszültség L2";
  if (code.endsWith("72.7.0")) return "Feszültség L3";
  if (code.endsWith("31.7.0")) return "Aramerősség L1";
  if (code.endsWith("51.7.0")) return "Aramerősség L2";
  if (code.endsWith("71.7.0")) return "Aramerősség L3";
  
  // Meddő energiák és egyéb
  if (code.endsWith("3.8.0")) return "Meddő energia (Q+)";
  if (code.endsWith("4.8.0")) return "Meddő energia (Q-)";
  
  return "Egyéb adatsor"; // Ha ismeretlen a kód
}

String formatObisToRow(String line) {
  line.trim();

  // 1. Fejléc, CRC, üres sorok ÉS A 98.1.0 ARCHÍV SOR KISZŰRÉSE
  if (line.length() == 0 || line.startsWith("!") || line.startsWith("AUX") || line.indexOf("98.1.0") != -1) {
  return "";
}

  // 2. Érvénytelen (non-ASCII) karakterek kiszűrése (pl. 96.13.0 hibás szimbólumai)
  String cleaned = "";
  for (size_t i = 0; i < line.length(); i++) {
    char c = line[i];
    if (c >= 32 && c <= 126) {
      cleaned += c;
    }
  }

  int openParen = cleaned.indexOf('(');
  if (openParen == -1) return "";

  String obisCode = cleaned.substring(0, openParen);
  String obisVal = cleaned.substring(openParen);
  String obisName = getObisName(obisCode); // Megnevezés felkeresése

  // 3. Háromoszlopos táblázat sorképzés
  String row = "<tr>";
  row += "<td><b>" + obisName + "</b></td>";
  row += "<td class='obis'>" + obisCode + "</td>";
  row += "<td>" + obisVal + "</td>";
  row += "</tr>";

  return row;
}