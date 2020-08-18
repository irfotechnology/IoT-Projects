#include <Servo.h>
//IR Sensor Pins for directions
const int leftSensor = 2;
const int rightSensor = 3;
const int downSensor = 4;
const int upSensor = 5;

//LED Pins for directions
const int rightLED = 8;
const int leftLED = 9;
const int upLED = 10;
const int downLED = 11;

//Servor Motors Pins
const int servo1Pin = 12;
const int carReversePin = 4;

int angle = 90;
Servo servo1;
bool leftChanged;
bool rightChanged;

int inches = 0;

int cm = 0;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


void setup()
{
  pinMode(rightLED, OUTPUT);
  pinMode(leftLED, OUTPUT);
  pinMode(upLED, OUTPUT);
  pinMode(downLED, OUTPUT);
  pinMode(carReversePin, OUTPUT);
  
  pinMode(rightSensor, INPUT);
  pinMode(leftSensor, INPUT);
  pinMode(upSensor, INPUT);
  pinMode(downSensor, INPUT);
  
  servo1.attach(servo1Pin);
  servo1.write(angle);
  Serial.begin(9600);
}

void runForward(){
  digitalWrite(upLED, HIGH);
  digitalWrite(downLED, HIGH);
  delay(500);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, LOW);
}

void runBackward(){
  digitalWrite(carReversePin, HIGH);
  delay(500);
  digitalWrite(carReversePin, LOW);
}

void loop()
{
  cm = 0.01723 * readUltrasonicDistance(7,7);   // convert to inches by dividing by 2.54
    inches = (cm / 2.54);
  if(inches < 25){
    Serial.println(inches);
    if(inches > 12){
		runForward();
      Serial.println("runForward()");
    }
    else
    {
      	runBackward();
      	Serial.println("runBackward()");
    }
  }
  
  
  if(digitalRead(leftSensor) >0 && leftChanged == false ){
    leftChanged = true;
    digitalWrite(leftLED, HIGH);
    
    if(angle > 45){
    	angle = angle - 5;
        servo1.write(angle);
    }
    else{
     	//run left motor here 
      	//
        digitalWrite(upLED, HIGH);
        delay(100);
        digitalWrite(upLED, LOW);
    }
  }
  else
  {
  	digitalWrite(leftLED, LOW);
    leftChanged = false;
  }
  
  if(digitalRead(rightSensor) >0 && rightChanged == false){
    rightChanged = true;
    digitalWrite(rightLED, HIGH);
    digitalWrite(rightLED, LOW);

    if(angle < 135){
    	angle = angle + 5;
        servo1.write(angle);
    }
    else{
     	//run right motor here 
        //
        digitalWrite(downLED, HIGH);
        delay(100);
        digitalWrite(downLED, LOW);
    }
  }
  else
  {
	digitalWrite(rightLED, LOW);
    rightChanged = false;
  }
  
  delay(500); // Wait for 200 millisecond(s)
}