#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS3231 rtc;

#define YELLOW 8
#define GREEN  9
#define RED    10

#define SW     4
#define BUZZER 7

byte morning_taken=0;
byte afternoon_taken=0;
byte night_taken=0;

byte reminder_active=0;

byte hour_=0;
byte min_=0;
byte sec_=0;

void rtc_read_time()
{
    DateTime now = rtc.now();

    sec_  = now.second();
    min_  = now.minute();
    hour_ = now.hour();
}

void rtc_set_time(byte hh, byte mm, byte ss)
{
    rtc.adjust(DateTime(2026,1,1,hh,mm,ss));
}

void display_time()
{
    char txt[17];

    sprintf(txt,"%02u:%02u:%02u",
            hour_,min_,sec_);

    lcd.setCursor(0,0);
    lcd.print("TIME:        ");

    lcd.setCursor(0,1);
    lcd.print("                ");

    lcd.setCursor(0,1);
    lcd.print(txt);
}

void setup()
{
    pinMode(YELLOW,OUTPUT);
    pinMode(GREEN,OUTPUT);
    pinMode(RED,OUTPUT);

    pinMode(BUZZER,OUTPUT);

    pinMode(SW,INPUT_PULLUP);

    Wire.begin();

    rtc.begin();

    lcd.init();
    lcd.backlight();

    digitalWrite(YELLOW,HIGH);
    digitalWrite(GREEN,HIGH);
    digitalWrite(RED,HIGH);

    digitalWrite(BUZZER,LOW);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SMART MEDICINE");

    lcd.setCursor(0,1);
    lcd.print("REMINDER BOX");

    delay(2000);



    // FIRST TIME ONLY
    rtc_set_time(8,59,55);
}

void loop()
{
    rtc_read_time();

    if(hour_==0 && min_==0 && sec_==0)
    {
        morning_taken=0;
        afternoon_taken=0;
        night_taken=0;
    }

    // MORNING

    if(hour_==9 && min_==0 && sec_==0 &&
       morning_taken==0)
    {
        reminder_active=1;

        digitalWrite(YELLOW,HIGH);
        digitalWrite(GREEN,LOW);
        digitalWrite(RED,LOW);

        digitalWrite(BUZZER,HIGH);

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("TAKE MEDICINE");

        lcd.setCursor(0,1);
        lcd.print("MORNING");
    }

    if(reminder_active &&
       digitalRead(SW)==LOW)
    {
        delay(50);

        if(digitalRead(SW)==LOW)
        {
            if(hour_>=9 && hour_<=10)
            {
                morning_taken=1;

                reminder_active=0;

                digitalWrite(BUZZER,LOW);

                digitalWrite(YELLOW,LOW);
                digitalWrite(GREEN,HIGH);
                digitalWrite(RED,LOW);

                lcd.clear();

                lcd.setCursor(0,0);
                lcd.print("MORNING");

                lcd.setCursor(0,1);
                lcd.print("MEDICINE TAKEN");

                delay(1000);
            }

            if(hour_>=13 && hour_<=14)
            {
                afternoon_taken=1;

                reminder_active=0;

                digitalWrite(BUZZER,LOW);

                digitalWrite(YELLOW,LOW);
                digitalWrite(GREEN,HIGH);
                digitalWrite(RED,LOW);

                lcd.clear();

                lcd.setCursor(0,0);
                lcd.print("AFTERNOON");

                lcd.setCursor(0,1);
                lcd.print("MEDICINE TAKEN");

                delay(1000);
            }

            if(hour_>=20 && hour_<=21)
            {
                night_taken=1;

                reminder_active=0;

                digitalWrite(BUZZER,LOW);

                digitalWrite(YELLOW,LOW);
                digitalWrite(GREEN,HIGH);
                digitalWrite(RED,LOW);

                lcd.clear();

                lcd.setCursor(0,0);
                lcd.print("NIGHT");

                lcd.setCursor(0,1);
                lcd.print("MEDICINE TAKEN");

                delay(1000);
            }
        }
    }

    if(hour_==9 &&
       min_==1 &&
       sec_==0 &&
       morning_taken==0)
    {
        reminder_active=0;

        digitalWrite(BUZZER,LOW);

        digitalWrite(YELLOW,LOW);
        digitalWrite(GREEN,LOW);
        digitalWrite(RED,HIGH);

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("MISSED MEDICINE");

        lcd.setCursor(0,1);
        lcd.print("MORNING");

        delay(1000);
    }

    // AFTERNOON

    if(hour_==13 &&
       min_==0 &&
       sec_==0 &&
       afternoon_taken==0)
    {
        reminder_active=1;

        digitalWrite(YELLOW,HIGH);
        digitalWrite(GREEN,LOW);
        digitalWrite(RED,LOW);

        digitalWrite(BUZZER,HIGH);

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("TAKE MEDICINE");

        lcd.setCursor(0,1);
        lcd.print("AFTERNOON");

        delay(1000);
    }

    if(hour_==13 &&
       min_==1 &&
       sec_==0 &&
       afternoon_taken==0)
    {
        digitalWrite(BUZZER,LOW);

        digitalWrite(YELLOW,LOW);
        digitalWrite(GREEN,LOW);
        digitalWrite(RED,HIGH);

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("MISSED MEDICINE");

        lcd.setCursor(0,1);
        lcd.print("AFTERNOON");

        delay(1000);
    }

    // NIGHT

    if(hour_==20 &&
       min_==0 &&
       sec_==0 &&
       night_taken==0)
    {
        reminder_active=1;

        digitalWrite(YELLOW,HIGH);
        digitalWrite(GREEN,LOW);
        digitalWrite(RED,LOW);

        digitalWrite(BUZZER,HIGH);

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("TAKE MEDICINE");

        lcd.setCursor(0,1);
        lcd.print("NIGHT");

        delay(1000);
    }

    if(hour_==20 &&
       min_==0 &&
       sec_==10 &&
       night_taken==0)
    {
        digitalWrite(BUZZER,LOW);

        digitalWrite(YELLOW,LOW);
        digitalWrite(GREEN,LOW);
        digitalWrite(RED,HIGH);

        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("MISSED MEDICINE");

        lcd.setCursor(0,1);
        lcd.print("NIGHT");

        delay(1000);
    }

     display_time();
}
