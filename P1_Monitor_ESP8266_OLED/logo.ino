void logo_on(uint32_t duration_ms) {
  display.clearDisplay(); 

  // Szöveg a felső sárga sávba
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 4);            
  display.println("GOMBY BUILDING SYSTEM");  

  // Logó az alsó kék sávba
  display.drawBitmap(0, 16, epd_bitmap_128x48, 128, 48, SSD1306_WHITE);
  display.display(); 

  // Várakozás a kért ideig (pl. 2000 ms = 2 másodperc)
  delay(duration_ms);

  // Miután letelt az idő, letöröljük a kijelzőt a főprogramnak
  display.clearDisplay();
  display.display();
}