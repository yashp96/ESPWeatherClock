void sysConnected(bool c)
{
  int rssi = WiFi.RSSI();
  //Serial.println(rssi);
  Clock -> setFont(&TomThumb);
  Clock -> setTextSize(1);
  Clock -> setCursor(4, 7);
  if(c)
  {
    Clock -> setTextColor(0x67E6, 0x0000);
    Clock -> print("Connected");
    
    Clock -> setCursor(84, 7);
    Clock -> print("WiFi " + (String)rssi + "dbm");
  }
  else
  {
    Clock -> setTextColor(0xF986, 0x0000);
    Clock -> print("Disconnected");  
    
    Clock -> setCursor(88, 7);
    Clock -> print("WiFi " + (String)rssi + "db");
  }
}

void buzz()
{
    tone(D4, 8000, 100);
    delay(250);
    tone(D4, 8000, 100);
    delay(250);
    digitalWrite(D4, HIGH);
}

void autoBrightness()
{
    int sensorValue = 0, brightness = 0;
    for(int i = 0; i < 10; i++)
    {
      sensorValue = sensorValue + analogRead(A0);
      delay(10);
    }
    //Serial.println(sensorValue/10);
    if((sensorValue/10) > 30)
    {
      analogWrite(TFT_BL, 1000);
    }
    else if((sensorValue/10) > 12 && (sensorValue/10) < 20)
    {
      analogWrite(TFT_BL, 512);
    }
    else if((sensorValue/10) < 10)
    {
      analogWrite(TFT_BL, 128);
    }
}
