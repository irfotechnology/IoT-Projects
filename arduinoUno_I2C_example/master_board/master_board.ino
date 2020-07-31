/*
https://www.youtube.com/watch?v=PnG4fO5_vU4
https://dronebotworkshop.com/i2c-arduino-arduino/
*/

// Include Arduino Wire library for I2C
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 0x09

// Analog pin for potentiometer
int analogPin = 0;
// Integer to hold potentiometer value
int val = 0;

void setup() {

  // Initialize I2C communications as Master
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  delay(50);
  
  // Read pot value 
  // Map to range of 1-255 for flash rate
    val = map(analogRead(analogPin), 0, 1023, 255, 1);
    
  // Write a charatre to the Slave
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(byte(0x00));
  Wire.write(val);
  Wire.endTransmission();

}
