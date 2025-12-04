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
  // set time to to 31/3/2018 17:33:0
  // pcf.setYear(25);//set year
  // pcf.setMonth(11);//set month
  // pcf.setDay(4);//set day
  // pcf.setHour(22);//set hour
  // pcf.setMinut(23);//set minut
  // pcf.setSecond(0);//set second

  // pcf.startClock();//start the clock

  // Serial.begin(9600);
}

void loop() {
  Time nowTime = pcf.getTime();
  lightControl(nowTime);
  // fanControl(nowTime);
  // printTime(nowTime);
}

void lightControl(Time nowTime)
{
  int currentTimeInMinutes = nowTime.hour * 60 + nowTime.minute;
  int lightStartTime = 570; //09:30
  int darkStartTime = 1290; //21:30
  bool shouldBeOn = (currentTimeInMinutes >= lightStartTime && currentTimeInMinutes < darkStartTime);
  if(shouldBeOn != isLightOn)
  {
    isLightOn = shouldBeOn;
    digitalWrite(LIGHT_CTRL_PIN, isLightOn ? HIGH : LOW);
    // Serial.println(isLightOn ? "HIGH" : "LOW");
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

void printTime(Time nowTime){
  Serial.print(nowTime.hour);
  Serial.print(":");
  Serial.print(nowTime.minute);
  Serial.print(":");
  Serial.println(nowTime.second);
}