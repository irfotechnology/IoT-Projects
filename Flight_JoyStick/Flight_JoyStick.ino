#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define the digital inputs
#define jB1 2  // Joystick button 1
#define jB2 3  // Joystick button 2
#define pinTone 4   //Buzzer
bool armed;
bool disarmed;
bool armStart;
//create objects
RF24 radio(7, 8);   // nRF24L01 (CE, CSN) 
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

void startUpTone() {
      digitalWrite(pinTone, HIGH);
      delay(200);
      digitalWrite(pinTone, LOW);
      delay(200);
}

void linkLostTone(){
  int i=0;
  for(i=0;i<6; i++){
      digitalWrite(pinTone, HIGH);
      delay(100);
      digitalWrite(pinTone, LOW);
      delay(100);
   }
}

void linkConnectTone(){
  int i=0;
  for(i=0;i<2; i++){
      digitalWrite(pinTone, HIGH);
      delay(100);
      digitalWrite(pinTone, LOW);
      delay(100);
   }
}

void resetPacket(){
  // Set initial default values
//  data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
//  data.j1PotY = 127;
//  data.j2PotX = 127;
//  data.j2PotY = 128;
//  data.j1Button = 1;
//  data.j2Button = 1;
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.arm = 0;
//  data.AUX1 = 1;
 // data.AUX2 = 1;
 // data.GPSSwtich = 0;
  armed = false;
  armStart = false;
  disarmed = false;
}

/**************************************************/

// Returns a corrected value for a joystick position that takes into account
// the values of the outer extents and the middle of the joystick range.
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}


void setup() {
  Serial.begin(9600);
  pinMode(pinTone, OUTPUT);  //BUZZER
  // Activate the Arduino internal pull-up resistors
  pinMode(jB1, INPUT_PULLUP);
  pinMode(jB2, INPUT_PULLUP);
  
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  resetPacket();
  startUpTone();
}

void loop() {
  //Serial.println(analogRead(A1));
    // Read all analog inputs and map them to one Byte value
//  data.j1PotX = map(analogRead(A0), 0, 892, 255, 0); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
//  data.j1PotY = map(analogRead(A1), 0, 892, 255, 0);
//  data.j2PotX = map(analogRead(A3), 0, 892, 255, 0);
//  data.j2PotY = map(analogRead(A2), 0, 892, 255, 0);
  // Read all digital inputs
//  data.j1Button = digitalRead(jB1);
//  data.j2Button = digitalRead(jB2);
  int j1Button = digitalRead(jB1);

  // Drone Data
  // The calibration numbers used here should be measured 
  // for your joysticks till they send the correct values.
  //data.pitch    = mapJoystickValues( analogRead(A0), 0, 434, 883, true );
  //data.roll     = mapJoystickValues( analogRead(A1), 0, 451, 884, true );
  //data.throttle = mapJoystickValues( analogRead(A2), 0, 451, 884, true );
  //data.yaw      = mapJoystickValues( analogRead(A3),  0, 438, 883, true );

  data.pitch    =  map(analogRead(A0), 0, 1023, 255, 0);
  data.roll     =  map(analogRead(A1), 0, 1023, 255, 0);
  data.throttle =  map(analogRead(A2), 0, 1023, 255, 0);
  data.yaw      =  map(analogRead(A3), 0, 1023, 255, 0);
 

  if( armStart == true && data.yaw>=240 && armed==false){
      armStart=false;
      armed = true;
      disarmed = false;
      linkConnectTone();
  }
  else if( data.throttle <10 && data.yaw<10 && armed==true && disarmed ==true){
      armStart=false;
      armed = false;
      linkLostTone();
  }
  else if(data.throttle <10 && j1Button==0 && armStart == false){
      armStart = true;
  }
  else if(armed ==true && disarmed==false && j1Button==0){
      disarmed = true;
  }
  data.arm     = armed;

  if(data.arm == 0){
    data.throttle = 0;
  }

  //Serial.print(analogRead(A0));
  //Serial.print("  ");
  //Serial.print(analogRead(A1));
  //Serial.print("  ");
  //Serial.print(analogRead(A2));
  //Serial.print("  ");
  //Serial.print(analogRead(A3));
  //Serial.println("  ");
  
  //Send to serial
 // Serial.print(data.j1PotX);
 // Serial.print("  ");
 //Serial.print(data.j1PotY);
 // Serial.print("  ");
 // Serial.print(data.j2PotX);
 // Serial.print("  ");
 // Serial.print(data.j2PotY);
 // Serial.print("  ");
 //Serial.print(data.j1Button);
 //Serial.print("  ");
 // Serial.print(data.j2Button);
 // Serial.println("  ");

 // Serial.print(data.throttle);
 // Serial.print("  ");
 // Serial.print(data.yaw);
 // Serial.print("  ");
 // Serial.print(data.pitch);
 // Serial.print("  ");
 // Serial.print(data.roll);
 // Serial.print("  ");
 // Serial.print(data.arm);
 // Serial.println("  ");
  // Send the whole data from the structure to the receiver
  String msg = "";
  msg = "[" + (String)data.throttle + "#" + (String)data.yaw + "#" + (String)data.pitch + "#" + (String)data.roll + "#" + data.arm + "]";
  const char text[msg.length()+1];
  msg.toCharArray(text, msg.length()+1);
  Serial.println(msg);
  radio.write(&text, sizeof(text));
  delay(200);
}
