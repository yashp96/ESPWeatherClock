// Libraries
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <time.h>
#include <Ticker.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#include <Fonts/Tiny3x3a2pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/TomThumb.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/Org_01.h>

//SSID and Password of your WiFi router
const char* ssid = "YourSSID";
const char* password = "YourPassword";

const char* apiKey = "dad1066bbeb0853324834991a8a49cf6"; //openweathermap.org
const char* cityID = "1260730"; //"Palghar,IN"; 

const int timezone = 0;   
int dst = 0;

int currentTemp; //current temperature
float currentWindSpeed;
const char* currentWeather; //string that holds current weather condition
const char* weatherDescription;
float kmhspeed;
const char* City;

Ticker timeTick; //ticker for a repeated 1sec time update
bool updateTime = false; //check if ticker has fired

Ticker weatherCheck; //ticker for a repeated weather update
bool updateWeather = false; //check if ticker has fired

// PinOuts
#define TFT_CS     D1
#define TFT_RST    D0  
#define TFT_DC     D2
#define TFT_BL     D3
#define Buzzer     D4
#define BL_Butt    D6

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST); // uses RGB565

GFXcanvas16 *Clock;
GFXcanvas16 *Weather;

struct text 
{ //text render struct
  bool updated;
  String str;
  String oldStr;
};

text dateText;
text dayText;
text timeText;
text tempText;
text condText;
text windText;
bool rectUpdated;

int oldHour, currentHour;
bool hourChanged;

bool onBattery = false;
short LEDtime = 0;

void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(TFT_BL, OUTPUT);
  //digitalWrite(TFT_BL, LOW);
  analogWrite(TFT_BL, 50);
  lcd_init();
  
  pinMode(BL_Butt, INPUT_PULLUP);
  if(digitalRead(BL_Butt) == false)
  {
    onBattery = true;
    tft.println("\nON BATTERY");
    digitalWrite(TFT_BL, HIGH);
  }
  
  configTime(5.5*3600, 1800, "pool.ntp.org"); //19800 (utc/gmt offset, daylight saving offset, ntp server)
  //configtime is esp8266 function timezoneoffset = timezone*60*60
  Serial.println("\nWaiting for time sync");
  while (!time(nullptr)) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Synced");
  
  //initialize text rendering false
  timeText.updated = false;
  dateText.updated = false;
  dayText.updated = false;
  tempText.updated = false;
  condText.updated = false;
  windText.updated = false;
  rectUpdated = false;
    
  delay(500);

  timeTick.attach(1, tick);

  getWeather();

  if(digitalRead(BL_Butt) == false)
  {
    onBattery = true;
    tft.println("\nON BATTERY");
    digitalWrite(TFT_BL, HIGH);
  }

  weatherCheck.attach(600, check);//360//600
  
  tft.print("City: ");
  tft.print(City);
  delay(3000);
  tft.fillScreen(ST77XX_BLACK);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);

}

void loop() 
{
  if(!onBattery)
  {
//    int sensorValue = 0, brightness = 0;
//    for(int i = 0; i < 10; i++)
//    {
//      sensorValue = sensorValue + analogRead(A0);
//      delay(10);
//    }
//    //Serial.println(sensorValue/10);
//    if((sensorValue/10) > 30)
//    {
//      analogWrite(TFT_BL, 1000);
//    }
//    else if((sensorValue/10) > 12 && (sensorValue/10) < 20)
//    {
//      analogWrite(TFT_BL, 512);
//    }
//    else if((sensorValue/10) < 10)
//    {
//      analogWrite(TFT_BL, 128);
//    }
      autoBrightness();
  }
  if(onBattery)
  {
    bool din = digitalRead(BL_Butt);
    delay(100);
    if(!din)
    {
      //buzz();
      //digitalWrite(TFT_BL, HIGH);
      autoBrightness();
    }
     LEDtime++;
    if(LEDtime == 45)
    {
      LEDtime = 0;
      digitalWrite(TFT_BL, LOW);
    }
      //digitalWrite(TFT_BL, !digitalRead(BL_Butt));
  }

  if (updateTime) 
  {
    updateTime = false;
    clockDisplay();
    if(hourChanged)
    {
      buzz();
      hourChanged = false;
    }
  } 
  else if (updateWeather)
  {
    updateWeather = false;
    getWeather();
  }
  render();
  renderWeather();
}
