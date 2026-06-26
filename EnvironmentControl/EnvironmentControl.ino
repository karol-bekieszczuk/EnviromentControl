#include <PCF8563.h>

constexpr int LIGHT_CTRL_PIN = 2;
constexpr int CYCLE_START_MIN = 600; //10:00
constexpr int CYCLE_STOP_MIN = 1320; //22:00

PCF8563 pcf;

void setup() 
{
  //************** set up pins mode **************//
  pinMode(LIGHT_CTRL_PIN, OUTPUT);
  //************** end set up pins mode **************//
  //************** set up pins default state **************//
  digitalWrite(LIGHT_CTRL_PIN, LOW);
  //************** end set up pins default state **************//
  //************** initialize the clock **************//
  pcf.init(); //THIS MUST BE UNCOMMENTED FOR THE RTC TO WORK!!
  
  // // set RTC time
  // pcf.stopClock();//stop the clock

  // pcf.setYear(26);//set year
  // pcf.setMonth(6);//set month
  // pcf.setDay(26);//set day
  // pcf.setHour(18);//set hour
  // pcf.setMinut(14);//set minut
  // pcf.setSecond(58);//set second
  
  // pcf.startClock();//start the clock
  // //************** end initialize the clock **************//
  // //************** initialize the fan **************//
  // fan.begin();
  //************** end initialize the fan **************//

  //  Serial.begin(9600);
}

void loop() 
{
  Time nowTime = pcf.getTime();
  int currentTimeInMinutes = nowTime.hour * 60 + nowTime.minute;

  lightControl(currentTimeInMinutes);
  // printTime(nowTime);
}

void lightControl(int currentTimeInMinutes)
{
  static bool isLightOn = false;
  bool shouldBeOn = (currentTimeInMinutes >= CYCLE_START_MIN && currentTimeInMinutes < CYCLE_STOP_MIN);
  if(shouldBeOn != isLightOn)
  {
    isLightOn = shouldBeOn;
    digitalWrite(LIGHT_CTRL_PIN, isLightOn ? HIGH : LOW);
  }
}

void printTime(Time nowTime){
  Serial.print(nowTime.hour);
  Serial.print(":");
  Serial.print(nowTime.minute);
  Serial.print(":");
  Serial.println(nowTime.second);
}