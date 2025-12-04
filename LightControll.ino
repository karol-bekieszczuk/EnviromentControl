#include <PCF8563.h>

PCF8563 pcf;

const int LIGHT_CTRL_PIN = 2;
const int FAN_PIN = 3;
bool isLightOn = false;

void setup() {
  pinMode(LIGHT_CTRL_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(LIGHT_CTRL_PIN, LOW);
  pcf.init();//initialize the clock
  // pcf.stopClock();//stop the clock

  // //set time to to 31/3/2018 17:33:0
  // pcf.setYear(18);//set year
  // pcf.setMonth(3);//set month
  // pcf.setDay(31);//set dat
  // pcf.setHour(17);//set hour
  // pcf.setMinut(33);//set minut
  // pcf.setSecond(0);//set second

  // pcf.startClock();//start the clock

  Serial.begin(9600);
}

void loop() {
  Time nowTime = pcf.getTime();
  lightControl(nowTime);
  //fanControl(nowTime);
}

void lightControl(Time nowTime)
{
  int currentTimeInMinutes = nowTime.hour * 60 + nowTime.minute;
  int lightStartTime = 630;
  int darkStartTime = 1290;
  bool shouldBeOn = (currentTimeInMinutes >= lightStartTime && currentTimeInMinutes < darkStartTime);

  if(shouldBeOn != isLightOn)
  {
    isLightOn = shouldBeOn;
    digitalWrite(LIGHT_CTRL_PIN, isLightOn ? HIGH : LOW);
    Serial.println(isLightOn ? "HIGH" : "LOW");
  }
}

void fanControl(Time nowTime){
  // analogWrite(FAN_PIN, 128); // ~50% obrotów
  // delay(10000);
  // analogWrite(FAN_PIN, 255); // 100% obrotów
  // delay(5000);
  analogWrite(FAN_PIN, 0);   // wentylator niskie obroty
  delay(5000);
}