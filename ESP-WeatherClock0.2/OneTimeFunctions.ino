void lcd_init()
{   
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST77XX_BLACK);
  delay(200);
  TextAtZero("System Init...", 0x5FE6, 1);
  WiFi.begin(ssid, password);     //Connect to your WiFi router

  // Wait for connection
  tft.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    tft.print(".");
    delay(1000);
  }  

  //If connection successful show IP address in serial monitor
  tft.println("");
  tft.println("Connected @ IP:");
  tft.println(WiFi.localIP());  //IP address assigned to your ESP
  tft.println();
  tft.println();
  tft.println();
}

void TextAtZero(char *text, uint16_t color, int t_size) 
{
  tft.setTextSize(t_size);
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.println(text);
}

void NewLineText(String text, uint16_t color, int t_size) 
{
  tft.setTextSize(t_size);
  tft.setTextColor(color, 0x0000);
  tft.setTextWrap(true);
  tft.println(text);
}
