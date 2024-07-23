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

const int buttonPin8 = 8;
const int buttonPin7 = 7;
const int buttonPin6 = 6;

int lastButtonState8 = HIGH;
int lastButtonState7 = HIGH;
int lastButtonState6 = HIGH;

unsigned long lastDebounceTime8 = 0;
unsigned long lastDebounceTime7 = 0;
unsigned long lastDebounceTime6 = 0;

const unsigned long debounceDelay = 200;

bool isEditingTime1 = false;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin8, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);

  Wire.begin();
  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd.print("RTC lost power!");
  }

  Time1.hour = 7;
  Time1.minute = 0;
  Time1.second = 0;

  Time2.hour = 18;
  Time2.minute = 0;
  Time2.second = 0;
}

void loop() {
  // LCD
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print("    ");
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
  if (isEditingTime1) {
    lcd.print(" - 1");
  } else {
    lcd.print(" - 2");
  }

  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(Time1.getString());
  lcd.print("   ");
  lcd.print(Time2.getString());

  // Push buttons
  int reading8 = digitalRead(buttonPin8);
  int reading7 = digitalRead(buttonPin7);
  int reading6 = digitalRead(buttonPin6);

  // Check for button press on pin D8
  if (reading8 != lastButtonState8) {
    lastDebounceTime8 = millis();
  }
  if (reading8 == LOW) {
    if ((millis() - lastDebounceTime8) > debounceDelay) {
      Serial.println("Button on pin D8 pressed");
      isEditingTime1 = !isEditingTime1;
      lastDebounceTime8 = millis(); // Reset debounce timer
    }
  }
  lastButtonState8 = reading8;

  // Check for button press on pin D7
  if (reading7 != lastButtonState7) {
    lastDebounceTime7 = millis();
  }
  if (reading7 == LOW) {
    if ((millis() - lastDebounceTime7) > debounceDelay) {
      Serial.println("Button on pin D7 pressed");
      if (isEditingTime1) {
        Time1.hour -= 1;
        if (Time1.hour < 0) {
          Time1.hour = 23;
        }
      } else {
        Time2.hour -= 1;
        if (Time2.hour < 0) {
          Time2.hour = 23;
        }
      }
      lastDebounceTime7 = millis(); // Reset debounce timer
    }
  }
  lastButtonState7 = reading7;

  // Check for button press on pin D6
  if (reading6 != lastButtonState6) {
    lastDebounceTime6 = millis();
  }
  if (reading6 == LOW) {
    if ((millis() - lastDebounceTime6) > debounceDelay) {
      Serial.println("Button on pin D6 pressed");
      if (isEditingTime1) {
        Time1.hour += 1;
        if (Time1.hour > 23) {
          Time1.hour = 0;
        }
      } else {
        Time2.hour += 1;
        if (Time2.hour > 23) {
          Time2.hour = 0;
        }
      }
      lastDebounceTime6 = millis(); // Reset debounce timer
    }
  }
  lastButtonState6 = reading6;
}
