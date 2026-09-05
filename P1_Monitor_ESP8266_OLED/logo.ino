void showLogo(uint32_t displayTimeMs) {
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.firstPage();
  do {
    // 1. Szöveg kirajzolása legfelülre (Y=12)
    u8g2.drawUTF8(0, 12, "GOMBY BUILDING SYSTEM");
    
    // 2. A 128x48-as logó kirajzolása pontos méretben (Y=16)
    u8g2.drawXBMP(0, 16, 128, 48, logo_bits);
    
  } while (u8g2.nextPage());

  // Várakozás a megadott ideig (WDT és Wi-Fi kiszolgálással)
  uint32_t startTime = millis();
  while (millis() - startTime < displayTimeMs) {
    yield();
  }
}