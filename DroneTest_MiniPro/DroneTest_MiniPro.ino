#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Pinouts
//MOSI  <==>  11
//MISO  <==> 12
//SCK   <==>   13

int pwm = 3;  //PWM output on pin D3

//create objects
RF24 radio(7, 8);   // nRF24L01 (CE, CSN) 
const byte address[6] = "000786"; // Remember that this code is the same as in the transmitter and receiver
unsigned long lastRecvTime = 0;
/**************************************************/

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

/**************************************************/
// Instanciate a Chrono object.
Data_Package data; //Create a variable with the above structure

void resetPacket(){
  // Set initial default values
  //data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  //data.j1PotY = 127;
  //data.j2PotX = 127;
  //data.j2PotY = 127;
  //data.j1Button = 1;
  //data.j2Button = 1;
  data.throttle = 80;
  data.yaw = 80;
  data.pitch = 80;
  data.roll = 80;
  data.arm = 0;
  //data.AUX1 = 1;
  //data.AUX2 = 1;
  //data.GPSSwtich = 0;
}

/**************************************************/

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(9,LOW);
  
  Serial.begin(9600);
  Serial.println("Restarting....");
  resetPacket();
  lastRecvTime = millis(); //here we receive the data
  // Define the radio communication
  //Serial.print("initialize Radio...");
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);  //Must have this line
  //we start the radio comunication
  radio.startListening();
  //Serial.print("Radio Listening...");
}


/**************************************************/

void recvData()
{
  //digitalWrite(LED_BUILTIN, LOW);
  if (radio.available()) {
    //radio.read(&data, sizeof(Data_Package));
    
    char text[20] = "";
    radio.read(&text, sizeof(text));
    //Serial.println(text);
    lastRecvTime = millis(); //here we receive the data
    //parse payload
    int i =0;
    int j = 0;
    String values[5];
    //Start char
    if(text[0] != '['){
      Serial.println("Invalid payload");
    }
    for(i=1; i< 20; i++){
        if(text[i]=='#'){
          j = j +1;
          continue;
        }
        else if(text[i]==']'){
         i=20;
        }
        else
        {
          values[j] += text[i];
        }       
    }
    //Serial.println(values[0] + " " + values[1] + " " + values[2] + " " + values[3] + " " + values[4]);
    data.throttle = values[0].toInt();
    data.yaw = values[1].toInt();
    data.pitch = values[2].toInt();
    data.roll = values[3].toInt();
    data.arm = values[4].toInt();
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
    Serial.println("Signal lost...");
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

   if(data.arm==1){
       analogWrite(pwm,data.throttle);
       analogWrite(6,data.throttle);
       analogWrite(5,data.throttle);
       analogWrite(9,data.throttle);
   }
   //delay(50);
}

/**************************************************/
