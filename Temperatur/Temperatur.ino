#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int sensorPin = A0;
int werte[21];
int i = 0;

void setup() {
  lcd.begin(16,2);
  lcd.print("Initializing");
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  float volt = (sensorVal/1024.0)*5.0;
  int temp = map(sensorVal, 0, 1024, -50, 450);
  //int humid = ;
  //float temp = (volt-0.5)*100;
  /*
  werte[i] = temp;
  i = i+1;
  if (i >= 21) {
    i = 0;
  }

  int j;
  int summe = 0;
  for (j=0; j<20; j++) {
    summe = summe+werte[j];  
  }
  int avg = summe/20;
  */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(temp);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print(humid);
  lcd.print(" %");
  delay(1000);
}
