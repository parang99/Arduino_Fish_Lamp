#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include <time.h>

const char* ssid = "";
const char* password = "";

const char* ntpServer = "kr.pool.ntp.org";
const long gmtOffset_sec = 32400;
const int daylightOffset_sec = 0;

int relay = 25;
int relay_state = 0; // relay is not open. closed. 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


extern volatile unsigned long timer0_millis; // timer variable
unsigned long timeVal = 0; // present time storage variable
unsigned long previousVal = 0; // previous time storage variable
int myhour, mymin, mysec;
unsigned long relayPresent = 0; // present time storage variable for relay
unsigned long relayPrevious = 0; // previous time storage variable for relay

void setup() {
  Serial.begin(115200);
  
  Wire.begin(5, 4);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  

  Serial.println("\nWifi connecting...");
  display.println("Wifi connecting...\n");
  display.display();
  
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected. ");
  display.println("WiFi connected. ");
  display.display(); 

  pinMode(relay, OUTPUT);
  
  Serial.println("Time setting...");
  display.println("Time setting...");
  display.display();
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  myhour = timeinfo.tm_hour;
  mymin = timeinfo.tm_min;
  mysec = timeinfo.tm_sec;

  Serial.println("Time set clear.");
  display.println("Time set clear.");
  display.display();
  
  delay(1000);
}


void loop() {
  timeVal = millis();
  relayPresent = timeVal;
  
  // change every seconds
  if(timeVal - previousVal >= 3000){
    previousVal = timeVal;

    mysec = mysec + 3;
    if(mysec >= 60){
      mysec = mysec - 60;
      mymin++;
    }
    if(mymin >= 60){
      mymin = mymin - 60;
      myhour++;
    }
    if(myhour >= 24){
      myhour = 0;
      //timer0_millis = 0; // reset timer, compile error
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.printf("%02d:%02d:%02d", myhour, mymin, mysec);
    display.setCursor(80,43);
    Serial.println(relay_state);
    if(relay_state == 0){
      display.print("OFF");
    } else{
      display.print("ON");
    }
    display.display();
  }

  //check every minutes and change the relay state
  if(relayPresent - relayPrevious >= 60000){
    if(myhour >= 9 && myhour <= 19 && relay_state == 0){
      digitalWrite(relay, HIGH);
      relay_state = 1;
    }
    if((myhour < 9 || myhour > 19) && relay_state == 1){
      digitalWrite(relay, LOW);
      relay_state = 0;
    }
    relayPrevious = relayPresent;
  }
}
