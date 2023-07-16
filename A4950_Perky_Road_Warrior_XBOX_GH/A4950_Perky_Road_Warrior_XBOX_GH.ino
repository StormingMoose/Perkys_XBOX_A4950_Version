 /*
   Adapted from the following:
   
   ESP32 Web Server Demo using Accesspoint
   https://circuits4you.com
   21-11-2018

   Many thanks to Rui Santos and his crew over at randomnerdturtorials.com
*/

 

#include <Servo.h>   //  servo stuff
Servo myservo;  // create servo object to control steering
Servo myservoV;  //head position
Servo myservoH;   //head position

// GPIO the servo is attached to
static const int servoPin = 15;      
static const int servoPinH = 18; 
static const int servoPinV= 19; 

int pos = 93; // intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment
int posV = 135;        // starting head position for servo 
int posH = 95;          // starting head position for servo 

// wired connections
int MOTOR_PWM_A = 33;//  Speed  
int MOTOR_DIR_A = 32; // 

int MOTOR_PWM_B = 26 ;//  Speed  Enable Pin
int MOTOR_DIR_B = 25; //  Direction


// the actual values for "fast" and "slow" depend on the motor
#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0
double PWM_SPEED = 0; // INTIAL PWM duty cycle


const int freq = 909;  // 1.1ms per bit
const int resolution = 8;
const int ledChannel_PWM_A = 10;
const int ledChannel_Dir_A = 11;
const int ledChannel_PWM_B = 12;
const int ledChannel_Dir_B = 13;

#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//Enter your desired WiFi SSID and PASSWORD
const char* ssid = "your wifi";
const char* password = "*******";  // change the *'s to your favourite vegetable

WebServer server(80);

//===============================================================
// Setup
//===============================================================
void setup(void) {

  // Serial.begin(115200);
 
  pinMode(servoPin, OUTPUT);
  pinMode(servoPinV, OUTPUT);
  pinMode(servoPinH, OUTPUT);

  myservo.attach(servoPin); 
  myservoV.attach(servoPinV);
  myservoH.attach(servoPinH);

  myservo.write(pos);
  myservoV.write(posV);
  myservoH.write(posH);

  pinMode(MOTOR_PWM_A, OUTPUT);
  pinMode(MOTOR_DIR_A, OUTPUT);
  pinMode(MOTOR_PWM_B, OUTPUT);
  pinMode(MOTOR_DIR_B, OUTPUT);


  ledcSetup(ledChannel_PWM_A, freq, resolution);
  ledcSetup(ledChannel_Dir_A, freq, resolution);
  ledcSetup(ledChannel_PWM_B, freq, resolution);
  ledcSetup(ledChannel_Dir_B, freq, resolution);
 
  ledcAttachPin(MOTOR_PWM_A, ledChannel_PWM_A);
  ledcAttachPin(MOTOR_DIR_A, ledChannel_Dir_A);
  ledcAttachPin(MOTOR_PWM_B, ledChannel_PWM_B);
  ledcAttachPin(MOTOR_DIR_B, ledChannel_Dir_B);


  ledcWrite( ledChannel_Dir_A, dir_Forward);
  ledcWrite( ledChannel_Dir_B, dir_Forward);
  ledcWrite( ledChannel_PWM_A, PWM_STOP );
  ledcWrite( ledChannel_PWM_B, PWM_STOP );

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);    //Password length minimum 8 char


  server.on("/", handle_OnConnect);
  server.on("/SPEED", handle_Set_Speed);
  server.on("/A", handle_Forward);
  server.on("/B", handle_Back);
  

  server.on("/off", handle_Stop);
 // server.on("/R", handle_Go_Righter);
 // server.on("/L", handle_Go_Lefter);
  server.on("/STEER", handle_Steer);


  server.on("/HR", handle_Head_Righter);
  server.on("/HL", handle_Head_Lefter);
  server.on("/Up", head_Go_Higher);
  server.on("/Dn", head_Go_Lower);
  server.on("/HC", head_Center);
  server.on("/CS", Center_Steering);


  server.on("/FO", handle_Head_light_Off);
  server.on("/Flon", handle_Head_light_On);
 
  server.onNotFound(handle_NotFound);


  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void) {
  server.handleClient();
}
void handle_NotFound () {
}


