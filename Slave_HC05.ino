// SLAVE-HC05 muss immer schnell blinken, d.h. kein AT-Mode sondern im Pair-Modus sein! //
// PSWD muss gleich sein, ROLE muss 0 sein (Slave),
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX
#define timer 40
//PINS für HEIZKÖRPERTHERMOSTAT: "MASSE" D9,links neben "MASSE" D3, ganz links A3.
bool state = false;
int hiLo;
void setup()
{
  //pinMode(X,INPUT);           //Für Temperature
  //pinMode(Y,INPUT);           //Für Humidity
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  hiLo = analogRead(3);
  int loHi = analogRead(9);
  delay(50);
  if (hiLo >= 800)
  {
    state = true;
    analogWrite(3, 153);
    analogWrite(A3, 153);
  }
  else
  {
    analogWrite(3, LOW);
    analogWrite(9, LOW);
    analogWrite(A3, LOW);
  }
  BTSerial.begin(38400);
  Serial.begin(9600);
  BTSerial.println("AT+ROLE=0");
  delay(100);
  BTSerial.println("AT+PSWD=1234");
  delay(100);
  BTSerial.print("AT+RMAAD");
  delay(100);//Auf 0,5 von 1 gesetzt
  int Puffer = Serial.read();
  Serial.println(hiLo);
  Serial.println(loHi);
  while (BTSerial.available() > 0)
  {
    int Puffer = BTSerial.read();
  }
}
void loop()
{
  if (BTSerial.available()) {
    char data = BTSerial.read();
    Serial.println(data);
    switch (data) {
      case 'a': UP(); break; //Rolladen hoch
      case 'b': DOWN(); break; // Rolladen runter
      case 'c': STOP(); break; // Rolladen stop
      case 'd': KLIMA(); break; // Klimadaten senden
      case 'A' : HEIZUNG(5); break;
      case 'B' : HEIZUNG(6); break;
      case 'C' : HEIZUNG(7); break;
      case 'D' : HEIZUNG(8); break;
      case 'E' : HEIZUNG(9); break;
      case 'F' : HEIZUNG(10); break;
      case 'G' : HEIZUNG(11); break;
      case 'H' : HEIZUNG(12); break;
      case 'I' : HEIZUNG(13); break;
      case 'J' : HEIZUNG(14); break;
      case 'K' : HEIZUNG(15); break;
      case 'L' : HEIZUNG(16); break;
      case 'M' : HEIZUNG(17); break;
      case 'N' : HEIZUNG(18); break;
      case 'O' : HEIZUNG(19); break;
      case 'P' : HEIZUNG(20); break;
      case 'Q' : HEIZUNG(21); break;
      case 'R' : HEIZUNG(22); break;
      case 'S' : HEIZUNG(23); break;
      case 'T' : HEIZUNG(24); break;
      case 'U' : HEIZUNG(25); break;
      case 'V' : HEIZUNG(26); break;
      case 'W' : HEIZUNG(27); break;
      case 'X' : HEIZUNG(28); break;
      case 'Y' : HEIZUNG(29); break;
      case 'Z' : HEIZUNG(30); break;
      default : break;
        /*    //Keep reading from HC-05 and send to Arduino Serial Monitor
          if (BTSerial.available())
          Serial.write(BTSerial.read());
           //Keep reading from Arduino Serial Monitor and send to HC-05
          if (Serial.available())
          BTSerial.write(Serial.read());*/
    }
  }
}
void UP() {
  Serial.println("Links!");
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
}
void DOWN() {
  Serial.println("Rechts!");
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
}
void STOP() {
  Serial.println("Stop!");
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}
void KLIMA() {
  BTSerial.println("20 55"/*&&digitalRead(X) && digitalRead(Y)*/); // Temperautur & Humidity
  while (BTSerial.available() > 0)
  {
    int Puffer = BTSerial.read();
  }
}
void HEIZUNG(int temperatur)
{
  for (int i = 1; i <= 50; i++) //Egal wie hoch die Temperatur ist, erstmal auf 5°C setzen und dann
  {

    analogWrite(3, LOW);
    delay(timer);
    digitalWrite(A3, LOW);
    delay(timer);
    Serial.println("LOW");

    analogWrite(3, 153);
    delay(timer);
    analogWrite(A3, 153);
    delay(timer);
    Serial.println("HIGH");
  }
  Serial.println("Auf 5 gesetzt");
  analogWrite(3, 153);
  for (int j = 1; j <= (temperatur - 5) * 2; j++)
  {
    Serial.println(j);

    analogWrite(9, LOW);
    delay(timer);
    analogWrite(3, LOW);
    delay(timer);
    Serial.println("LOW2");

    analogWrite(9, 153);
    delay(timer);
    analogWrite(3, 153);
    delay(timer);
    Serial.println("HIGH2");
  }
  analogWrite(3, 153);
  analogWrite(9, 153);
  analogWrite(A3, 153);
  Serial.print("Auf ");
  Serial.print(temperatur);
  Serial.print(" °C gesetzt");
  while (BTSerial.available() > 0)
  {
    int Puffer = BTSerial.read();
  }
}
