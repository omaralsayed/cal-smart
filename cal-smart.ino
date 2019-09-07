#include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h>   
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define TS_MINX 122
#define TS_MINY 111
#define TS_MAXX 942
#define TS_MAXY 890

#define YP A3
#define XM A2
#define YM 9
#define XP 8

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

const int optOne = 160;   
const int optTwo = 280;  

volatile int currentPage = 0;  
volatile int cal = 0;           

volatile bool buttonEnabled = true;  
volatile bool homePage = true;
volatile bool homeButton = false;    
volatile bool pageOne = false;

void Home(){
// 128x64  display
  tft.fillScreen(0xFFFF); // Clear
  tft.drawRect(0, 0, 319, 240, 0xFFE0);  
  tft.setCursor(60, 90);
  tft.setTextColor(0x0000);
  tft.setTextSize(4);
  tft.print("calSMART");

  tft.fillRect(50, 180, 230, 40, 0xF800);
  tft.drawRect(50, 180, 230, 40, 0x0000);
  tft.setCursor(60, 190);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.print("Calorie Count: ");
  tft.print(cal);
}

void PageOne(){
  tft.fillScreen(0xFFFF);

  // LHS
  tft.fillRect(tft.width() / 10, tft.height() / 4, 120, 40, 0xF800);
  tft.drawRect(tft.width() / 10, tft.height() / 4, 120, 40, 0x0000);
  tft.setCursor((tft.width() / 10) + 10, (tft.height() / 4) + 10); // Offset: 10
  tft.print("optTwo(280)");

  // RHS
  tft.fillRect(tft.width() / 2, tft.height() / 4, 120, 40, 0xFFE0);
  tft.drawRect(tft.width() / 2, tft.height() / 4, 120, 40, 0x0000);
  tft.setCursor((tft.width() / 2) + 10, (tft.height() / 4) + 10); // Offset: 10
  tft.print("optOne(160)");
}

void setup() {
  Serial.begin(9600);
  tft.reset();
 
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(1);
  Home();
}

void loop() {  
  TSPoint p = ts.getPoint();
  p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
  p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);

  if (currentPage == 0) {
    pinMode(XM, OUTPUT);  
    pinMode(YP, OUTPUT);

    if (homePage){
      Home();
      homePage = false;
    }

    buttonEnabled = true;
    if (buttonEnabled && 0 < p.x && p.x < tft.width() && (tft.height() / 2) < p.y && p.y < tft.height() && p.z > ts.pressureThreshhold) {
      buttonEnabled = false;
      currentPage = 1;
      pageOne = true;
    }
  }

  if (currentPage == 1) {    
    pinMode(XM, OUTPUT);  
    pinMode(YP, OUTPUT);

    if (pageOne) {
      PageOne();
      pageOne = false;
    }  
    homeButton = true; 

    if (homeButton && p.x > (tft.width() / 2) && p.x < tft.width() && p.y > 0 && p.y < (tft.height() / 2) && p.z > ts.pressureThreshhold) {
      currentPage = 0;
      cal += optOne;
      Home();
    }
    
    if (homeButton && 0 < p.x && p.x < (tft.width() / 2) && 0 < p.y && p.y < (tft.height() / 2) && p.z > ts.pressureThreshhold) {
      currentPage = 0;
      cal += optTwo;
      Home();
    }
  }
}