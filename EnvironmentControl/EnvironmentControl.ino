#include <PCF8563.h>

PCF8563 pcf;

constexpr int LIGHT_CTRL_PIN = 2;
constexpr int FAN_POWER_PIN = 5;
constexpr int FAN_PWM_PIN = 9;
constexpr int CYCLE_START_MIN = 570; //09:30
constexpr int CYCLE_STOP_MIN = 1290; //21:30
const uint16_t TOP = 640; // 16 MHz / 25 kHz TOP - maximum counter value

void setup() {
  //************** manualy set timers **************//
  TCCR1A = (1 << COM1A1) | (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);

  ICR1 = TOP;
  //************** end manualy set timers **************//
  //************** set up pins mode **************//
  pinMode(LIGHT_CTRL_PIN, OUTPUT);
  pinMode(FAN_POWER_PIN, OUTPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);
  //************** end set up pins mode **************//
  //************** set up pins default state **************//
  digitalWrite(LIGHT_CTRL_PIN, LOW);
  digitalWrite(FAN_POWER_PIN, LOW);
  //************** end set up pins default state **************//
  //************** initialize the clock **************//
  pcf.init(); //THIS MUST BE UNCOMMENTED FOR THE RTC TO WORK!!
  
  // set RTC time
  // pcf.stopClock();//stop the clock

  // pcf.setYear(25);//set year
  // pcf.setMonth(12);//set month
  // pcf.setDay(15);//set day
  // pcf.setHour(16);//set hour
  // pcf.setMinut(28);//set minut
  // pcf.setSecond(0);//set second

  // pcf.startClock();//start the clock
  //************** end initialize the clock **************//

  // Serial.begin(9600);
}

void loop() {
  Time nowTime = pcf.getTime();
  int currentTimeInMinutes = nowTime.hour * 60 + nowTime.minute;

  lightControl(currentTimeInMinutes);
  fanControl(currentTimeInMinutes);
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

void fanControl(int currentTimeInMinutes){
  static bool isFanOn = false;
  bool shouldBeOn = (currentTimeInMinutes >= CYCLE_START_MIN && 
                      currentTimeInMinutes < CYCLE_STOP_MIN &&
                      currentTimeInMinutes % 21 == 0);
  if(shouldBeOn != isFanOn)
  {
    isFanOn = shouldBeOn;
    digitalWrite(FAN_POWER_PIN, isFanOn ? HIGH : LOW);
    uint8_t percent = constrain(50, 20, 100); // nie schodź niżej
    OCR1A = map(percent, 0, 100, 0, TOP);
  }
}

void printTime(Time nowTime){
  Serial.print(nowTime.hour);
  Serial.print(":");
  Serial.print(nowTime.minute);
  Serial.print(":");
  Serial.println(nowTime.second);
}