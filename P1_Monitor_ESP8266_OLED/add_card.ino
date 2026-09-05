void addObisCard(const String& rawCode, String value) {
    String cleanCode = rawCode;
    cleanCode.trim();
    if (cleanCode.length() == 0 || value.length() == 0) return;

    String obisName = getObisName(cleanCode);
    if (obisName.length() == 0) obisName = cleanCode;

    // Mértékegységek és formázás
    if (value.indexOf('*') != -1) {
        value.replace("*", " ");
    } else {
        if (cleanCode == "1.0.0") value = formatP1Time(value);
        else if (cleanCode.startsWith("1.8.") || cleanCode.startsWith("2.8.") || cleanCode.startsWith("15.8.")) value += " kWh";
        else if (cleanCode.startsWith("3.8.") || cleanCode.startsWith("4.8.") || cleanCode.startsWith("5.8.") || cleanCode.startsWith("6.8.")) value += " kvarh";
        else if (cleanCode == "32.7.0" || cleanCode == "52.7.0" || cleanCode == "72.7.0") value += " V";
        else if (cleanCode == "31.7.0" || cleanCode == "51.7.0" || cleanCode == "71.7.0" || cleanCode == "31.4.0" || cleanCode == "51.4.0" || cleanCode == "71.4.0") value += " A";
        else if (cleanCode == "1.7.0" || cleanCode == "2.7.0" || cleanCode == "21.7.0" || cleanCode == "41.7.0" || cleanCode == "61.7.0" || cleanCode == "22.7.0" || cleanCode == "42.7.0" || cleanCode == "62.7.0") value += " kW";
        else if (cleanCode == "14.7.0") value += " Hz";
        else if (cleanCode == "WIFI_signal") value += " dBm";
    }

    // HTML kártya hozzáfűzése az ideiglenes pufferhez
    tempObisData += "<div class='card' data-code='" + cleanCode + "'>";
    tempObisData += "<div class='card-title'>" + obisName + "</div>";
    tempObisData += "<span class='star' onclick='tG(\"" + cleanCode + "\", this)'>★</span>";
    tempObisData += "<div class='card-value'>" + value + "</div>";
    tempObisData += "<div class='card-code'>" + cleanCode + "</div>";
    tempObisData += "</div>";
}