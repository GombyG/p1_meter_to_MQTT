// Segédfüggvény a dBm érték 0-100% közötti skálázásához
int getWifiQuality() {
  if (WiFi.status() != WL_CONNECTED) return 0;
  long rssi = WiFi.RSSI();
  if (rssi <= -100) return 0;
  if (rssi >= -50) return 100;
  return 2 * (rssi + 100); // -100 dBm = 0%, -50 dBm = 100%
}

void updateOledDisplay() {
    u8g2.clearBuffer();
    
   

    u8g2.drawUTF8(0, 10, (String("P1 Monitor v.") + String(CURRENT_VERSION, 2)).c_str());


    // Wi-Fi státusz: ha csatlakozva van, a százalékot mutatja (pl. "67%"), különben "??"
    String wifiStatus;
    if (WiFi.status() == WL_CONNECTED) {
        wifiStatus = String(getWifiQuality()) + "%";
    } else {
        wifiStatus = "??";
    }

    String mqttStatus = (mqttClient.connected()) ? "OK" : "??";

    // Kijelző felső sora: Kapcsolati státuszok (pl. "WiFi:67% MQTT:OK")
    
    u8g2.drawUTF8(0, 24, ("WiFi:" + wifiStatus + " MQTT:" + mqttStatus).c_str());
    
    // Fogyasztási adatok
    String powerBeStr = "Power BE: " + formatWatt(lastPowerBE);
    u8g2.drawUTF8(0, 40, powerBeStr.c_str());

    String powerKiStr = "Power KI: " + formatWatt(lastPowerKI);
    u8g2.drawUTF8(0, 58, powerKiStr.c_str());

    u8g2.sendBuffer();
}