#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>     // Touchscreen Library
#include <Colors.h>          // Farben Library

#define ILI9341_DISPLAYOFF         0x28
//Touch For New ILI9341 TP
#define TS_MINX 108
#define TS_MAXX 922
#define TS_MINY 115
#define TS_MAXY 920

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define MINPRESSURE 5
#define MAXPRESSURE 1000

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

bool Fensterautomatik = true;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void Startbildschirm() // Aussehen des Startbildschirms als Funktion gespeichert
{
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(120, 15);
  tft.println("Menue");
  tft.fillRect(10, 42, 300, 5, RED);
  tft.drawRoundRect(20, 60, 135, 40, 10, GREEN);
  tft.drawRoundRect(165, 60, 135, 40, 10, GREEN);
  tft.drawRoundRect(20, 110, 135, 40, 10, MAGENTA);
  tft.drawRoundRect(165, 110, 135, 40, 10, MAGENTA);
  tft.drawRoundRect(20, 160, 135, 40, 10, BLUE);
  tft.drawRoundRect(165, 160, 135, 40, 10, BLUE);
  tft.setCursor(0, 232);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Version 1.0 Smart-Home by Hig4tower");
  tft.setCursor(40, 77);
  tft.setTextSize(1);
  tft.println("FENSTERSTEUERUNG");
  tft.setCursor(180, 77);
  tft.println("HEIZUNGSSTEUERUNG");
  tft.setCursor(35, 127);
  tft.println("STEUERUNGSTEUERUNG");
  tft.setCursor(75, 177);
  tft.println("KLIMA");
}
void Fenstersteuerung() // Aussehen der Fenstersteuerung als Funktion gespeichert
{
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  tft.setCursor(70, 16);
  tft.setTextSize(2.5);
  tft.setTextColor(WHITE);
  tft.println("FENSTERSTEUERUNG");
  tft.fillRect(10, 36, 300, 5, GREEN);
  tft.fillCircle(260, 90, 40, RED);
  tft.fillCircle(260, 180, 40, RED);
  tft.fillTriangle(260, 50, 220, 90, 300, 90, WHITE);
  tft.fillTriangle(260, 220, 220, 180, 300, 180, WHITE);
  tft.fillRect(250, 90, 20, 36, WHITE);
  tft.fillRect(250, 145, 20, 36, WHITE);
  tft.fillRoundRect(20,50,180,40,5,LIGHTGREY);
  tft.drawRoundRect(-10,210,90,40,5,GREEN);
  tft.setTextSize(3.0);
  if (Fensterautomatik == true) {
    tft.setTextColor(WHITE);
    tft.fillRoundRect(20,50,180,40,5,RED);
    tft.setCursor(30,59);
    tft.println("AUTOMATIK");
    tft.setCursor(5,216);
    tft.println("BACK");
  }
  else if (Fensterautomatik == false) {
    tft.setTextColor(WHITE);
    tft.setCursor(30,59);
    tft.println("AUTOMATIK");
    tft.setCursor(5,216);
    tft.println("BACK");
  }
}

void Fensterteuerungslogik() // Logik der Fenstersteuerung als Funktion gespeichert
{  
  while (1)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
      p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());

      if (p.y < 90 && p.x > 210)                                                    // Für den Back-Button den Strom des Motors ausschalten
      {
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        break;
      }
      Serial.println(p.z);
      while (p.z > MINPRESSURE && p.z < MAXPRESSURE)
      {
        TSPoint p = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
          p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
          p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());
          if (p.y > 220 && p.y < 300 && p.x > 50 && p.x < 130)
          {
            digitalWrite(10, HIGH);
            digitalWrite(11, LOW);
          }
          else if (p.y > 220 && p.y < 300 && p.x > 140 && p.x < 220)
          {
            digitalWrite(10, LOW);
            digitalWrite(11, HIGH);
          }
          else if (p.y > 20 && p.y < 200 && p.x > 45 && p.x < 85 )
          {
            if (Fensterautomatik == true) {
              Fensterautomatik = false;
              tft.fillRoundRect(20,50,180,40,5,DARKGREY);
              tft.setTextColor(WHITE);
              tft.setCursor(30,59);
              tft.println("AUTOMATIK");  
              tft.setCursor(5,216);
              tft.println("BACK");
            }
            else if (Fensterautomatik == false) 
            {
              Fensterautomatik = true;
              tft.setTextColor(WHITE);
              tft.fillRoundRect(20,50,180,40,5,RED);
              tft.setCursor(30,59);
              tft.println("AUTOMATIK");
              tft.setCursor(5,216);
              tft.println("BACK");
            }
          }
        }
        else if (p.z == 0 && p.x == 0)
        {
          digitalWrite(10, LOW);
          digitalWrite(11, LOW);
          break;
        }
      }
    }
  }
  Startbildschirm();
}

void setup() {
  Serial.begin(9600);

  //int x1, y1, x2, y2;   //Variablen für den Grafiktest
  int w = tft.width();  //Variable für Bildschirmbreite 240
  int h = tft.height(); //Variable für Bildschirmhöhe 320
  
  tft.begin(0x9341);
  Startbildschirm();
}
void loop() {
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

    p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
    p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());

    if (p.y > 20 && p.y < 150) // Regeln für die linken drei Buttons
    {
      if (p.x > 60 && p.x < 100) {
        tft.fillRoundRect(20, 60, 135, 40, 10, GREEN);
        tft.setTextColor(BLACK);
        tft.setCursor(40, 77);
        tft.setTextSize(1);
        tft.println("FENSTERSTEUERUNG");
        Fenstersteuerung();
        Fensterteuerungslogik();
      }
      else if (p.x > 110 && p.x < 150)
      {
        tft.fillRoundRect(20, 110, 135, 40, 10, MAGENTA);
        tft.setTextColor(BLACK);
        tft.setCursor(35, 127);
        tft.setTextSize(1);
        tft.println("STEUERUNGSTEUERUNG");
      }
      else if (p.x > 160 && p.x < 200)
      {
        tft.fillRoundRect(20, 160, 135, 40, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(75, 177);
        tft.setTextSize(1);
        tft.println("KLIMA");
      }
    }
    if (p.y > 160 && p.y < 295) // Regeln für die rechten drei Buttons
    {
      if (p.x > 60 && p.x < 100) {
        tft.fillRoundRect(165, 60, 135, 40, 10, GREEN);
        tft.setTextColor(BLACK);
        tft.setCursor(180, 77);
        tft.setTextSize(1);
        tft.println("HEIZUNGSSTEUERUNG");
      }
      else if (p.x > 110 && p.x < 150)
      {
        tft.fillRoundRect(165, 110, 135, 40, 10, MAGENTA);
        tft.setTextColor(BLACK);
        tft.setCursor(180, 127);
        tft.setTextSize(1);
        tft.println("STEUERUNGSTEUERUNG");
      }
      else if (p.x > 160 && p.x < 200)
      {
        tft.fillRoundRect(165, 160, 135, 40, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(180, 177);
        tft.setTextSize(1);
        tft.println("KLIMA");
      }
    }
  }
}
