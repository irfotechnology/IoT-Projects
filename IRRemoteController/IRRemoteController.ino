#include <IRremote.h>

IRrecv irrecv(4);
decode_results results;

void setup()
{
  	pinMode(4, INPUT);
  Serial.begin(9600);
   irrecv.enableIRIn(); // Start the receiver

}

void loop()
{
 	if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);
        irrecv.resume(); // Receive the next value
    }
    delay(100);
}