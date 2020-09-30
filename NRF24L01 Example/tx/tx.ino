/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
unsigned long count = 0;
const byte address[6] = "000786";
unsigned long previousMillis = 0; 
const long interval = 200;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
 // radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    //code here after delay...
      count++;
      String msg = "Hello World : ";
      msg.concat(count);
      //Serial.println(msg);
      const char text[msg.length()+1];
      msg.toCharArray(text, msg.length()+1);
      radio.write(&text, sizeof(text));
      Serial.print("Message sent : ");
      Serial.println(text);
  }
}
