#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

bool fetch_blynk_state = true;  //true or false

//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define LeftOutput 12 //D12
#define RightOutput 13 //D13
#define ReverseOutput 14 //D14

#define wifiLed   27   //D27 WIFI STATUS CHECKER

#define VPIN_BUTTON_F    V1 
#define VPIN_BUTTON_B    V2
#define VPIN_BUTTON_L    V3 
#define VPIN_BUTTON_R    V4

#define VPIN_BUTTON_SENTRY    V5
#define VPIN_BUTTON_CANCEL    V0

// Switch states
bool toggleState_f = LOW; //Define integer to remember the toggle state for forward
bool toggleState_b = LOW; //Define integer to remember the toggle state for reverse
bool toggleState_l = LOW; //Define integer to remember the toggle state for left
bool toggleState_r = LOW; //Define integer to remember the toggle state for right
bool toggleState_sentry = LOW; //Define integer to remember the toggle state for SENTRY
bool toggleState_cancel = LOW; //Define integer to remember the toggle state for CANCEL

int wifiFlag = 0;

char auth[] = BLYNK_AUTH_TOKEN;

void forward(){
  digitalWrite(LeftOutput, HIGH);
  digitalWrite(RightOutput, HIGH);
  digitalWrite(ReverseOutput, LOW);  
}
void reverse(){
   digitalWrite(LeftOutput, LOW);
  digitalWrite(RightOutput, LOW);
  digitalWrite(ReverseOutput, HIGH);
}

void left(){
  digitalWrite(LeftOutput, HIGH); //counter 
  digitalWrite(RightOutput, LOW); //clock-wise
  digitalWrite(ReverseOutput, LOW);
}

void right(){
  digitalWrite(LeftOutput, LOW); //clock-wise
  digitalWrite(RightOutput, HIGH); //counter
  digitalWrite(ReverseOutput, LOW);
}

void sentry(){
  //do i wanna use delay here? nopeee
  //TRIPLE PIN HIGH CONFIG
  digitalWrite(LeftOutput, LOW);
  digitalWrite(RightOutput, LOW);
  digitalWrite(ReverseOutput, LOW);
}

void Cancel_All(){
  //all low-low conditions 
  digitalWrite(LeftOutput,LOW);
  digitalWrite(LeftOutput, LOW); //NOTE THAT BOTH LOW IS NOT REVERSE AND WE NEED TO USE THREE PIN INPUT CONFIGURATION TO CONTROL ANY MOVEMENT
  digitalWrite(ReverseOutput, LOW);
  
  toggleState_f = 0; Blynk.virtualWrite(VPIN_BUTTON_F, toggleState_f); delay(100);
  toggleState_b = 0; Blynk.virtualWrite(VPIN_BUTTON_B, toggleState_b); delay(100);
  toggleState_l = 0; Blynk.virtualWrite(VPIN_BUTTON_L, toggleState_l); delay(100);
  toggleState_r = 0; Blynk.virtualWrite(VPIN_BUTTON_R, toggleState_r); delay(100);
  toggleState_sentry = 0; Blynk.virtualWrite(VPIN_BUTTON_SENTRY, toggleState_sentry); delay(100);
  toggleState_cancel = 0; Blynk.virtualWrite(VPIN_BUTTON_CANCEL, toggleState_cancel); delay(100);
}

BlynkTimer timer;

// When App button is pushed - switch the state

//>>>>>>>>>>>>>>>>>>>>NEED TO CHECK THREE PIN CONDITIONS AT THE ARDUINO UNO SIDE NOW <<<<<<<<<<<<<<
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
BLYNK_WRITE(VPIN_BUTTON_F) {
  if (!toggleState_f){
  Cancel_All();
  Blynk.virtualWrite(VPIN_BUTTON_F, 1);
  }
  toggleState_f = param.asInt();
  Serial.println("Forward command received!");
  forward();
  
}

BLYNK_WRITE(VPIN_BUTTON_B) {
  if(!toggleState_b){
  Cancel_All();
  Blynk.virtualWrite(VPIN_BUTTON_B, 1);
  }
  toggleState_b = param.asInt();
  Serial.println("Reverse command received!");
  reverse();
}

BLYNK_WRITE(VPIN_BUTTON_L) {
  if(!toggleState_l){
    Cancel_All();
    Blynk.virtualWrite(VPIN_BUTTON_L, 1);
  }
  toggleState_l = param.asInt();
  Serial.println("Left command received!");
  left();  
}

BLYNK_WRITE(VPIN_BUTTON_R) {
  if(!toggleState_r){
  Cancel_All();
  Blynk.virtualWrite(VPIN_BUTTON_R, 1);
  }
  toggleState_r = param.asInt();
  Serial.println("Right command received!");
  right();
}

BLYNK_WRITE(VPIN_BUTTON_CANCEL) {
  toggleState_cancel = param.asInt();
  Serial.println("Cancel command received!");
  Cancel_All();
} 

//SENTRY MODE
BLYNK_WRITE(VPIN_BUTTON_SENTRY){
  if(!toggleState_sentry){
    Cancel_All();
    Blynk.virtualWrite(VPIN_BUTTON_SENTRY, 1);
  }
  toggleState_sentry = param.asInt();
  Serial.println("Sentry mode enabled!");
  sentry();
  toggleState_sentry=HIGH;
}

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_F, toggleState_f);
    Blynk.virtualWrite(VPIN_BUTTON_B, toggleState_b);
    Blynk.virtualWrite(VPIN_BUTTON_L, toggleState_l);
    Blynk.virtualWrite(VPIN_BUTTON_R, toggleState_r);
    Blynk.virtualWrite(VPIN_BUTTON_SENTRY,toggleState_sentry);
    Blynk.virtualWrite(VPIN_BUTTON_CANCEL,toggleState_cancel);
    }
    digitalWrite(wifiLed, HIGH);
    //Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_BUTTON_F);
    Blynk.syncVirtual(VPIN_BUTTON_B);
    Blynk.syncVirtual(VPIN_BUTTON_L);
    Blynk.syncVirtual(VPIN_BUTTON_R);
    Blynk.syncVirtual(VPIN_BUTTON_CANCEL);
    Blynk.syncVirtual(VPIN_BUTTON_SENTRY); //SENTRY MODE
  }
}


void setup()
{
  Serial.begin(9600);

  pinMode(LeftOutput, OUTPUT);
  pinMode(RightOutput, OUTPUT);
  pinMode(ReverseOutput, OUTPUT);

  pinMode(wifiLed, OUTPUT);
  
  digitalWrite(wifiLed, LOW);

  //Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
  
  if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_F, toggleState_f);
    Blynk.virtualWrite(VPIN_BUTTON_B, toggleState_b);
    Blynk.virtualWrite(VPIN_BUTTON_L, toggleState_l);
    Blynk.virtualWrite(VPIN_BUTTON_R, toggleState_r);
  }
}


void loop() {
  // main code:
  Blynk.run();
  timer.run();
}
