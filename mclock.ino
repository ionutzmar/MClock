/*
  Author: ionutzmar@gmail.com
  This is a clock built for a high school certificate in Computer Science.
*/
#include <LiquidCrystal.h>
#include <Time.h>

const int minutePin = 13;
const int hourPin = 10;
const int setPin = 7;
const int alarmPin = 8;
const int piezoPin = 9;
const int tempPin = A2;
const int alarmSoundFrequency = 2000;  //Hz
const int alarmDuration = 300000; //in milliseconds

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int shour = 0; //used to set the clock
int sminute = 0; //used to set the clock
int hours, minutes, seconds; //actual time
int temperature;
bool alarm = false;
int alarmMinute = 0;
int alarmHour = 0;
int alarmState, prevAlarmState = 0, setState,  hourState, prevHourState = 0, minuteState, prevMinuteState = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(minutePin, INPUT);
  pinMode(hourPin, INPUT);
  pinMode(setPin, INPUT);
  pinMode(alarmPin, INPUT);
  pinMode(piezoPin, OUTPUT);
}

void loop() {
  
  alarmState = digitalRead(alarmPin);
  setState = digitalRead(setPin);
  hourState = digitalRead(hourPin);
  minuteState = digitalRead(minutePin);
  
  if(hourState != prevHourState && hourState == HIGH && setState == HIGH)
    shour++;
  if(minuteState != prevMinuteState && minuteState == HIGH && setState == HIGH)
    sminute++;
  if(alarmState != prevAlarmState && alarmState == HIGH && setState == HIGH)
    alarm = !alarm;
  if(hourState != prevHourState && hourState == HIGH && alarmState == HIGH && setState == LOW)
    alarmHour++;
  if(minuteState != prevMinuteState && minuteState == HIGH && alarmState == HIGH && setState == LOW)
    alarmMinute++;
  if(alarmState == HIGH)
    noTone(piezoPin);
  
  hours = (shour + hour() + (minute() + sminute) / 60) % 24;
  minutes = (sminute + minute()) % 60;
  seconds = second();
  alarmHour = alarmHour % 24;
  alarmMinute = alarmMinute % 60;
  
  lcd.setCursor(4, 0);
  if(alarmState == LOW)
  {
    (hours > 9) ? (lcd.print(hours)) : (lcd.print(0), lcd.print(hours));
    lcd.print(":");
    (minutes > 9) ? (lcd.print(minutes)) : (lcd.print(0), lcd.print(minutes));
    lcd.print(":");
    (seconds > 9) ? (lcd.print(seconds)) : (lcd.print(0), lcd.print(seconds));
  }
  else
  {
    (alarmHour > 9) ? (lcd.print(alarmHour)) : (lcd.print(0), lcd.print(alarmHour));
    lcd.print(":");
    (alarmMinute > 9) ? (lcd.print(alarmMinute)) : (lcd.print(0), lcd.print(alarmMinute));
    lcd.print(":00");
  }
  temperature = (analogRead(tempPin) / 1024.0 * 5.0 - .5) * 100;
  
  lcd.setCursor(0, 1);
  lcd.print("Temp ");
  lcd.print(temperature);
  lcd.print(" Alarm");
  (alarm) ? (lcd.print(" ON")) : (lcd.print("OFF"));
  
  if(alarmHour == hours && alarmMinute == minutes && alarm && seconds < 1)
    tone(piezoPin, alarmSoundFrequency, alarmDuration);
  
  prevAlarmState = alarmState;
  prevHourState = hourState;
  prevMinuteState = minuteState;
}


