#include <Wire.h>
#include <SparkFunHTU21D.h>

HTU21D myHumidity;

void setup() {
Serial.begin(9600);
Serial.println("HTU21D TEST");
pinMode(A3,OUTPUT);

myHumidity.begin();
}

void loop() {
float humd = myHumidity.readHumidity();
  float temp = myHumidity.readTemperature();

  Serial.println("Time:");
  Serial.println(millis());
  Serial.println(" Temperature:");
  Serial.print(temp, 1);
  Serial.println(" C");
  Serial.println(" Humidity:");
  Serial.print(humd, 1);
  Serial.println(" %");

  if (humd > 65){
    digitalWrite(A3,HIGH);
  }
  else{
    digitalWrite(A3,LOW);
  }

  Serial.println();
  delay(5000);
}
