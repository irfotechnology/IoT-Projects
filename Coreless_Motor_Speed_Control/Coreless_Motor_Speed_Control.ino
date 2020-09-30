int pot = A0; //Input from the potentiometer pin A0
int pwm = 3;  //PWM output on pin D3

void setup() {
  pinMode(pot,INPUT);
  pinMode(pwm,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int value = map(analogRead(pot),0,1024,0,255);
  analogWrite(pwm,value);
  Serial.println(value);
  delay(10);  
}
