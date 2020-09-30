#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Pinouts
//MOSI  <==>  11
//MISO  <==> 12
//SCK   <==>   13



//create objects
RF24 radio(9, 10);   // nRF24L01 (CE, CSN) 
const byte address[6] = "000786"; // Remember that this code is the same as in the transmitter and receiver
// Instanciate a Chrono object.

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  //byte j1PotX;
  //byte j1PotY;
  //byte j1Button;
  //byte j2PotX;
  //byte j2PotY;
  //byte j2Button;
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte arm;      //0-DISARMED 1-ARMED
  //byte AUX1;
  //byte AUX2;
  //byte GPSSwtich; //0-OFF 1-ON
};

Data_Package data; //Create a variable with the above structure

void resetPacket(){
  // Set initial default values
  //data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  //data.j1PotY = 127;
  //data.j2PotX = 127;
  //data.j2PotY = 127;
  //data.j1Button = 1;
  //data.j2Button = 1;
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.arm = 0;
  //data.AUX1 = 1;
  //data.AUX2 = 1;
  //data.GPSSwtich = 0;
}

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  resetPacket();
  // Define the radio communication
  Serial.print("initialize Radio...");
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);  //Must have this line
  //we start the radio comunication
  radio.startListening();
  Serial.print("Radio Listening...");
}


/**************************************************/

unsigned long lastRecvTime = 0;

void recvData()
{
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));
    lastRecvTime = millis(); //here we receive the data
    //Serial.println(data.throttle);
    //digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    //digitalWrite(LED_BUILTIN, LOW);
  }
}

/**************************************************/

void loop()
{
  recvData();
  unsigned long now1 = millis();
  //Here we check if we've lost signal, if we did we reset the values 
  if ( now1 - lastRecvTime > 1000 ) {
    // Signal lost?
    //Serial.println("Signal lost...");
    //digitalWrite(LED_BUILTIN, LOW);
    resetPacket();
    //return;
  }
  Serial.print("Throttle: "); Serial.print(data.throttle);  Serial.print("    ");
  Serial.print("Yaw: ");      Serial.print(data.yaw);       Serial.print("    ");
  Serial.print("Pitch: ");    Serial.print(data.pitch);     Serial.print("    ");
  Serial.print("Roll: ");     Serial.print(data.roll);      Serial.print("    ");
  Serial.print("Arm: ");     Serial.print(data.arm);      Serial.println("    ");
  //Serial.print("Aux2: ");     Serial.print(data.AUX2);      Serial.print("\n");
}

/**************************************************/
