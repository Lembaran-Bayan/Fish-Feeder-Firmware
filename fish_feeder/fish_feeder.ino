#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <Servo.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address (0x27) if needed
Servo servo;
int servoPin = 3;
int motorPin = 5;

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
    // time += ":";
    // if (this->minute < 10) {
    //   time += "0";
    // }
    // time += String(this->minute);
    return time;
  }
} Time1, Time2;

int servoAngle = 90;

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

int editIndex = 1;
bool trigger = false;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin8, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);

  servo.attach(servoPin);

  Wire.begin();
  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
  DateTime now = rtc.now();

  // Run Motor and Servo on schedule
  if ((now.hour() == Time1.hour || now.hour() == Time2.hour) && now.minute() == 0 && now.second() == 0) {
    Serial.println("Trigger state on");
    trigger = true;
  }
  if ((now.hour() == Time1.hour || now.hour() == Time2.hour) && now.minute() == 0 && now.second() == 10) {
    Serial.println("Trigger state off");
    trigger = false;
  }
  if ((now.hour() == Time1.hour || now.hour() == Time2.hour) && now.minute() == 30 && now.second() == 0) {
    Serial.println("Trigger state on");
    trigger = true;
  }
  if ((now.hour() == Time1.hour || now.hour() == Time2.hour) && now.minute() == 30 && now.second() == 10) {
    Serial.println("Trigger state off");
    trigger = false;
  }

  // Motor and Servo Logic
  if (trigger) {
    analogWrite(motorPin, 255); // Start motor

    delay(200); // Wait 200 ms

    servo.write(100 - servoAngle); // Move servo to the calculated angle
  } else {
    analogWrite(motorPin, 0); // Stop motor
    servo.write(100);         // Reset servo position
  }

  // Display
  lcd.setCursor(0, 0);
  lcd.print("  ");
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
  if (editIndex == 1) {
    lcd.print(" [1]");
  } else if (editIndex == 2) {
    lcd.print(" [2]");
  } else if (editIndex == 3) {
    lcd.print(" [3]");
  } else {
    lcd.print(" [4]");
  }

  lcd.setCursor(0, 1);
  lcd.print("  ");
  lcd.print(Time1.getString());
  lcd.print(" ");
  lcd.print(Time2.getString());
  lcd.print(" ");
  lcd.print(String(servoAngle));
  lcd.print(" ");
  if (!trigger) {
    lcd.print("OFF");
  } else {
    lcd.print(" ");
    lcd.print("ON");
  }

  // Buttons
  int reading8 = digitalRead(buttonPin8);
  int reading7 = digitalRead(buttonPin7);
  int reading6 = digitalRead(buttonPin6);

  if (reading8 != lastButtonState8) {
    lastDebounceTime8 = millis();
  }
  if (reading8 == LOW) {
    if ((millis() - lastDebounceTime8) > debounceDelay) {
      Serial.println("Button on pin D8 pressed");
      editIndex += 1;
      if (editIndex > 4) {
        editIndex = 0;
      }
      lastDebounceTime8 = millis();
    }
  }
  lastButtonState8 = reading8;

  if (reading7 != lastButtonState7) {
    lastDebounceTime7 = millis();
  }
  if (reading7 == LOW) {
    if ((millis() - lastDebounceTime7) > debounceDelay) {
      Serial.println("Button on pin D7 pressed");
      if (editIndex == 1) {
        Time1.hour -= 1;
        if (Time1.hour < 0) {
          Time1.hour = 23;
        }
      } else if (editIndex == 2) {
        Time2.hour -= 1;
        if (Time2.hour < 0) {
          Time2.hour = 23;
        }
      } else if (editIndex == 3) {
        servoAngle -= 10;
        if (servoAngle < 10) {
          servoAngle = 90;
        }
      } else {
        trigger = !trigger;
      }
      lastDebounceTime7 = millis();
    }
  }
  lastButtonState7 = reading7;

  if (reading6 != lastButtonState6) {
    lastDebounceTime6 = millis();
  }
  if (reading6 == LOW) {
    if ((millis() - lastDebounceTime6) > debounceDelay) {
      Serial.println("Button on pin D6 pressed");
      if (editIndex == 1) {
        Time1.hour += 1;
        if (Time1.hour > 23) {
          Time1.hour = 0;
        }
      } else if (editIndex == 2) {
        Time2.hour += 1;
        if (Time2.hour > 23) {
          Time2.hour = 0;
        }
      } else if (editIndex == 3) {
        servoAngle += 10;
        if (servoAngle > 90) {
          servoAngle = 10;
        }
      } else {
        trigger = !trigger;
      }
      lastDebounceTime6 = millis();
    }
  }
  lastButtonState6 = reading6;
}
