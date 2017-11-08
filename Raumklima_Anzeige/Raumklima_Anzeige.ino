int TMP = A ;               //Temperatursensor am Pin A
int TMPsens;                //Wert am Temperatursensor
int temperatur;             //Temperatur in °C
const int TMPmax = ;        //Maximal messbare Temperatur des Sensors
const int TMPmin = ;        //Minimal messbare Temperatur des Sensors

int BAR = A                 //Luftdrucksensor am Pin A
int BARsens;                //Wert am Drucksensor
int druck;                  //Druck in hPa
const int BARmax = ;        //Maximal messbarer Druck des Sensors
const int BARmin = ;        //Minimal messbarer Druck des Sensors

int HUM = A                 //Luftfeuchtigkeitssensor am Pin A
int HUMsens;                //Wert am Luftfeuchtigkeitssensor
int humidity;               //Luftfeuchtigkeit in %
const int HUMmax = ;        //Maximal messbare Luftfeuchtigkeit des Sensors
const int HUMmin = ;        //Minimal messbare Luftfeuchtigkeit des Sensors

int data[4];

bool enable = true;

int t = 500;                //Zeitlicher Abstand der Messungen in ms


void Temperatur()                                       //Variable temperatur erhält ihren Wert
{
  TMPsens = analogRead(TMP);                            //analoger Wert wird gespeichert
  temperatur = map(TMPsens, 0, 410, TMPmin, TMPmax);    //analoger Wert wird auf °C umgerechnet
  data[0] = temperatur;
}

void Druck()                                            //Variable druck erhält ihren Wert
{
  BARsens = analogRead(BAR);                            //analoger Wert wird gespeichert
  druck = map(BARsens, 0, 410, BARmin, BARmax);         //analoger Wert wird auf hPa umgerechnet
  data[1] = druck;
}

void Luftfeuchtigkeit()                                //Variable humidity erhält ihren Wert
{
  HUMsens = analogRead(HUM);                           //analoger Wert wird gespeichert
  humidity = map(HUMsens, 0, 410, HUMmin, HUMmax);     //analoger Wert wird in % umgerechnet
  data[2] = humidity;
}


void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop()
{
  enable = 
  if enable == true
  {
    Temperatur;
    Druck;
    Luftfeuchtigkeit;
    
    delay(t);
  }
  else
  {
    delay(t*5);
  }
}
