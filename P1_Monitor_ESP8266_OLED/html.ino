void handleRoot() {
  String html = "<!DOCTYPE html><html lang='hu'><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='10'>";
  html += "<title>P1 Smart Meter v." +  String(CURRENT_VERSION) + "</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background: #121212; color: #e0e0e0; margin: 0; padding: 20px; }";
  html += ".container { max-width: 900px; margin: 0 auto; }";
  html += "h1 { text-align: center; color: #00e676;font-size:18px; }";
  html += "h2 { color: #80d8ff; border-bottom: 2px solid #333; padding-bottom: 5px; margin-top: 30px;font-size:14px; }";
  html += "table { width: 100%; border-collapse: collapse; margin-top: 10px; background: #1e1e1e; border-radius: 8px; overflow: hidden; }";
  html += "th, td { padding: 12px 15px; text-align: left; border-bottom: 1px solid #2c2c2c; }";
  html += "th { background-color: #263238; color: #fff; }";
  html += ".val { font-weight: bold; color: #00e676; }";
  html += ".obis { color: #ffb74d; font-family: monospace; }";
  html += "</style></head><body>";

  html += "<div class='container'>";
  html += "<h1>P1 SmartMeter v." + String(CURRENT_VERSION) + " adatok</h1>";

  // 1. TÁBLÁZAT: Összes Kiolvasott OBIS Kód
  html += "<h2>Összes Kiolvasott OBIS Adat</h2>";
  html += "<table>";
  html += "<tr><th>Megnevezés</th><th>OBIS Kód</th><th>Érték</th></tr>";

  if (allObisData.length() > 0) {
    html += allObisData;
  } else {
    html += "<tr><td colspan='2'><i>Várakozás a P1 adatokra...</i></td></tr>";
  }

  html += "</table>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
} // <-- ITT a handleRoot() vége!

void handleData() {
  server.send(200, "text/plain", allObisData);
} // <-- ITT a handleData() vége!