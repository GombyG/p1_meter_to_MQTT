#include "wifi_management.h"
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern ESP8266WebServer server;
extern const char* wifiConfigPath;

DNSServer dnsServer;
const byte DNS_PORT = 53;

bool loadConfig(String& ssid, String& password, String& ip, String& gateway, String& subnet,
                String& mqttServer, String& mqttPort, String& mqttUser, String& mqttPass) {
    if (!SPIFFS.begin()) return false;
    if (!SPIFFS.exists(wifiConfigPath)) { SPIFFS.end(); return false; }
    
    File f = SPIFFS.open(wifiConfigPath, "r");
    if (!f) { SPIFFS.end(); return false; }
    
    ssid = f.readStringUntil('\n'); ssid.trim();
    password = f.readStringUntil('\n'); password.trim();
    ip = f.readStringUntil('\n'); ip.trim();
    gateway = f.readStringUntil('\n'); gateway.trim();
    subnet = f.readStringUntil('\n'); subnet.trim();
    
    mqttServer = f.readStringUntil('\n'); mqttServer.trim();
    mqttPort = f.readStringUntil('\n'); mqttPort.trim();
    mqttUser = f.readStringUntil('\n'); mqttUser.trim();
    mqttPass = f.readStringUntil('\n'); mqttPass.trim();
    
    f.close();
    SPIFFS.end();
    return ssid.length() > 0;
}

void saveConfig(const String& ssid, const String& password, const String& ip, const String& gateway, const String& subnet,
                const String& mqttServer, const String& mqttPort, const String& mqttUser, const String& mqttPass) {
    SPIFFS.begin();
    File f = SPIFFS.open(wifiConfigPath, "w");
    if (!f) return;
    
    f.println(ssid);
    f.println(password);
    f.println(ip);
    f.println(gateway);
    f.println(subnet);
    
    f.println(mqttServer);
    f.println(mqttPort);
    f.println(mqttUser);
    f.println(mqttPass);
    
    f.close();
    SPIFFS.end();
}

