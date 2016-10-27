/*
 * Notes for circuit building:
 * - Left is white wheel, Right is black wheel
 * - LEFT/RIGHT Encoder Pins: A to D2/D3, B to D4/D7 
 * - LEFT/RIGHT Motor Pins: M1 to 20f/30e, M2 to 19f/29e ~~~ This is opposite (relative to h-bridge) b/c the wheels are mirrored
 * - LEFT/RIGHT Motor Control Pins: D5/D9 to 22f/27e, D6/D10 to 27f/22e
 * - Make sure Encoder power pin is connected to 5v and not the battery!!!
 * - Make sure to ground the arduino with the battery!!! 
 */

int leftInterrupt=0; //Actualy pin 2
int leftEncoderPin=4;
int rightInterrupt=1; //Actually pin 3
int rightEncoderPin=7;

int leftMotorPin1=5; 
int leftMotorPin2=6;
int rightMotorPin1=9; 
int rightMotorPin2=10;

volatile int leftTicks=0;
volatile int rightTicks=0;

void setup() {
  //Begin Serial Port
  Serial.begin(9600);

  //Initialize pins
  pinMode(leftInterrupt, INPUT);
  pinMode(leftEncoderPin, INPUT);
  pinMode(rightInterrupt, INPUT);
  pinMode(rightEncoderPin, INPUT);

  //Attach Interrupts
  attachInterrupt(leftInterrupt, onLeftInterrupt, RISING);
  attachInterrupt(rightInterrupt, onRightInterrupt, FALLING); //Falling the wheels are mirrored on the vehicle
}

void onLeftInterrupt() {
  //We know that pin1 just switched to LOW
  if(digitalRead(leftEncoderPin)==0){ //Check if pin2 is at low as well
    leftTicks++; //If so, then it is moving forward
  }
  else{
    leftTicks--; //Otherwise, it is moving backwards
  }
}

void onRightInterrupt() {
  //We know that pin1 just switched to LOW
  if(digitalRead(rightEncoderPin)==0){ //Check if pin2 is at low as well
    rightTicks++; //If so, then it is moving forward
  }
  else{
    rightTicks--; //Otherwise, it is moving backwards
  }
}

void driveLeft(float spd){
  analogWrite(leftMotorPin1, spd*255);
}

void driveRight(float spd){
  analogWrite(rightMotorPin1, spd*255);
}

int prev_error = 0;
int prev=0;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(prev);
  prev = rightTicks-leftTicks;
  int error = leftTicks - rightTicks;
  int delta_error = error - prev_error;
  float P = -2;
  float D = -100;
  float pidOutput = P * error + D * error;
  prev_error = error;
  driveLeft(0.75 + 0.25*pidOutput);
  driveRight(0.75 - 0.25 * pidOutput);
}
