/*
* 7 Segment Alarm Clock With temp and Humidity Sensors
*
* The code includes the clock functionality for communicating with a ds1307,
* Communicating with the adafruit 7 segment led backpacks, and reading the
* temperature and humidity settings. The program uses the millis() feature
* to perform timed tasks without halting execution. The program will display
* the time in 12 hour and 24 hour modes, display temperature and humidity,
* as well as show the full calendar year.
*
* written by John Burns
*
*
*/

//Include the needed Libraries

#include <Wire.h>
#include "RTClib.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>
#define isdigit(n) (n >= '0' && n <= '9')
#define OCTAVE_OFFSET 0
// Declare variables and instantiate objects
Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_7segment matrix1 = Adafruit_7segment();
Adafruit_7segment matrix2 = Adafruit_7segment();
RTC_DS1307 rtc;

// Create constants
const byte hourPin = 4;
const byte minPin = 2;
const byte modePin = 3;
const byte tonePin = 13;
const bool alarmPin = 6;
const byte tempSensor = A1;
const byte humidSensor = A0;
const int notes[] = { 0,
262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};
const char song_20[] PROGMEM = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
const char song_19[] PROGMEM = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char song_18[] PROGMEM = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
const char song_17[] PROGMEM = "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
const char song_16[] PROGMEM = "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
const char song_15[] PROGMEM = "Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
const char song_14[] PROGMEM = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
const char song_13[] PROGMEM = "20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
const char song_12[] PROGMEM = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
const char song_11[] PROGMEM = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
const char song_10[] PROGMEM = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
const char song_9[] PROGMEM = "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
const char song_8[] PROGMEM = "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
const char song_7[] PROGMEM = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
const char song_6[] PROGMEM = "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
const char song_5[] PROGMEM = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
const char song_4[] PROGMEM = "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
const char song_3[] PROGMEM = "Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
const char song_2[] PROGMEM = "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
const char song_1[] PROGMEM = "LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
const char song_0[] PROGMEM = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
const char *song_table[] ={
  song_0,
  song_1,
  song_2,
  song_3,
  song_4,
  song_5,
  song_6,
  song_7,
  song_8,
  song_9,
  song_10,
  song_11,
  song_12,
  song_13,
  song_14,
  song_15,
  song_16,
  song_17,
  song_18,
  song_10,
  song_20 };
 
// Create variables
DateTime alarm;
DateTime now;
float tempC;
float tempF;
bool AMPM;
bool alarmSet;
byte brightness;
byte mode;
byte smode;
byte alarmTone;
float trueRH;
bool setMode;
char buf[421];
byte choice =1;
unsigned long smodeTimer;
unsigned long modePinTimer;
unsigned long buttonTimer;
unsigned long minPinTimer;
unsigned long hourPinTimer;
unsigned long sensorTimer;
unsigned long displayTicks;
unsigned long rtcTicks;

// Setup function only done once.
void setup () {

  // Set pin registers
  pinMode(tempSensor, INPUT);
  pinMode(humidSensor, INPUT);
  pinMode(hourPin, INPUT);
  pinMode(minPin, INPUT);
  pinMode(modePin, INPUT);
  pinMode(tonePin, INPUT);
  // Assign default variable values.
  brightness = 15;
  alarmTone=0;
  strcpy_P(buf, (char*)(song_table[alarmTone]));
  setMode = false;
  smode = 0;
  AMPM = false;
  // Start the various communication buses and set starting values
  matrix.begin(0x71);
  matrix.setBrightness(brightness);
  matrix1.begin(0x70);
  matrix1.setBrightness(brightness);
  matrix2.begin(0x72);
  matrix2.setBrightness(brightness);
  // Conditional compile statements
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();
  // If RTC looses power start the clock and issue a default starting time.
  if (! rtc.isrunning()) {
    rtc.adjust(DateTime(1983, 11, 07, 0, 0, 0));
  }
}

// Main program loop
void loop () {

  // Detect if the Set Mode has been enabled if its not check the pin status.
  if (!setMode) {
    ReadPins();
  }
  else {
    ReadPinsSet();
  }
  // Update displays
  DisplayAll();
  // Poll the sensors ever second
  if (millis() - sensorTimer >= 1000UL) {
    GetTemp();
    GetHumid();
    sensorTimer = millis();
  }

}