void handle_OnConnect() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page

}

void handle_Set_Speed() {
  if(server.arg("value") ){PWM_SPEED = server.arg("value").toInt(); }
  server.send(200, "text/plain", "ok");
}

void handle_Forward() {
  ledcWrite( ledChannel_Dir_A, dir_Forward ); // direction = forward
  ledcWrite( ledChannel_PWM_A, PWM_SPEED ); // PWM speed = fast
  ledcWrite( ledChannel_Dir_B, dir_Forward ); // direction = forward
  ledcWrite( ledChannel_PWM_B, PWM_SPEED); // PWM speed = fast
  
  server.send(200, "text/plain", "ok");
}


void handle_Back() {

 ledcWrite( ledChannel_Dir_A, dir_Back ); // direction = forward
  ledcWrite( ledChannel_PWM_A, 255-PWM_SPEED );   // see note in the handle_Stop routine below.
  ledcWrite( ledChannel_Dir_B, dir_Back ); 
  ledcWrite( ledChannel_PWM_B, 255-PWM_SPEED); 
 
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
  ledcWrite( ledChannel_Dir_A, dir_Forward );    // odd thing about these drivers, the stop function means stop when going forward, if in reverse it means go as fast as you can.
  ledcWrite( ledChannel_PWM_A, PWM_STOP );       // Make sure your direction is forward when you want to stop.
  ledcWrite( ledChannel_Dir_B, dir_Forward );
  ledcWrite( ledChannel_PWM_B, PWM_STOP );
  
  server.send(200, "text/plain", "ok");
}


void handle_Steer() { // Receive an HTTP GET request for steering.  Depending on the position of sg90 servo could change name to Lefter
  if(server.arg("value") ){pos = server.arg("value").toInt(); }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}


void handle_Head_Righter() { // Receive an HTTP GET request for steering
  posH = posH - 10;
  if (posH < 0) {
    posH = 0;
  }
 /* myservoH.write(posH+7);
  myservoH.write(posH+4);
  myservoH.write(posH+3);
  myservoH.write(posH+2);
  myservoH.write(posH+1);
  */
  myservoH.write(posH);
  server.send(200, "text/plain", "ok");
}


void handle_Head_Lefter() { // Receive an HTTP GET request for steering
  posH = posH + 10;
  if (posH > 180) {
    posH = 180;
  }
  myservoH.write(posH);
  server.send(200, "text/plain", "ok");
}


void head_Go_Higher() { // Receive an HTTP GET request for steering
 // digitalWrite(Flight, HIGH);
  posV = posV - 15;
  if (posV < 45) {
    posV = 45;
  }
  myservoV.write(posV);
  server.send(200, "text/plain", "ok");
}

void head_Go_Lower() { // Receive an HTTP GET request for steering
 //digitalWrite(Flight, HIGH);
  posV = posV + 15;
  if (posV > 165) {
    posV = 165;
  }
  myservoV.write(posV);
  server.send(200, "text/plain", "ok");
}

void head_Center() { // Receive an HTTP GET request for steering
 //digitalWrite(Flight, HIGH);
  posH = 95;
  myservoH.write(posH);
  posV = 135;
  myservoV.write(posV);
  
  server.send(200, "text/plain", "ok");
}

void Center_Steering() { // Receive an HTTP GET request for steering
 //digitalWrite(Flight, HIGH);
  pos = 93;
  myservo.write(pos);
  
  server.send(200, "text/plain", "ok");
}



void handle_Head_light_On() {
/*  chain1.setLEDColor(7, 7, 7, 0);
  int j=0;
  while(j<10){
  chain1.communicate();
  j++;
  }
  server.send(200, "text/plain", "ok");
*/
}

void handle_Head_light_Off() {
  /*chain1.setLEDColor(0, 0, 0, 0);
  int i=0;
  while(i<10){
    chain1.communicate();
  i++;
  }
  server.send(200, "text/plain", "ok");
*/
}