void startAPMode() {
    WiFi.mode(WIFI_AP_STA);
    
    IPAddress local_ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP("P1-METER", "12345678");

    dnsServer.start(DNS_PORT, "*", local_ip);

    server.on("/", HTTP_GET, []() {
        String ssid, password, ip, gateway, subnet;
        String mqttServer, mqttPort, mqttUser, mqttPass;
        
        loadConfig(ssid, password, ip, gateway, subnet, mqttServer, mqttPort, mqttUser, mqttPass);
        
        // Alapértelmezett értékek, ha még üresek lennének
        if (mqttServer.length() == 0) mqttServer = "lan.go1.hu";
        if (mqttPort.length() == 0)   mqttPort   = "41883";
        if (mqttUser.length() == 0)   mqttUser   = "nodered";
        if (mqttPass.length() == 0)   mqttPass   = "nodered";

        int n = WiFi.scanNetworks();
        String ssidOptions = "";
        bool foundSavedSsid = false;

        if (n > 0) {
            for (int i = 0; i < n; ++i) {
                String networkName = WiFi.SSID(i);
                int rssi = WiFi.RSSI(i);
                bool isSelected = (networkName == ssid);
                if (isSelected) foundSavedSsid = true;

                String selected = isSelected ? " selected" : "";
                ssidOptions += "<option value='" + networkName + "'" + selected + ">" 
                            + networkName + " (" + String(rssi) + " dBm)</option>";
            }
        }

        // Opció a kézi beírásra
        String manualSelected = (!foundSavedSsid && ssid.length() > 0) ? " selected" : "";
        ssidOptions += "<option value='__MANUAL__' " + manualSelected + ">-- Kézi megadás / Rejtett SSID --</option>";

        String html = R"====(<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name="viewport" content="width=device-width, initial-scale=1"><title>Eszköz Konfiguráció</title><style>body{font-family:Arial,sans-serif;background-color:#f7f7f7;display:flex;justify-content:center;align-items:center;min-height:100vh;margin:0;padding:20px 0;box-sizing:border-box}.container{background-color:#4CAF50;color:white;padding:25px 30px;border-radius:12px;box-shadow:0 0 15px rgba(0,0,0,0.2);text-align:center;width:90%;max-width:420px}h1{margin-bottom:15px;font-size:20px}h2{font-size:16px;margin:15px 0 5px 0;text-align:left;border-bottom:1px solid rgba(255,255,255,0.4);padding-bottom:3px}form{display:flex;flex-direction:column;gap:10px}label{font-size:13px;text-align:left}input[type="text"],input[type="password"],input[type="number"],select{font-size:14px;padding:8px;width:100%;box-sizing:border-box;border-radius:5px;border:none}input[type="submit"]{font-size:16px;padding:12px;margin-top:15px;cursor:pointer;background-color:white;color:#4CAF50;border:none;border-radius:5px;font-weight:bold}.form-group{display:flex;flex-direction:column;align-items:flex-start}#manual_group{display:none;margin-top:5px;width:100%;}</style><script>function toggleManualSsid(){var sel=document.getElementById('ssid_select');var man=document.getElementById('manual_group');if(sel.value==='__MANUAL__'){man.style.display='block';}else{man.style.display='none';}}window.onload=function(){toggleManualSsid();};</script></head><body><div class="container"><h1>P1 Mérő Beállítások</h1><form method='POST' action='/save'><h2>WiFi Beállítások</h2><div class="form-group"><label>WiFi Hálózat:</label><select id='ssid_select' name='ssid_select' onchange='toggleManualSsid()'>{{ssid_options}}</select><div id='manual_group'><label style='margin-top:5px;'>Kézi SSID / Hálózat neve:</label><input name='ssid_manual' type='text' value='{{ssid_manual_value}}' placeholder='Írd be a hálózat nevét'></div></div><div class="form-group"><label>WiFi Jelszó:</label><input name='password' type='password' value='{{password}}'></div><div class="form-group"><label>Fix IP (opcionális):</label><input name='ip' type="text" value='{{ip}}'></div><div class="form-group"><label>Gateway (opcionális):</label><input name='gateway' type="text" value='{{gateway}}'></div><div class="form-group"><label>Subnet (opcionális):</label><input name='subnet' type="text" value='{{subnet}}'></div><h2>MQTT Beállítások</h2><div class="form-group"><label>MQTT Szerver IP / Host:</label><input name='mqtt_server' type="text" value='{{mqtt_server}}'></div><div class="form-group"><label>MQTT Port:</label><input name='mqtt_port' type="number" value='{{mqtt_port}}'></div><div class="form-group"><label>MQTT Felhasználónév:</label><input name='mqtt_user' type="text" value='{{mqtt_user}}'></div><div class="form-group"><label>MQTT Jelszó:</label><input name='mqtt_pass' type="password" value='{{mqtt_pass}}'></div><input type='submit' value='Mentés és újraindítás'></form></div></body></html>)====";

        html.replace("{{ssid_options}}", ssidOptions);
        html.replace("{{ssid_manual_value}}", ssid);
        html.replace("{{password}}", password); 
        html.replace("{{ip}}", ip); 
        html.replace("{{gateway}}", gateway); 
        html.replace("{{subnet}}", subnet);
        
        html.replace("{{mqtt_server}}", mqttServer);
        html.replace("{{mqtt_port}}", mqttPort);
        html.replace("{{mqtt_user}}", mqttUser);
        html.replace("{{mqtt_pass}}", mqttPass);
        
        server.send(200, "text/html; charset=UTF-8", html);
    });

    server.on("/save", HTTP_POST, []() {
        // Ellenőrizzük, hogy a kézi megadást választotta-e a felhasználó
        String selectedSsid = server.arg("ssid_select");
        String finalSsid = (selectedSsid == "__MANUAL__") ? server.arg("ssid_manual") : selectedSsid;

        saveConfig(
            finalSsid, server.arg("password"), server.arg("ip"), server.arg("gateway"), server.arg("subnet"),
            server.arg("mqtt_server"), server.arg("mqtt_port"), server.arg("mqtt_user"), server.arg("mqtt_pass")
        );
        server.send(200, "text/html", "<!DOCTYPE html><html><head><meta charset='UTF-8'></head><body><h1>Beállítások elmentve. Az eszköz újraindul...</h1></body></html>");
        delay(1000);
        ESP.restart();
    });

    server.onNotFound([]() {
        server.sendHeader("Location", String("http://192.168.4.1/"), true);
        server.send(302, "text/plain", "");
    });

    server.begin();

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tf);
    u8g2.drawUTF8(0, 10, "WiFi Konfiguráció");
    u8g2.drawUTF8(0, 24, "SSID:P1-METER");
    u8g2.drawUTF8(0, 38, "PW:12345678");
    u8g2.drawUTF8(0, 52, "IP: 192.168.4.1");
    u8g2.sendBuffer();
}