//Main display function. This function updates the LED's based on mode and settings menu.
void DisplayAll() {
  // Local variables
  static bool m0select;
  static bool m1select;
  static bool m2select;
  byte colon;
  now = rtc.now();
  byte minutes = now.minute();
  byte hours = now.hour();
  int years = now.year();
  byte months = now.month();
  byte days = now.day();
  byte aminutes = alarm.minute();
  byte ahours = alarm.hour();

  // Sets the period for the LED periods to blink twice a second
  if ( millis() - smodeTimer >= 500UL) {
    if (setMode) {
      if (smode == 0) {
        m2select = !m2select;
        m1select = false;
        m0select = false;
      }
      else if (smode == 1) {
        m1select = !m1select;
        m2select = false;
        m0select = false;
      }
      else if (smode == 2 || 3) {
        m0select = !m0select;
        m2select = false;
        m1select = false;
      }
    }
    else {
      m0select = false;
      m1select = false;
      m2select = false;
    }
    smodeTimer = millis();
  }
  
  // Draw colon ever second on time display
  if (millis() - rtcTicks >= 1000) {
    if (colon == 0) {
      colon = 1;
      matrix2.drawColon(colon);

    }
    else {
      colon = 0;
      matrix2.drawColon(colon);

    }
    matrix2.writeDisplay();

    rtcTicks = millis();
  }
  
  
  // Make sure any changes to the brightness variable is adjusted
  // no later than every 100ms. And apply blink profiles for set modes
  if (millis() - displayTicks >= 100UL) {
    matrix.setBrightness(brightness);
    matrix1.setBrightness(brightness);
    matrix2.setBrightness(brightness);

    if ( (!setMode && mode == 0) || (setMode && smode != 3)) {
      matrix.writeDigitNum(0, (years / 1000) , m0select);
      matrix.writeDigitNum(1, (years / 100) % 10 , m0select);
      matrix.writeDigitNum(3, (years / 10) % 10, m0select);
      matrix.writeDigitNum(4, (years % 10), m0select);
      matrix.drawColon(false);
      matrix.writeDisplay();
    }
    else if ( (!setMode && mode == 2) || (setMode && smode == 3)) {
      matrix.writeDigitNum(0, (ahours / 10) , m0select);
      matrix.writeDigitNum(1, (ahours % 10) , m0select);
      matrix.writeDigitNum(3, (aminutes / 10), m0select);
      matrix.writeDigitNum(4, (aminutes % 10), m0select);
      matrix.drawColon(true);
      matrix.writeDisplay();
    }
    else if ( (!setMode && mode == 1)) {
      int rh = trueRH * 100;
      matrix.writeDigitNum(0, rh / 1000 , m0select);
      matrix.writeDigitNum(1, (rh / 100) % 10 , true);
      matrix.writeDigitNum(3, (rh / 10) % 10, m0select);
      matrix.writeDigitNum(4, rh % 10, m0select);
      matrix.drawColon(false);
      matrix.writeDisplay();
    }
    if (!setMode && mode == 1) {
      int temp = tempF * 100.00;
      matrix1.writeDigitNum(0, temp / 1000 , m1select);
      matrix1.writeDigitNum(1, (temp / 100) % 10 , true);
      matrix1.writeDigitNum(3, (temp / 10) % 10, m1select);
      matrix1.writeDigitNum(4, temp % 10, m1select);
      matrix1.drawColon(false);
      matrix1.writeDisplay();
    }
    else {
      matrix1.writeDigitNum(0, months / 10 , m1select);
      matrix1.writeDigitNum(1, months % 10 , true);
      matrix1.writeDigitNum(3, days / 10, m1select);
      matrix1.writeDigitNum(4, days % 10, m1select);
      matrix1.drawColon(false);
      matrix1.writeDisplay();
    }
    if (AMPM && hours > 12) {
      hours = hours - 12;
    }
    matrix2.writeDigitNum(0, hours / 10 , m2select);
    matrix2.writeDigitNum(1, hours % 10 , m2select);
    matrix2.writeDigitNum(3, minutes / 10, m2select);
    matrix2.writeDigitNum(4, minutes % 10, m2select);
    matrix2.writeDisplay();

    displayTicks = millis();
  }
}


