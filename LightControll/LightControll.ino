#include <PCF8563.h>

PCF8563 pcf;

const int LIGHT_CTRL_PIN = 2;
const int FAN_POWER_PIN = 5;
const int FAN_PWM_PIN = 3;
bool isLightOn = false;

void setup() {
  pinMode(LIGHT_CTRL_PIN, OUTPUT);
  pinMode(FAN_POWER_PIN, OUTPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);
  digitalWrite(FAN_POWER_PIN, LOW);
  digitalWrite(LIGHT_CTRL_PIN, LOW);
  pcf.init();//initialize the clock
  
  // set time to to 31/3/2018 17:33:0
  // pcf.stopClock();//stop the clock

  // pcf.setYear(25);//set year
  // pcf.setMonth(12);//set month
  // pcf.setDay(14);//set day
  // pcf.setHour(20);//set hour
  // pcf.setMinut(5);//set minut
  // pcf.setSecond(0);//set second

  // pcf.startClock();//start the clock
  //

  // Serial.begin(9600);
}

void loop() {
  Time nowTime = pcf.getTime();
  lightControl(nowTime);
  fanControl(nowTime);
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
    int hour = nowTime.hour;
    int minute = nowTime.minute;

    if (hour >= 10 && hour < 22) { // tylko w przedziale 10-22
        int minutesSinceStart = (hour - 10) * 60 + minute; // minuty od 10:00
        if (minutesSinceStart % 21 < 1) { // 1 minuta ON
            digitalWrite(FAN_POWER_PIN, HIGH);
            analogWrite(FAN_PWM_PIN, 254);
        } else {
            digitalWrite(FAN_POWER_PIN, LOW);
            analogWrite(FAN_PWM_PIN, 0);
        }
    } else {
        digitalWrite(FAN_POWER_PIN, LOW);
        analogWrite(FAN_PWM_PIN, 0);
    }
}

void printTime(Time nowTime){
  Serial.print(nowTime.hour);
  Serial.print(":");
  Serial.print(nowTime.minute);
  Serial.print(":");
  Serial.println(nowTime.second);
}