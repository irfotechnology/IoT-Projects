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
unsigned long waitAfterturn = 0;
unsigned long waitAfterRun = 0;

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

void runForward() {
  waitAfterRun = millis();
  digitalWrite(upLED, HIGH);
  digitalWrite(downLED, HIGH);
  delay(200);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, LOW);
}

void runBackward() {
  waitAfterRun = millis();
  digitalWrite(carReversePin, HIGH);
  delay(200);
  digitalWrite(carReversePin, LOW);
}

void resestAngle() {
  Serial.print("Angle ");
  Serial.println(angle);
  Serial.print("leftChanged ");
  Serial.println(leftChanged);
  Serial.print("rightChanged ");
  Serial.println(rightChanged);
  Serial.print("waitAfterturn ");
  Serial.println(waitAfterturn);
  if (angle != 90 && millis() - waitAfterturn > 2000) {
    angle = 90;
    servo1.write(angle);
  }
}

void loop()
{

  //logic for turning of car based on L/R IR sensor readings
  if (digitalRead(leftSensor) == 0 && leftChanged == false ) {
    leftChanged = true;
    digitalWrite(leftLED, HIGH);
    waitAfterturn = millis();
    if (angle > 45) {
      angle = angle - 5;
      servo1.write(angle);
    }
    else {
      //run left motor here
      //
      digitalWrite(upLED, HIGH);
      delay(150);
      digitalWrite(upLED, LOW);
    }
  }
  else
  {
    resestAngle();
    digitalWrite(leftLED, LOW);
    leftChanged = false;
  }

  if (digitalRead(rightSensor) == 0 && rightChanged == false) {
    rightChanged = true;
    digitalWrite(rightLED, HIGH);
    waitAfterturn = millis();
    if (angle < 135) {
      angle = angle + 5;
      servo1.write(angle);
    }
    else {
      //run right motor here
      //
      digitalWrite(downLED, HIGH);
      delay(150);
      digitalWrite(downLED, LOW);
    }
  }
  else
  {
    resestAngle();
    digitalWrite(rightLED, LOW);
    rightChanged = false;
  }


  if ( leftChanged == false && rightChanged == false & angle == 90) {

    //logic for driving car f/b based on ultrasonic distance sensor readings
    cm = 0.01723 * readUltrasonicDistance(7, 7);  // convert to inches by dividing by 2.54
    inches = (cm / 2.54);
    if (inches < 37) {
      Serial.println(inches);
      if (millis() - waitAfterRun > 500) {
        if (inches > 15) {
          runForward();
          Serial.println("runForward()");
        }
        else if (inches < 15 && inches > 6) {
          runBackward();
          Serial.println("runBackward()");
        }
      }
    }
  }

  delay(50); // Wait for 200 millisecond(s)
}
