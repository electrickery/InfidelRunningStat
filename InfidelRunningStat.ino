
#define VERSION "v0.3"

#include <Wire.h>

//I2C addresses for Infidel sensor
#define INFIDELADD 43

#define INFIDEL_FAULT A1

#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "RunningStat.cpp"

RunningStat rsd;

#define SERIALBUFSIZE         80
char serialBuffer[SERIALBUFSIZE];
byte setBufPointer = 0;

int interval = 5000;

char busy[] = {'|', '-', '|', '-'};
int busyIndex = 0;

int running = true;
int muted = false;

void setup() {
  
  Serial.begin(9600);
  Serial.print(F("Infidel RunningStat test "));
  Serial.print(VERSION);
  Serial.println(F("!"));
  
  Wire.begin();
  
  pinMode(INFIDEL_FAULT, INPUT_PULLUP);

  lcd.begin(20, 4);

  lcd.print("Infidel RunningStat");
  lcd.setCursor(0, 1);
  lcd.print(VERSION);

}

void loop() {
  int ccInterval = 500;
  int partialIntervalCount = interval / ccInterval;
  for (int p=0; p < partialIntervalCount; p++) {
    commandCollector();
    delay(ccInterval);
  }

  if(!running) return;
    
  float infidelin = 0;
  Serial.print("  ");
  if (digitalRead(INFIDEL_FAULT)) {
    Serial.println("Infidel fault ");
  } else {
    Wire.requestFrom(INFIDELADD, 2);
    byte b1 = Wire.read();
    byte b2 = Wire.read();
    infidelin = (((float) b1) * 256 + b2) / 1000;

    if (!muted) {
  
    Serial.print("Diameter: ");
    Serial.print(infidelin);
    Serial.print(" mm");

    rsd.Push(infidelin);
    
    Serial.print(", Count: ");
    Serial.print(rsd.NumDataValues());
    Serial.print(", Mean: ");
    Serial.print(rsd.Mean(), 4);
    Serial.print(", Variance: ");
    Serial.print(rsd.Variance(), 6);
    Serial.print(", Stdev: ");
    Serial.print(rsd.StandardDeviation(), 4);

    Serial.print(", Min: ");
    Serial.print(rsd.Min());
    Serial.print(", Max: ");
    Serial.print(rsd.Max());
    Serial.println(",");
    }
  }
  

// LCD 
  lcd.clear();

  lcd.setCursor(0, 0);
  if (digitalRead(INFIDEL_FAULT)) {
    lcd.print("Infidel fault");
    lcd.setCursor(0, 3);
    lcd.print(busy[busyIndex]);
    busyIndex = (busyIndex < 3) ?  busyIndex+1 : 0;
  } else {
    lcd.print("Diameter: ");
    lcd.print(infidelin);
    lcd.print(" mm");
    
    lcd.setCursor(0, 1);
    lcd.print("Avg,SD: ");
    lcd.print(rsd.Mean(), 3);
//    lcd.print(", Variance: ");
//    lcd.print(rsd.Variance());
    lcd.print(",");
    lcd.print(rsd.StandardDeviation(), 4);
    
    lcd.setCursor(0, 2);
    lcd.print("Min,Max: ");
    lcd.print(rsd.Min());
    lcd.print(",");
    lcd.print(rsd.Max());
      
    lcd.setCursor(0, 3);
    lcd.print("Count: ");
    lcd.print(rsd.NumDataValues());
  }
}

String make2digits(int number) {
  char buf[3];
  sprintf(buf, "%02d", number);
  return buf;
}

void commandCollector() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch(inByte) {
    case '.':
//    case '\r':
    case '\n':
      commandInterpreter();
      clearSerialBuffer();
      setBufPointer = 0;
      break;
    case '\r':
      break;  // ignore carriage return
    default:
      serialBuffer[setBufPointer] = inByte;
      setBufPointer++;
      if (setBufPointer >= SERIALBUFSIZE) {
        Serial.println("Serial buffer overflow. Cleanup.");
        clearSerialBuffer();
        setBufPointer = 0;
      }
    }
  }
}

void commandInterpreter() {
  byte bufByte = serialBuffer[0];
  
  switch(bufByte) {
//    case 'I':
//    case 'i':
//      setInterval();
//      break;
    case 'C':
    case 'c':
      clear();
      break;
    case 'H':
    case 'h':
    case '?':
      Serial.println("Usage: [Hh?] - help");
      Serial.println("       [Cc] - clear running stat");
//      Serial.println("       [Ii]nnnn - set interval") ;
      Serial.println("       [Mm] - mute report");
      Serial.println("       [Pp] - pause running stat");
      Serial.println("       [Rr] - unmute report");
      Serial.println("       [Ss] - start running stat");
//      Serial.println("       [Tt]HHMMSS - set time") ;
      break;
    case 'M':
    case 'm':
      mute();
      break;
    case 'P':
    case 'p':
      pause();
      break;
    case 'R':
    case 'r':
      pause();
      break;
    case 'S':
    case 's':
      start();
      break;
    default:
      Serial.print(bufByte);
      Serial.print(" ");
      Serial.println("unsupported");
      return;
  }
}

void clearSerialBuffer() {
  byte i;
  for (i = 0; i < SERIALBUFSIZE; i++) {
    serialBuffer[i] = 0;
  }
}

// T, t - set RTC time
/* void setTime() {
  if (setBufPointer != 7) {
    Serial.println("ERROR T arg. size");
    clearSerialBuffer();
    return; 
  }
  int hour = getDigit(serialBuffer[1]) * 10;
  hour += getDigit(serialBuffer[2]);
  int minute = getDigit(serialBuffer[3]) * 10;
  minute += getDigit(serialBuffer[4]);
  int second = getDigit(serialBuffer[5]) * 10;
  second += getDigit(serialBuffer[6]);

  Serial.print("T");
  Serial.print(make2digits(hour));
  Serial.print(make2digits(minute));
  Serial.print(make2digits(second));

} */

int getDigit(char numberDigit) {
    if (numberDigit < '0' || numberDigit > '9') {
      return 0;
    }
    return numberDigit - '0';
}

void start() {
    running = true;
}

void pause() {
    running = false;
}

void clear() {
    rsd.Clear();
}

void mute() {
  muted = true;
}

void report() {
  muted = false;
}