// Read input pins and set variables
void ReadPinsSet() {
  
  // Set local variables
  byte minPinState;
  static byte minPinLastState;
  byte hourPinState;
  static byte hourPinLastState;
  byte modePinState;
  static byte modePinLastState;

  if (millis() - buttonTimer >= 10UL) {
    minPinState = digitalRead(minPin);
    hourPinState = digitalRead(hourPin);
    modePinState = digitalRead(modePin);
    alarmSet = digitalRead(alarmPin);
  }

  if (millis() - buttonTimer >= 100UL) {

    if (minPinState == 0) {
      minPinLastState = 0;
    }
    if (hourPinState == 0) {
      hourPinLastState = 0;
    }
    if (modePinState == 0) {
      modePinLastState = 0;
    }

    if (minPinState == 1 && minPinLastState == 0) {
      minPinLastState = 1;
      minPinTimer = millis();
      if (smode == 0) {
        SetTime('m');
      }
      else if (smode == 1) {
        SetDate('d');
      }
      else if (smode == 2) {
        SetDate('y');
      }
      else if (smode == 3) {
        SetAlarm('m');
      }
      else if (smode ==4) {
        SetAlarmTone();
      }
    }
    if (minPinState == 1 && minPinLastState == 1) {
      if (millis() - minPinTimer >= 1000UL) {
        if (smode == 0) {
          SetTime('m');
        }
        else if (smode == 1) {
          SetDate('d');
        }
        else if (smode == 2) {
          SetDate('y');
        }
        else if (smode == 3) {
          SetAlarm('m');
        }
        else if (smode ==4) {
          SetAlarmTone();
      }
    }

    if (hourPinState == 1 && hourPinLastState == 0 ) {
      hourPinLastState = 1;
      hourPinTimer = millis();
      if (smode == 0) {
        SetTime('h');
      }
      else if (smode == 1) {
        SetDate('m');
      }
      else if (smode == 2) {
        SetDate('z');
      }
      else if (smode == 3) {
        SetAlarm('h');
      }
    }
    if (hourPinState == 1 && hourPinLastState == 1) {
      if (millis() - hourPinTimer >= 1000UL) {
        if (smode == 0) {
          SetTime('h');
        }
        else if (smode == 1) {
          SetDate('m');
        }
        else if (smode == 2) {
          SetDate('z');
        }
        else if (smode == 3) {
          SetAlarm('h');
        }
      }
    }

    if (modePinState == 1 && modePinLastState == 0 ) {
      modePinLastState = 1;
      modePinTimer = millis();
      if (smode + 1 <= 4) {
        smode += 1;
      }
      else {
        smode = 0;
      }
    }
    if (modePinState == 1 && modePinLastState == 1) {
      if (millis() - modePinTimer >= 3000UL) {
        setMode = !setMode;
        modePinTimer = millis();
       
     }

    }

    buttonTimer = millis();
  }
  } 
}

void ReadPins() {

  byte minPinState;
  static byte minPinLastState;
  byte hourPinState;
  static byte hourPinLastState;
  byte modePinState;
  static byte modePinLastState;

  if (millis() - buttonTimer >= 10UL) {
    minPinState = digitalRead(minPin);
    hourPinState = digitalRead(hourPin);
    modePinState = digitalRead(modePin);
    alarmSet = digitalRead(alarmPin);
  }

  if (millis() - buttonTimer >= 100UL) {

    if (minPinState == 0) {
      minPinLastState = 0;
    }
    if (hourPinState == 0) {
      hourPinLastState = 0;
    }
    if (modePinState == 0) {
      modePinLastState = 0;
    }

    if (minPinState == 1 && minPinLastState == 0) {
      minPinLastState = 1;
      minPinTimer = millis();
      if (brightness + 5 <= 15) {
        brightness += 5;
      }
      else {
        brightness = 0;
      }
    }
    if (minPinState == 1 && minPinLastState == 1) {
      if (millis() - minPinTimer >= 1000UL) {
        if (brightness + 5 <= 15) {
          brightness += 5;
        }
        else {
          brightness = 0;
        }
      }
    }

    if (hourPinState == 1 && hourPinLastState == 0 ) {
      hourPinLastState = 1;
      hourPinTimer = millis();
      AMPM = !AMPM;
    }

    if (modePinState == 1 && modePinLastState == 0 ) {
      modePinLastState = 1;
      modePinTimer = millis();
      if (mode + 1 <= 2) {
        mode += 1;
      }
      else {
        mode = 0;
      }
    }
    if (modePinState == 1 && modePinLastState == 1) {
      if (millis() - modePinTimer >= 3000UL) {
        setMode = !setMode;
        modePinTimer = millis();
      }
    }

    buttonTimer = millis();
  }

}

