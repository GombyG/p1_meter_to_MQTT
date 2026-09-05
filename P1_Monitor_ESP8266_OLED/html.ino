void handleRaw() {
  String out = "=== P1 WEBES ADATOK HOSSZA ===\n";
  out += String(allObisData.length()) + " karakter\n\n";
  out += "=== GENERÁLT HTML KÁRTYÁK MINTÁJA ===\n";
  out += allObisData;
  
  server.send(200, "text/plain", out);
}

void handleRoot() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  String page = "<!DOCTYPE html><html lang='hu'><head><meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width,initial-scale=1.0'>";
  page += "<meta http-equiv='refresh' content='10'>";
  page += "<title>P1 Smart Meter</title><style>";
  page += "body{font-family:sans-serif;background:#121212;color:#e0e0e0;margin:0;padding:10px}";
  page += ".container{max-width:1100px;margin:0 auto}";
  page += "h1{text-align:center;color:#00e676;font-size:18px}";
  page += ".controls{display:flex;justify-content:center;gap:10px;margin-bottom:15px}";
  page += ".btn{background:#1e1e1e;border:1px solid #444;color:#aaa;padding:8px 16px;border-radius:15px;font-size:13px;cursor:pointer;-webkit-tap-highlight-color:transparent}";
  page += ".btn.act{background:#00e676;color:#121212;font-weight:bold}";
  page += ".grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(150px,1fr));gap:8px}";
  page += ".card{background:#1e1e1e;border-radius:8px;padding:10px;border:1px solid #2c2c2c;position:relative;display:flex;flex-direction:column;justify-content:space-between}";
  page += ".card-title{font-size:10px;color:#90caf9;text-transform:uppercase;margin-bottom:5px;padding-right:20px}";
  page += ".star{position:absolute;top:6px;right:6px;font-size:18px;color:#444;cursor:pointer;padding:4px;user-select:none;-webkit-tap-highlight-color:transparent}";
  page += ".card.is-fav .star{color:#ffd700}";
  page += ".card-value{font-size:15px;font-weight:bold;color:#00e676}";
  page += ".card-code{font-size:8px;color:#ffb74d;font-family:monospace;margin-top:5px}";
  
  // Szűrés CSS alapokon (ha Kedvencek módban vagyunk)
  page += ".fav-mode .card:not(.is-fav){display:none !important}";
  page += "@media(max-width:480px){.grid{grid-template-columns:1fr}}";
  page += "</style></head><body><div class='container'>";
  page += "<h1>P1 SmartMeter</h1>";
  page += "<div class='controls'>";
  page += "<button id='bA' class='btn act' onclick='setFilter(0)'>Összes</button>";
  page += "<button id='bF' class='btn' onclick='setFilter(1)'>★ Kedvencek</button>";
  page += "</div><div id='g' class='grid'>";
  
  server.sendContent(page);

  if (allObisData.length() > 0) {
    server.sendContent(allObisData);
  } else {
    server.sendContent("<div class='card'><div class='card-title'>Állapot</div><div class='card-value'>Várakozás adatra...</div></div>");
  }

  // Megbízható JavaScript szinkronizáció
  String js = "</div></div><script>";
  js += "let favs = JSON.parse(localStorage.getItem('p1_favs') || '[]');";
  js += "let showFavOnly = localStorage.getItem('p1_show_fav') === '1';";
  js += "let grid = document.getElementById('g');";
  
  // Szinkronizáló függvény: beállítja a csillagok állapotát és a szűrést
  js += "function syncUI(){";
  js += "  document.querySelectorAll('.card').forEach(c => {";
  js += "    let code = c.getAttribute('data-code');";
  js += "    if(code && favs.includes(code)){ c.classList.add('is-fav'); } else { c.classList.remove('is-fav'); }";
  js += "  });";
  js += "  if(showFavOnly){ grid.classList.add('fav-mode'); } else { grid.classList.remove('fav-mode'); }";
  js += "  document.getElementById('bA').className = 'btn' + (!showFavOnly ? ' act' : '');";
  js += "  document.getElementById('bF').className = 'btn' + (showFavOnly ? ' act' : '');";
  js += "}";
  
  // Csillag kattintás
  js += "window.tG = function(code, el){";
  js += "  let idx = favs.indexOf(code);";
  js += "  if(idx > -1){ favs.splice(idx, 1); } else { favs.push(code); }";
  js += "  localStorage.setItem('p1_favs', JSON.stringify(favs));";
  js += "  syncUI();";
  js += "};";
  
  // Szűrő gombok kattintása
  js += "window.setFilter = function(val){";
  js += "  showFavOnly = (val === 1);";
  js += "  localStorage.setItem('p1_show_fav', showFavOnly ? '1' : '0');";
  js += "  syncUI();";
  js += "};";
  
  // Betöltéskor azonnali szinkron
  js += "syncUI();";
  js += "</script></body></html>";

  server.sendContent(js);
  server.sendContent("");
}

void handleData() {
  server.send(200, "text/plain", allObisData);
}

void addObisCardToWeb(String cleanCode, String rawVal) {
  String obisName = getObisName(cleanCode);
  if (obisName.length() == 0) {
    obisName = "Kód: " + cleanCode;
  }

  String valWithUnit = rawVal;

  // Mértékegység hozzáadása a tiszta OBIS kód alapján
  if (valWithUnit.indexOf('*') != -1) {
    valWithUnit.replace("*", " ");
  } else {
    if (cleanCode == "1.0.0") {
      valWithUnit = formatP1Time(valWithUnit);
    } 
    else if (cleanCode.startsWith("1.8.") || cleanCode.startsWith("2.8.") || cleanCode.startsWith("15.8.")) {
      valWithUnit += " kWh";
    } 
    else if (cleanCode.startsWith("3.8.") || cleanCode.startsWith("4.8.") || cleanCode.startsWith("5.8.") || cleanCode.startsWith("6.8.")) {
      valWithUnit += " kvarh";
    }
    else if (cleanCode == "32.7.0" || cleanCode == "52.7.0" || cleanCode == "72.7.0") {
      valWithUnit += " V";
    } 
    else if (cleanCode == "31.7.0" || cleanCode == "51.7.0" || cleanCode == "71.7.0" || cleanCode == "31.4.0" || cleanCode == "51.4.0" || cleanCode == "71.4.0") {
      valWithUnit += " A";
    } 
    else if (cleanCode == "1.7.0" || cleanCode == "2.7.0" || cleanCode == "21.7.0" || cleanCode == "41.7.0" || cleanCode == "61.7.0" || cleanCode == "22.7.0" || cleanCode == "42.7.0" || cleanCode == "62.7.0") {
      valWithUnit += " kW";
    }
    else if (cleanCode == "14.7.0") {
      valWithUnit += " Hz";
    }
  }

  // HTML kártya hozzáfűzése a weblap pufferéhez
  allObisData += "<div class='card' data-code='" + cleanCode + "'>";
  allObisData += "<div class='card-title'>" + obisName + "</div>";
  allObisData += "<div class='card-value'>" + valWithUnit + "</div>";
  allObisData += "<div class='card-code'>" + cleanCode + "</div>";
  allObisData += "</div>";
}