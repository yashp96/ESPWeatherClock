void tick() 
{
  updateTime = true;
}

void check() 
{
  updateWeather = true;
}

void render()
{
  if (dayText.updated || dateText.updated || timeText.updated || rectUpdated)
  {
    Clock = new GFXcanvas16(128,64);
    Clock -> fillScreen(0x0000);
    
    sysConnected(dayText.updated || dateText.updated || timeText.updated || rectUpdated);
    timeText.updated = false;
    dateText.updated = false;
    dayText.updated = false;
    rectUpdated = false;
    
    Clock -> setFont(&FreeSansBold12pt7b);
    Clock -> setTextColor(0xFFFF, 0x0000);
    Clock -> setTextSize(1);
    Clock -> setCursor(34, 32);
    Clock -> print(timeText.str);
    Clock -> setTextSize(1);
    Clock -> setFont(&FreeMono9pt7b);
    Clock -> setCursor(16, 56);
    Clock -> print(dayText.str + " " + dateText.str);

    Clock -> drawLine(0,40, 128,40,0x94DB);
    Clock -> drawRect(0,0, 128,64,0x94DB);
    tft.drawRGBBitmap(0, 0,Clock -> getBuffer(), 128, 64);
    delete Clock;
  }
}

void renderWeather()
{
  //if (condText.updated ||tempText.updated || rectUpdated)
  {
    tempText.updated = false;
    condText.updated = false;
    windText.updated = false;
    rectUpdated = false;

    Weather = new GFXcanvas16(128,64);
    Weather -> fillScreen(0x0000);

    Weather -> setTextColor(0xFFFF, 0x0000);
    Weather -> setFont(&TomThumb);
    Weather -> setTextSize(1);
    Weather -> setCursor(10, 7);
    Weather -> print("Temperature");
    Weather -> setCursor(68, 7);
    Weather -> print("Weather");
    Weather -> setCursor(68, 39);
    Weather -> print("Wind");
    
    Weather -> setFont(&FreeSansBold12pt7b);
    Weather -> setTextColor(0xFFFF, 0x0000);
    Weather -> setTextSize(1);
    Weather -> setCursor(20, 32);
    Weather -> print(String(currentTemp));
    Weather -> drawCircle(25,40, 2, 0xFFFF);
    Weather -> setCursor(26, 58);
    Weather -> print("C");

    Weather -> setFont();
    Weather -> setCursor(72, 18);
    Weather -> print(currentWeather);

    Weather -> setFont();
    Weather -> setCursor(72, 48);
    Weather -> print(String(kmhspeed) + "Km/h");
    
    Weather -> drawLine(64,32, 128,32,0xFF43); //horizontol
    Weather -> drawLine(64,0, 64,64,0xFF43);  //vertical
    Weather -> drawRect(0,0, 128,64,0xFF43);
    tft.drawRGBBitmap(0, 64,Weather -> getBuffer(), 128, 64);
    delete Weather;
  }
}

void clockDisplay()
{
  time_t now = time(nullptr);
  //Serial.println(ctime(&now));
  String t = ctime(&now);
  Serial.println(t);

  struct tm * timeinfo;
  timeinfo = localtime (&now);

  setClockTime(timeinfo);
  setClockDate(timeinfo);
  setClockDay(timeinfo);
}

void getWeather()
{
  getWeatherCurrent();

  String weatherCond = String(currentWeather);
//  if (weatherCond.length() < 1) 
//  {
//    weatherCond = "-";
//  }

//  if (rainCond(weatherCond))
//  {
//    rectUpdated = true;
//  } 
//  else 
  {
    rectUpdated = true;
  }

  Serial.println(weatherCond);
  Serial.println(weatherDescription);  

  String weatherTemp = String(currentTemp);
  if (weatherTemp.length() < 1) 
  {
    weatherTemp = "-";
  }
  Serial.println(weatherTemp);

  //convert to km/h
  // float kmhspeed = currentWindSpeed * 3.6;
  kmhspeed = currentWindSpeed * 3.6;
  static char outstr[8];
  dtostrf(kmhspeed,4, 1, outstr);
  
  String windSpeed = String(outstr);
  if (windSpeed.length() < 1)
  {
    windSpeed = "-";
  }
  windSpeed.concat(" km/h");
  Serial.println(windSpeed);
}
