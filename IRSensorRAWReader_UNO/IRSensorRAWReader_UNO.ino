/*


    results.decode_type: Will be one of the following: NEC, SONY, RC5, RC6, or UNKNOWN.
    results.value: The actual IR code (0 if type is UNKNOWN)
    results.bits: The number of bits used by this code
    results.rawbuf: An array of IR pulse times
    results.rawlen: The number of items stored in the array




*/
#include <IRremote.h>  //including infrared remote header file
const int RECV_PIN = 4; // the pin where you connect the output pin of IR sensor
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
     irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.
    {
        Serial.println(results.value);
        irrecv.resume(); // Restart the ISR state machine and Receive the next value
        
    }
   
}