void SetDate(char opt) {
  now = rtc.now();

  if (opt == 'm') {
    if (now.month() + 1 <= 12) {
      rtc.adjust(DateTime(now.year(), now.month() + 1, now.day(), now.hour(), now.minute(), now.second() ));
    }
    else {
      rtc.adjust(DateTime(now.year(), 1, now.day(), now.hour(), now.minute() , now.second() ));
    }
    DisplayAll();
  }
  if (opt == 'd') {

    if (now.month() == 2) {
      if (now.day() + 1 <= 28) {

        rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour() , now.minute() , now.second() ));
      }
      else {
        rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
      }
    }
    else if (now.month() == 9 || now.month() == 11) {
      if (now.day() + 1 <= 30) {

        rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour() , now.minute() , now.second() ));
      }
      else {
        rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
      }
    }
    else if (now.day() + 1 <= 31) {

      rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour() , now.minute() , now.second() ));
    }
    else {
      rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute() , now.second() ));
    }
    DisplayAll();
  }

  if (opt == 'y') {
    if (now.year() + 1 <= 2100) {
      rtc.adjust(DateTime(now.year() + 1, now.month(), now.day(), now.hour(), now.minute(), now.second() ));
    }
    else {
      rtc.adjust(DateTime(2015, now.month(), now.day(), now.hour(), now.minute() , now.second() ));
    }
    DisplayAll();
  }
  if (opt == 'z') {
    if (now.year() + 10 <= 2100) {
      rtc.adjust(DateTime(now.year() + 10, now.month(), now.day(), now.hour() , now.minute() , now.second() ));
    }
    else {
      rtc.adjust(DateTime(2015, now.month(), now.day(), now.hour(), now.minute() , now.second() ));
    }
    DisplayAll();
  }


}
void SetTime(char opt) {

  now = rtc.now();

  if (opt == 'm') {
    if (now.minute() + 1 <= 59) {
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute() + 1, 0 ));
    }
    else {
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), 0 , 0 ));
    }
    DisplayAll();
  }
  if (opt == 'h') {
    if (now.hour() + 1 <= 23) {
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() + 1, now.minute() , now.second() ));
    }
    else {
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute() , now.second() ));
    }
    DisplayAll();
  }
}
void SetAlarm(char opt) {


  if (opt == 'm') {
    if (alarm.minute() + 1 <= 59) {
      alarm = DateTime(alarm.year(), alarm.month(), alarm.day(), alarm.hour(), alarm.minute() + 1, 0 );
    }
    else {
      alarm = DateTime(alarm.year(), alarm.month(), alarm.day(), alarm.hour(), 0 , 0 );
    }
    DisplayAll();
  }
  if (opt == 'h') {
    if (alarm.hour() + 1 <= 23) {
      alarm = DateTime(alarm.year(), alarm.month(), alarm.day(), alarm.hour() + 1, alarm.minute() , 0 );
    }
    else {
      alarm = DateTime(alarm.year(), alarm.month(), alarm.day(), 0, alarm.minute() , 0 );
    }
    DisplayAll();
  }

}


void GetHumid() {
  static int hreading[100];
  static byte hindex = 0;
  static unsigned int htotal = 0;
  int haverage = 0;
  float hvoltage;
  static unsigned long humidTimer;

  if (millis() - humidTimer >= 10) {
    htotal = htotal - hreading[hindex];
    hreading[hindex] = analogRead(humidSensor);
    htotal = htotal + hreading[hindex];
    hindex += 1;
    humidTimer = millis();
  }

  if (hindex == 100) {
    hindex = 0;
  }
  haverage = htotal / 100;
  hvoltage = haverage * 5.0;
  hvoltage /= 1024.0;
  trueRH = (((hvoltage / 5.0) - 0.16) / 0.0062) / 1.0546 - 0.00216 * tempC;

}

void GetTemp() {
  static unsigned long tempTimer;
  float voltage;
  static int treading[100];
  static byte tindex = 0;
  static unsigned int ttotal = 0;
  int taverage = 0;

  if (millis() - tempTimer >= 10) {
    ttotal = ttotal - treading[tindex];
    treading[tindex] = analogRead(tempSensor);
    ttotal = ttotal + treading[tindex];
    tindex += 1;
    tempTimer = millis();
  }
  if (tindex == 100) {
    tindex = 0;
  }
  taverage = ttotal / 100;
  voltage = taverage * 5.0;
  voltage /= 1024;
  tempC = (voltage - 0.5) * 100;
  tempF = (tempC * 9.0 / 5.0) + 32;
}

// Checks if the alarm should go off and activates the alarm tone.

void SetAlarmTone() {
  // Local variable declaration

  
  if (alarmTone + 1 <= 21) {
   alarmTone += 1; 
   strcpy_P(buf, (char*)(song_table[alarmTone]));
  }
  else {
    alarmTone = 0;
    strcpy_P(buf, (char*)(song_table[alarmTone]));
  }
  PlayAlarm(1);
}

void CheckAlarm() {
  
  if (alarmSet) {
    if (now.hour() == alarm.hour() && now.minute() == alarm.minute()) {
     PlayAlarm(0);   
    }
  }
}



void PlayAlarm(bool demo)
{

char *p = buf;  
if ((smode==4 && setMode) || alarmSet) 
{
  // Absolutely no error checking in here
  byte counter=0;
  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }



  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  // now begin note loop
  while(*p)
  {
      alarmSet = digitalRead(alarmPin);
      if ((demo && counter==10) || (!demo && !alarmSet)) return;
    // first, get note duration, if available
    num = 0;
    
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      tone(tonePin, notes[(scale - 4) * 12 + note]);
      delay(duration);
      noTone(tonePin);
    }
    else
    {
      delay(duration);
    }
   counter+= 1; 
  }
}
}

