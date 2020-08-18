#include <IRremote.h>  //including infrared remote header file
#include "IRChinessRemote.h"

const int RECV_PIN = 4; // the pin where you connect the output pin of IR sensor
IRrecv irrecv(RECV_PIN);
decode_results results;
IRChinessRemote remote;

//unsigned long lastCode;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("Programe Started");
  remote = IRChinessRemote();
}

void loop()
{
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.
  {
    //Serial.println(results.value, HEX);

    char * Key = remote.FIND_KEY(results.value);
    if ( Key != "" ) {
      Serial.println(Key);
    }
    delay(30);
    irrecv.resume(); // Restart the ISR state machine and Receive the next value
  }
}
