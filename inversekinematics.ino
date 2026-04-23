#include <Servo.h>
#include <math.h>
Servo shoulderx;  // create Servo object to control a servo
Servo shouldery;
Servo elbow;
Servo rightfinger;
Servo leftfinger;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  shoulderx.attach(11);
  shouldery.attach(10);
  elbow.attach(9);
  rightfinger.attach(6);
  leftfinger.attach(5);
  pinMode(2, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  shoulderx.write(90);
  elbow.write(180 - 70);
  shouldery.write(160);
  leftfinger.write(174);
  rightfinger.write(0); 
}
//float positionx = 20;
//float positiony = 10;
//float positionXelbow = 180;
float x = 20;
float y = 0;
float rotate = 90;
int prev = 0;
int ready = 0;
// the loop routine runs over and over again forever:
void loop() {
  float horizontal = analogRead(A0);
  float vertical = analogRead(A1);
  float turn = analogRead(A2);
  float joint4 = analogRead(A3);

  //reading joysticks
  if (horizontal < 400) {  ///forward
    x--;
  }
  if (horizontal > 600) {  ///forward
    x++;
  }
  if (vertical < 400) {  ///up
    y++;
  }
  if (vertical > 600) {  ///down
    y--;
  }
  if(turn < 400){
    rotate--;
  }
  if(turn > 600){
    rotate++;
  }

  //range and domains of arms
  if (x > 200) {
    x = 200;
  }
  if (x < 10) {
    x = 10;
  }
  if (y > 200) {
    y = 200;
  }
  if (y < 0) {
    y = 0;
  }
  if (y < 20 && x < 50){
    y = 20;
    x = 50;
  }
  //base rotation range
  if (rotate < 0){
    rotate = 0;
  }
  if (rotate > 180){
    rotate = 180;
  }

  //inverse kinematic equations
  double c = pow(x, 2) + pow(y, 2);
  double hlength = sqrt(c);
  float radians = atan2(y, x);
  float baseangle = radians * (180 / 3.1415);
  float radians2 = acos((hlength / 2) / 150);
  float baseangle2 = radians2 * (180 / 3.1415);
  float baseanglesum = baseangle + baseangle2;
  float elbowangle = 180 - (baseangle2 * 2);
  
  //moving the servos
  shouldery.write(baseanglesum);
  elbow.write(180 - elbowangle);
  shoulderx.write(rotate);
  delay(15);
  
  //claw
  int obs = digitalRead(13);
  int open = digitalRead(2);
  if(obs == 0){ //notify that there is object
    ready = 1;
    //x =+ 5;
   //y =- 2;
  if(ready == 1){ //closes claw
    delay(100);
    leftfinger.write(174);
    rightfinger.write(0); 
    ready = 0;
    delay(100);
    //x =- 5;
    //y =+ 2;
    }
  }
  if(open == 0){ //opens claw
    leftfinger.write(130);
    rightfinger.write(50);  
  } 
  Serial.println((x));
  Serial.println((y));
}