#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address (0x27) if needed

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
    // Uncomment and set the date & time below to set it to the current date & time
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Or set to a specific date & time:
    // rtc.adjust(DateTime(2024, 7, 16, 12, 0, 0));
  }
}

void loop() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());

  delay(1000);
}
