#include <Wire.h>
#include <ds3231.h>

struct ts t;

void setdateTime() {
  Serial.println("What is day in DD?");
  String inputVal;
  while (inputVal = Serial.readString()) {
    if (inputVal != "") {
      if (isDigit(inputVal[0]))
      {
        t.mday = inputVal.toInt();
        break;
      }
      else {
        Serial.println("Please enter numeric values");
      }
    }
  }

  Serial.println("What is month in MM?");
  inputVal = "";
  while (inputVal = Serial.readString()) {
    if (inputVal != "") {
      if (isDigit(inputVal[0]))
      {
        t.mon = inputVal.toInt();
        break;
      }
      else {
        Serial.println("Please enter numeric values");
      }
    }
  }

  Serial.println("What is month in YYYY?");
  inputVal = "";
  while (inputVal = Serial.readString()) {
    if (inputVal != "") {
      if (isDigit(inputVal[0]))
      {
        t.year = inputVal.toInt();
        break;
      }
      else {
        Serial.println("Please enter numeric values");
      }
    }
  }

  Serial.println("What is hour in HH?");
  inputVal = "";
  while (inputVal = Serial.readString()) {
    if (inputVal != "") {
      if (isDigit(inputVal[0]))
      {
        t.hour = inputVal.toInt();
        break;
      }
      else {
        Serial.println("Please enter numeric values");
      }
    }
  }

  Serial.println("What is Min in mm?");
  inputVal = "";
  while (inputVal = Serial.readString()) {
    if (inputVal != "") {
      if (isDigit(inputVal[0]))
      {
        t.min = inputVal.toInt();
        break;
      }
      else {
        Serial.println("Please enter numeric values");
      }
    }
  }

  Serial.println("What is Sec in ss?");
  inputVal = "";
  while (inputVal = Serial.readString()) {
    if (inputVal != "") {
      if (isDigit(inputVal[0]))
      {
        t.sec = inputVal.toInt();
        break;
      }
      else {
        Serial.println("Please enter numeric values");
      }
    }
  }
  DS3231_set(t);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  //DS3231_init(DS3231_INTCN);
  DS3231_init(0);
  /*----------------------------------------------------------------------------
    In order to synchronise your clock module, insert timetable values below !
    ----------------------------------------------------------------------------*/
  Serial.println("Do you want to set time Y/N?");
  String inputStr, tempStr="";
  while (inputStr = Serial.readString()) {
    if (inputStr != "") {
      Serial.println(inputStr[0]);
      if (inputStr[0] == 'Y' || inputStr[0] == 'y' ) { 
        setdateTime();
        break;
      }
      else {
        break;
      }
    }
  }
}


void loop() {
  DS3231_get(&t);
  Serial.print("Date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year);
  Serial.print("\t Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.println(t.sec);

  delay(1000);
}
