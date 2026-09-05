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

  // Fejléc és CSS (a villódzást gátló rejtő szabállyal)
  String page = "<!DOCTYPE html><html lang='hu'><head><meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width,initial-scale=1.0'>";
  page += "<meta http-equiv='refresh' content='10'>";
  page += "<title>P1 Smart Meter</title><style>";
  page += "body{font-family:sans-serif;background:#121212;color:#e0e0e0;margin:0;padding:10px}";
  page += ".container{max-width:1100px;margin:0 auto}";
  page += "h1{text-align:center;color:#00e676;font-size:18px}";
  page += ".controls{display:flex;justify-content:center;gap:10px;margin-bottom:15px}";
  page += ".btn{background:#1e1e1e;border:1px solid #444;color:#aaa;padding:6px 12px;border-radius:15px;font-size:12px;cursor:pointer}";
  page += ".btn.act{background:#00e676;color:#121212;font-weight:bold}";
  page += ".grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(150px,1fr));gap:8px}";
  page += ".card{background:#1e1e1e;border-radius:8px;padding:10px;border:1px solid #2c2c2c;position:relative;display:flex;flex-direction:column;justify-content:space-between}";
  page += ".card-title{font-size:10px;color:#90caf9;text-transform:uppercase;margin-bottom:5px;padding-right:15px}";
  page += ".star{position:absolute;top:8px;right:8px;font-size:14px;color:#444;cursor:pointer;user-select:none}";
  page += ".star.fav{color:#ffd700}";
  page += ".card-value{font-size:15px;font-weight:bold;color:#00e676}";
  page += ".card-code{font-size:8px;color:#ffb74d;font-family:monospace;margin-top:5px}";
  
  // VILLÓDZÁS MEGELŐZÉSE: Ha a fav-mode aktív, elrejtjük a nem kedvenceket CSS-ből
  page += ".fav-mode .card:not(.is-fav){display:none !important}";
  
  page += "@media(max-width:480px){.grid{grid-template-columns:1fr}}";
  page += "</style></head><body><div class='container'>";
  page += "<h1>P1 SmartMeter</h1>";
  page += "<div class='controls'>";
  page += "<button id='bA' class='btn act' onclick='f(0)'>Összes</button>";
  page += "<button id='bF' class='btn' onclick='f(1)'>★ Kedvencek</button>";
  page += "</div><div id='g' class='grid'>";
  
  server.sendContent(page);

  // Kártyák
  if (allObisData.length() > 0) {
    server.sendContent(allObisData);
  } else {
    server.sendContent("<div class='card'><div class='card-title'>Állapot</div><div class='card-value'>Várakozás adatra...</div></div>");
  }

  // Azonnal lefutó, villódzásmentes JavaScript
  String js = "</div></div><script>";
  js += "let favs=JSON.parse(localStorage.getItem('p1_f')||'[]');";
  js += "let sf=localStorage.getItem('p1_sf')==='1';";
  js += "let g=document.getElementById('g');";
  js += "if(sf)g.classList.add('fav-mode');";
  js += "document.querySelectorAll('.card').forEach(c=>{";
  js += "let k=c.getAttribute('data-code');if(!k)return;";
  js += "let isF=favs.includes(k);";
  js += "if(isF)c.classList.add('is-fav');";
  js += "let s=document.createElement('span');";
  js += "s.className='star'+(isF?' fav':'');s.innerHTML='★';";
  js += "s.onclick=(e)=>{e.stopPropagation();tG(k,s,c);};";
  js += "c.appendChild(s);";
  js += "});";
  js += "function tG(k,s,c){";
  js += "if(favs.includes(k)){favs=favs.filter(x=>x!==k);s.classList.remove('fav');c.classList.remove('is-fav');}";
  js += "else{favs.push(k);s.classList.add('fav');c.classList.add('is-fav');}";
  js += "localStorage.setItem('p1_f',JSON.stringify(favs));";
  js += "}";
  js += "function f(v){sf=v==1;localStorage.setItem('p1_sf',sf?'1':'0');";
  js += "document.getElementById('bA').className='btn'+(!sf?' act':'');";
  js += "document.getElementById('bF').className='btn'+(sf?' act':'');";
  js += "if(sf)g.classList.add('fav-mode');else g.classList.remove('fav-mode');";
  js += "}";
  js += "document.getElementById('bA').className='btn'+(!sf?' act':'');";
  js += "document.getElementById('bF').className='btn'+(sf?' act':'');";
  js += "</script></body></html>";

  server.sendContent(js);
  server.sendContent("");
}

void handleData() {
  server.send(200, "text/plain", allObisData);
}