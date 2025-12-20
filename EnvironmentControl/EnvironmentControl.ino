#include <PCF8563.h>
#include <FanController.h>

PCF8563 pcf;

constexpr int LIGHT_CTRL_PIN = 2;
constexpr int FAN_POWER_PIN = 5;
constexpr int FAN_PWM_PIN = 9;
constexpr int FAN_SENSOR_PIN = 4;
constexpr int CYCLE_START_MIN = 570; //09:30
constexpr int CYCLE_STOP_MIN = 1290; //21:30
// Choose a threshold in milliseconds between readings.
// A smaller value will give more updated results,
// while a higher value will give more accurate and smooth readings
constexpr int FAN_SENSOR_THRESHOLD = 1000;

FanController fan(FAN_SENSOR_PIN, FAN_SENSOR_THRESHOLD, FAN_PWM_PIN);

void setup() {
  //************** set up pins mode **************//
  pinMode(LIGHT_CTRL_PIN, OUTPUT);
  pinMode(FAN_POWER_PIN, OUTPUT);
  // pinMode(FAN_PWM_PIN, OUTPUT);
  //************** end set up pins mode **************//
  //************** set up pins default state **************//
  digitalWrite(LIGHT_CTRL_PIN, LOW);
  digitalWrite(FAN_POWER_PIN, LOW);
  // analogWrite(FAN_PWM_PIN, 0);
  //************** end set up pins default state **************//
  //************** initialize the clock **************//
  pcf.init(); //THIS MUST BE UNCOMMENTED FOR THE RTC TO WORK!!
  
  // set RTC time
  pcf.stopClock();//stop the clock

  pcf.setYear(25);//set year
  pcf.setMonth(12);//set month
  pcf.setDay(15);//set day
  pcf.setHour(16);//set hour
  pcf.setMinut(27);//set minut
  pcf.setSecond(57);//set second

  pcf.startClock();//start the clock
  //************** end initialize the clock **************//

  fan.begin();

  Serial.begin(9600);
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
    Serial.print("Current speed: ");
    unsigned int rpms = fan.getSpeed(); // Send the command to get RPM
    Serial.print(rpms);
    Serial.println("RPM");

  // Get new speed from Serial (0-100%)
  // if (Serial.available() > 0) {
    // Parse speed
    // int input = Serial.parseInt();

    // Constrain a 0-100 range
    byte target = max(min(isFanOn ? 100 : 0, 100), 0);

    // Print obtained value
    Serial.print("Setting duty cycle: ");
    Serial.println(target, DEC);

    // Set fan duty cycle
    fan.setDutyCycle(target);

    // Get duty cycle
    byte dutyCycle = fan.getDutyCycle();
    Serial.print("Duty cycle: ");
    Serial.println(dutyCycle, DEC);
    Serial.println(isFanOn);
    digitalWrite(FAN_POWER_PIN, isFanOn ? HIGH : LOW);
    // analogWrite(FAN_PWM_PIN, isFanOn ? 254 : 0);
  }

    // Not really needed, just avoiding spamming the monitor,
  // readings will be performed no faster than once every THRESHOLD ms anyway
  delay(250);
}

void printTime(Time nowTime){
  Serial.print(nowTime.hour);
  Serial.print(":");
  Serial.print(nowTime.minute);
  Serial.print(":");
  Serial.println(nowTime.second);
}