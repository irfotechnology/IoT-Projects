#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 0x0A

// Define LED Pin
int LED = 13;

// Variable for received data
int rd;

// Variable for blink rate
int br;

void setup() {

  pinMode(LED, OUTPUT);
  
  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
   
  // Function to run when data received from master
  Wire.onReceive(receiveEvent);
  
  // Setup Serial Monitor 
  Serial.begin(9600);
  Serial.println("I2C Slave Demonstration");
}

void loop() {
   delay(50);
   digitalWrite(LED, rd);  
}


//this function is called when I2C event is received
void receiveEvent(int howMany){
  while(Wire.available()){
    //read received character
    unsigned char c = Wire.read();
    //let's use an over-simplified MCP23008 emulation.  0x9 = GPIO.  Next byte is data byte to be sent to LCD.
    //seems to work just fine.
    if(c == 0x00)
    {
      while(!Wire.available()) { delay(5);}
      rd = (int) Wire.read();
      Serial.println(rd);
    }
  }
}