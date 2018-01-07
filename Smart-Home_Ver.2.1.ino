#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <Colors.h>
#include <SoftwareSerial.h>
#include <Wire.h>

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
#define MINPRESSURE 20
#define MAXPRESSURE 1000

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

SoftwareSerial BTSerial(10, 11); // RX | TX
bool RolladenAutomatik = true;
bool BT_Bind = false;
int Puffer;
int temperatur = 5;
int temperatur2;
String swap;

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
  tft.drawRoundRect(20, 60, 135, 40, 10, BLUE);
  tft.drawRoundRect(165, 60, 135, 40, 10, BLUE);
  tft.drawRoundRect(20, 110, 135, 40, 10, BLUE);
  tft.drawRoundRect(165, 110, 135, 40, 10, BLUE);
  //tft.drawRoundRect(20, 160, 135, 40, 10, BLUE);
  //tft.drawRoundRect(165, 160, 135, 40, 10, BLUE);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.setCursor(40, 77);
  tft.println("ROLLADENSTEUERUNG");
  tft.setCursor(180, 77);
  tft.println("HEIZUNGSSTEUERUNG");
  tft.setCursor(35, 127);
  tft.println("LUEFTUNGSSTEUERUNG");
  //tft.setCursor(75, 177);     BUTTON UNTEN LINKS
  //tft.println("");            BUTTON BEZEICHNUNG
  tft.setCursor(205, 127);
  tft.println("RAUMKLIMA");
  tft.setCursor(0, 232);
  tft.println("Version 2.1 Smart-Home by TMT15SI");
}
void bufferClear()
{
  while (BTSerial.available() > 0)
  {
    Serial.println(BTSerial.read());
    int Puffer = BTSerial.read();
  }
}
void RolladenSteuerung()
{
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  tft.setCursor(70, 16);
  tft.setTextSize(2.5);
  tft.setTextColor(WHITE);
  tft.println("ROLLADENSTEUERUNG");
  tft.fillRect(10, 36, 300, 5, GREEN);
  tft.fillCircle(260, 90, 40, RED);
  tft.fillCircle(260, 180, 40, RED);
  tft.fillTriangle(260, 50, 220, 90, 300, 90, WHITE);
  tft.fillTriangle(260, 220, 220, 180, 300, 180, WHITE);
  tft.fillRect(250, 90, 20, 36, WHITE);
  tft.fillRect(250, 145, 20, 36, WHITE);
  tft.drawRoundRect(-10, 210, 90, 40, 5, GREEN);
  tft.setTextSize(3.0);
  tft.setCursor(5, 215);
  tft.println("BACK");
  if (RolladenAutomatik == true) {
    tft.setTextColor(WHITE);
    tft.fillRoundRect(20, 50, 180, 40, 5, RED);
    tft.setCursor(30, 59);
    tft.println("AUTOMATIK");
    tft.setCursor(5, 216);
    tft.println("BACK");
  }
  else if (RolladenAutomatik == false) {
    tft.setTextColor(WHITE);
    tft.fillRoundRect(20, 50, 180, 40, 5, DARKGREY);
    tft.setCursor(30, 59);
    tft.println("AUTOMATIK");
    tft.setCursor(5, 216);
    tft.println("BACK");
  }

  while (true)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
      p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
      p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());

      if (p.y < 90 && p.x > 210)
      {
        BTSerial.println("c");
        break;
      }
      else if (p.y > 20 && p.y < 200 && p.x > 45 && p.x < 85 )
      {
        if (RolladenAutomatik == true)
        {
          RolladenAutomatik = false;
          tft.fillRoundRect(20, 50, 180, 40, 5, DARKGREY);
          tft.setTextColor(WHITE);
          tft.setCursor(30, 59);
          tft.println("AUTOMATIK");
          tft.setCursor(5, 216);
          tft.println("BACK");
        }
        else if (RolladenAutomatik == false)
        {
          RolladenAutomatik = true;
          tft.setTextColor(WHITE);
          tft.fillRoundRect(20, 50, 180, 40, 5, RED);
          tft.setCursor(30, 59);
          tft.println("AUTOMATIK");
          tft.setCursor(5, 216);
          tft.println("BACK");
        }
      }
      while (true)
      {
        TSPoint p = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        if (p.z > MINPRESSURE && p.z < MAXPRESSURE || p.x > 0)
        {
          p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
          p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());
          if (p.y > 220 && p.y < 300 && p.x > 50 && p.x < 130)
          {
            BTSerial.println("a");
          }
          else if (p.y > 220 && p.y < 300 && p.x > 140 && p.x < 220)
          {
            BTSerial.println("b");
          }
        }
        else
        {
          BTSerial.println("c");
          break;
        }
      }
    }
    else
    {
      BTSerial.println("c");
    }
  }
  delay(200);
  Startbildschirm();
}
void Lueftungssteuerung()
{
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  tft.setCursor(55, 16);
  tft.setTextSize(2.5);
  tft.setTextColor(WHITE);
  tft.println("LUEFTUNGSSTEUERUNG");
  tft.fillRect(10, 36, 300, 5, GREEN);
  tft.drawRoundRect(-10, 210, 90, 40, 5, GREEN);
  tft.setTextSize(3.0);
  tft.setCursor(5, 215);
  tft.println("BACK");

  while (1)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
      p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
      p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());

      if (p.y < 90 && p.x > 210)
      {
        break;
      }
    }
  }
  delay(200);
  Startbildschirm();
}
void Heizungssteuerung()
{
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  tft.setCursor(60, 16);
  tft.setTextSize(2.5);
  tft.setTextColor(WHITE);
  tft.println("HEIZUNGSSTEUERUNG");
  tft.fillRect(10, 36, 300, 5, GREEN);
  tft.drawRoundRect(-10, 210, 90, 40, 5, GREEN);
  tft.fillCircle(260, 90, 40, RED);
  tft.fillCircle(260, 180, 40, RED);
  tft.fillTriangle(260, 50, 220, 90, 300, 90, WHITE);
  tft.fillTriangle(260, 220, 220, 180, 300, 180, WHITE);
  tft.fillRect(250, 90, 20, 36, WHITE);
  tft.fillRect(250, 145, 20, 36, WHITE);
  tft.fillRoundRect(40, 150, 140, 50, 5, RED);
  tft.drawCircle(145, 100, 3, WHITE);
  tft.drawCircle(145, 100, 4, WHITE);
  tft.setTextSize(3.0);
  tft.setCursor(152, 100);
  tft.println("C");
  tft.setCursor(5, 215);
  tft.println("BACK");
  tft.setCursor(100, 100);
  tft.println(temperatur);
  tft.setTextColor(BLACK);
  tft.setCursor(68, 164);
  tft.println("ENTER");
  tft.setTextColor(WHITE);

  while (true)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
      p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
      p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());

      if (p.y < 90 && p.x > 210)
      {
        break;
      }
      else if (p.y > 220 && p.y < 300 && p.x > 50 && p.x < 130)
      {
        if (temperatur < 30)
        {
          temperatur += 1;
          tft.fillRect(100, 100, 40, 22, BLACK);
          tft.setCursor(100, 100);
          tft.println(temperatur);
          delay(100);
        }
      }
      else if (p.y > 220 && p.y < 300 && p.x > 140 && p.x < 220)
      {
        if (temperatur > 5)
        {
          temperatur -= 1;
          tft.fillRect(100, 100, 40, 22, BLACK);
          tft.setCursor(100, 100);
          tft.println(temperatur);
          delay(100);
        }
      }
      else if (p.y > 40 && p.y < 180 && p.x > 150 && p.x < 200)
      {
        //swap = temperatur + 60;
        temperatur2 = temperatur + 60;
        //swap.toCharArray(temperatur2, 2);
        BTSerial.write(temperatur2);
        delay(500);
      }
    }
  }
  delay(200);
  Startbildschirm();
}
void Raumklima()
{
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  tft.setCursor(120, 16);
  tft.setTextSize(2.5);
  tft.setTextColor(WHITE);
  tft.println("RAUMKLIMA");
  tft.fillRect(10, 36, 300, 5, GREEN);
  tft.drawRoundRect(-10, 210, 90, 40, 5, GREEN);
  tft.setCursor(32, 60);
  tft.println("TEMPERATUR: ");
  tft.setCursor(205, 60);
  tft.drawCircle(200, 60, 2, WHITE);
  tft.drawCircle(200, 60, 3, WHITE);
  tft.println("C");
  tft.setCursor(20, 90);
  tft.println("LUFTFEUCHTE: ");
  tft.setCursor(200, 90);
  tft.println("%");
  tft.setTextSize(3.0);
  tft.setCursor(5, 215);
  tft.println("BACK");
  BTSerial.println("d");
  tft.setTextSize(2.5);

  while (true)
  {
    BTSerial.println("d");
    String Klima = BTSerial.readString();
    tft.fillRect(170, 60, 24, 16, BLACK);
    tft.fillRect(170, 90, 24, 16, BLACK);
    tft.fillRect(0, 106, 24, 16, BLACK);
    tft.setCursor(170, 60);
    tft.println(Klima.substring(0, 2));        // Zeichen 1 bis 2 ausgeben
    tft.setCursor(170, 90);                    // Zeichen 3 ist eine Leerzeile
    tft.println(Klima.substring(3, 5));        // Zeichen 4 und 5 ausgeben
    if (ts.isTouching())
    {
      TSPoint p = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
      p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());
      if (p.y < 90 && p.x > 210)
      {
        break;
      }
    }
  }
  Startbildschirm();
}
void BT_Connection()
{
  tft.fillScreen(BLACK);
  tft.setTextSize(2.5);
  tft.setTextColor(WHITE);
  tft.setCursor(50, 100);
  tft.println("Verbindung wird");
  tft.setCursor(50, 150);
  tft.println("hergestellt..!");
  BTSerial.println("AT+ROLE=1");
  delay(50);
  BTSerial.println("AT+PSWD=1234");
  delay(50);
  BTSerial.println("AT+RMAAD");
  //Serial.println("GELÖSCHT");
  delay(500);
  BTSerial.println("AT+INIT");
  //Serial.println("INIT MODE AN");
  delay(500);
  BTSerial.println("AT+INQ");
  //Serial.println("SSP LIBRARY");
  delay(800);
  BTSerial.println("AT+BIND=98d3,31,fb1763");
  //Serial.println("BINDED!");
  delay(800);
  BTSerial.println("AT+PAIR=98D3,31,fb1763,2");
  //Serial.println("PAIRED!");
  delay(800);
  BTSerial.println("AT+LINK=98d3,31,fb1763");
  //Serial.println("LINKED!");
  delay(800);
  tft.fillScreen(BLACK);
  tft.setCursor(50, 100);
  tft.println("Verbunden!");
  BT_Bind = true;
  delay(300);
}
void setup()
{
  Serial.begin(9600);
  BTSerial.begin(38400);

  //pinMode(12,OUTPUT);
  //digitalWrite(12,HIGH);
  int x1, y1, x2, y2;   //Variablen für den Grafiktest
  int w = tft.width();  //Variable für Bildschirmbreite 240
  int h = tft.height(); //Variable für Bildschirmhöhe 320
  tft.begin(0x9341);
  Startbildschirm();
}
void loop()
{
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

    p.x = tft.height() - map(p.x, TS_MINY, TS_MAXY, tft.height(), 0 );
    p.y = map(p.y, TS_MINX, TS_MAXX, 10, tft.width());

    if (p.y > 20 && p.y < 150) // Regeln für die linken drei Buttons
    {
      if (p.x > 60 && p.x < 100) {
        tft.fillRoundRect(20, 60, 135, 40, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(40, 77);
        tft.setTextSize(1);
        tft.println("ROLLADENSTEUERUNG");
        bufferClear();
        if (BT_Bind == true)
        {
          RolladenSteuerung();
        }
        else
        {
          BT_Connection();
          RolladenSteuerung();
        }
      }
      else if (p.x > 110 && p.x < 150)
      {
        tft.fillRoundRect(20, 110, 135, 40, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(35, 127);
        tft.setTextSize(1);
        tft.println("LUEFTUNGSSTEUERUNG");
        Lueftungssteuerung();
      }
      else if (p.x > 160 && p.x < 200)
      {
        /*tft.fillRoundRect(20, 160, 135, 40, 10, BLUE);
          tft.setTextColor(BLACK);
          tft.setCursor(75, 177);
          tft.setTextSize(1);
          tft.println("KLIMA");*/
      }
    }
    if (p.y > 160 && p.y < 295) // Regeln für die rechten drei Buttons
    {
      if (p.x > 60 && p.x < 100) {
        tft.fillRoundRect(165, 60, 135, 40, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(180, 77);
        tft.setTextSize(1);
        tft.println("HEIZUNGSSTEUERUNG");
        bufferClear();
        if (BT_Bind == true)
        {
          Heizungssteuerung();
        }
        else
        {
          BT_Connection();
          Heizungssteuerung();
        }
      }
      else if (p.x > 110 && p.x < 150)
      {
        tft.fillRoundRect(165, 110, 135, 40, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(205, 127);
        tft.setTextSize(1);
        tft.println("RAUMKLIMA");
        bufferClear();
        if (BT_Bind == true)
        {
          Raumklima();
        }
        else
        {
          BT_Connection();
          Raumklima();
        }

      }
      else if (p.x > 160 && p.x < 200)
      {
        /*tft.fillRoundRect(165, 160, 135, 40, 10, BLUE);
          tft.setTextColor(BLACK);
          tft.setCursor(180, 177);
          tft.setTextSize(1);
          tft.println("KLIMA");*/
      }
    }
  }
}
