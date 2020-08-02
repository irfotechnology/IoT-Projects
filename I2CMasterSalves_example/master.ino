#include <Wire.h>

#define SLAVE1_ADDR 0x09
#define SLAVE2_ADDR 0x0A
// Analog pin for potentiometer
int analogPin1 = 0;
int analogPin2 = 1;
// Integer to hold potentiometer value
int val1 = 0;
int val2 = 0;
void setup() {

  // Initialize I2C communications as Master
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  delay(50);
  
  // Read pot value 
  // Map to range of 1-255 for flash rate
  // val = map(analogRead(analogPin), 0, 1023, 255, 1);
  val1 = map(analogRead(analogPin1), 10, 1023, 0, 1);
  val2 = map(analogRead(analogPin2), 10, 1023, 0, 1);
  //Serial.println((val1));
  // Write a charatre to the Slave
  Wire.beginTransmission(SLAVE1_ADDR);
  Wire.write(byte(0x00));
  Wire.write(val1);
  Wire.endTransmission();
  
  Wire.beginTransmission(SLAVE2_ADDR);
  Wire.write(byte(0x00));
  Wire.write(val2);
  Wire.endTransmission();
}