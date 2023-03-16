#define INP1 9
#define INP2 10
#define INP3 11
#define INP4 12 //outputs to l298n drivers

#define LeftOutput 3 //D12 esp to D3 arduino
#define RightOutput 4 //D13 esp to D4 arduino
#define ReverseOutput 2 //D14 esp to D2 arduino

/* #define ESPIN1 2
   #define ESPIN2 3 */

#define ENA 5 //analog enable pin one for voltage control
#define ENB 6 //analog enable pin two

void setup() {
  // put your setup code here, to run once:
  pinMode(INP1,OUTPUT);
  pinMode(INP2,OUTPUT);
  pinMode(INP3,OUTPUT);
  pinMode(INP4,OUTPUT);

//  pinMode(ESPIN1, INPUT);
//  pinMode(ESPIN2, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(LeftOutput, INPUT);
  pinMode(RightOutput, INPUT);
  pinMode(ReverseOutput, INPUT);

  //send analog half pwm signal to the l298n motor driver
  //essentially cut the power draw of motors by half
  analogWrite(ENA,255/2); //0-255 value variation
  analogWrite(ENB,255/2);
  //we're running it throughout the code irrespective of what function we are using so configured in void setup

  //set baudrate and serial config
  Serial.begin(9600);
}

/* L R B
 *  0 0 0 CANCEL
 *  1 1 0 FWD 
 *  1 0 0 LEFT
 *  0 1 0 RIGHT
 *  0 0 1 REVERSE
 *  1 1 1 SENTRY
 *  REMAINING 
 *  
 */
 
void loop() { 
  Serial.print("L: "); 
  Serial.print(digitalRead(LeftOutput)); //debug
  Serial.print("R: ");
  Serial.print(digitalRead(RightOutput)); //debug
  Serial.print("B: ");
  Serial.print(digitalRead(ReverseOutput)); //debug
  Serial.println("N");
//forward
if((digitalRead(LeftOutput)==HIGH && digitalRead(RightOutput)==HIGH) && digitalRead(ReverseOutput==LOW)){
  forward(); //should not need a delay timer
  Serial.println("Forward!");
}

//left
if((digitalRead(LeftOutput)==HIGH && digitalRead(RightOutput)==LOW) && digitalRead(ReverseOutput==LOW)){
  left(); //should not need a delay timer
  Serial.println("Left!");
}

//right
if((digitalRead(LeftOutput)==LOW && digitalRead(RightOutput)==HIGH) && digitalRead(ReverseOutput==LOW)){
  right(); //should not need a delay timer
  Serial.println("Right!");
}

//reverse
if((digitalRead(LeftOutput)==LOW && digitalRead(RightOutput)==LOW) && digitalRead(ReverseOutput==HIGH)){
  reverse(); //should not need a delay timer
  Serial.println("Reverse!");
}

//sentry mode
if((digitalRead(LeftOutput)==HIGH && digitalRead(RightOutput)==HIGH) && digitalRead(ReverseOutput==HIGH)){
  Serial.println("Sentry!");
  left();
  delay(1000);
  right();
  delay(1000);
  forward();
  delay(1000);
  reverse();
  delay(1000);
}

//STOP MOVING
if((digitalRead(LeftOutput)==LOW && digitalRead(RightOutput)==LOW) && digitalRead(ReverseOutput==LOW)){
  stopAll(); //should not need a delay timer
}
*/
}

void forward(){
  digitalWrite(INP1,HIGH); //FORWARD
  digitalWrite(INP2,LOW);
  
  digitalWrite(INP4,HIGH);//FORWARD
  digitalWrite(INP3,LOW);
  //delay(tms); //should work without delay
}

void reverse(){
  digitalWrite(INP1,LOW); //REVERSE
  digitalWrite(INP2,HIGH);
  
  digitalWrite(INP4,LOW); //REVERSE
  digitalWrite(INP3,HIGH);
  //delay(tms);
}

void right(){
  digitalWrite(INP1,LOW); //REVERSE
  digitalWrite(INP2,HIGH);

  digitalWrite(INP4,HIGH);//FORWARD
  digitalWrite(INP3,LOW);
  //delay(tms);
}

void left(){
  digitalWrite(INP1,HIGH); //FORWARD 0-255
  digitalWrite(INP2,LOW);

  digitalWrite(INP4,LOW); //REVERSE
  digitalWrite(INP3,HIGH);
 // delay(tms);
}

void stopAll(){
   digitalWrite(INP1,LOW); //STOP
  digitalWrite(INP2,LOW);

  digitalWrite(INP4,LOW); //STOP
  digitalWrite(INP3,LOW);
}
