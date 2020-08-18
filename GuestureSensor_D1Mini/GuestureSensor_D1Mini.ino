/*
 * Connect proximity sensor (IR) to pin no D2 and D3
 * of Wemos D1 R1
 * detect values changes of sensors from right to left or left to right
 * and display the message
 */
int gesture[2];
int idx;
bool leftChanged;
bool rightChanged;
//bool bothChanged;
   
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(D2, INPUT);
    pinMode(D8, INPUT);
    idx=0;
}

void loop() {
  delay(20);
  // put your main code here, to run repeatedly:
   int i = digitalRead(D2);
   int j = digitalRead(D8);
   //Serial.print(i);
   //Serial.print(" ");
   //Serial.print(j);
   //Serial.println(" ");
   //Serial.print("Idx ");
   //Serial.println(idx);


   //if (i == LOW && j == LOW && bothChanged == false){
      //bothChanged = true;
      //gesture[idx] = 0;
      //idx = idx + 1;
      //Serial.println("both changed");
  // }
   
   if (i == LOW && j == HIGH && leftChanged == false){
      leftChanged = true;
      gesture[idx] = 1;
      idx = idx + 1;
      //Serial.println("left changed");
   }

   if (j == LOW && i == HIGH && rightChanged == false){ 
      rightChanged = true;
      gesture[idx] = 2;
      idx = idx + 1;
      //Serial.println("right changed");
   }

  //Serial.print(gesture[0]);
  //Serial.print(" ");
   //Serial.print(gesture[1]);
  //Serial.println(" ");
  
   if(idx == 2)
   {
         if(gesture[0] == 1 && gesture[1] == 2){
               Serial.println("LTR");
         }
         if(gesture[0] == 2 && gesture[1] == 1){
               Serial.println("RTL");
         }
         idx = 0;
         gesture[0] == 0;
         gesture[1] == 0;
         leftChanged = false;
         rightChanged = false;
         //bothChanged = false;
   }
}
