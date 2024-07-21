#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address (0x27) if needed

struct {
  int hour = 0;
  int minute = 0;
  int second = 0;

  String getString () {
    String time = "";
    if (this->hour < 10) {
      time += "0";
    }
    time += String(this->hour);
    time += ":";
    if (this->minute < 10) {
      time += "0";
    }
    time += String(this->minute);
    // time += ":";
    // if (this->second < 10) {
    //   time += "0";
    // }
    // time += String(this->second);
    return time;
  }
} Time1, Time2;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd.print("RTC lost power!");
    rtc.(DateTime(F(__DATE__), F(__TIME__)));
  }

}

void loop() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print(" ");
  lcd.print(" ");
  lcd.print(" ");
  lcd.print(" ");
  if (now.hour() < 10) {
    lcd.print("0");
  }
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10) {
    lcd.print("0");
  }
  lcd.print(now.minute());

  lcd.print(':');
  if (now.second() < 10) {
    lcd.print("0");
  }
  lcd.print(now.second());
  lcd.print("       ");

  Time1.hour = 7;
  Time1.minute = 30;
  Time1.second = 45;
  
  Time2.hour = 18;
  Time2.minute = 15;
  Time2.second = 00;
  lcd.setCursor(0, 1);
  // Serial.print(Time1.getString());
  // Serial.print("\t");
  // Serial.print(Time2.getString());
  lcd.print(" ");
  lcd.print(" ");
  lcd.print(Time1.getString());
  lcd.print(" ");
  lcd.print(" ");
  lcd.print(Time2.getString());

  delay(1000);
}