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

int echoPin = 7;
int trigPin = 8;

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

volatile int leftTicks=0;
volatile int rightTicks=0;

//Speed variables (Units: pulses/microsecond)
volatile int leftSpeed=0; 
volatile int rightSpeed=0;

volatile long timer_sampling_interval = 50; //sampling time in microseconds

volatile long timer = 0;
volatile long current_time = 0;

int targetSpeed=50;

float pidOutput=0;

void setup() {
  delay(3000);
  Serial.begin(9600);
  //Initialize pins

 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
  
  pinMode(leftInterrupt, INPUT);
  pinMode(leftEncoderPin, INPUT);
  
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  pinMode(rightInterrupt, INPUT);
  pinMode(rightEncoderPin, INPUT);
  
  //Attach Interrupts
  attachInterrupt(leftInterrupt, onLeftInterrupt, RISING);
  attachInterrupt(rightInterrupt, onRightInterrupt, FALLING); //Falling the wheels are mirrored on the vehicle

  //Start Timer
  timer = millis();
}

void onLeftInterrupt() {
  //We know that pin1 just switched to LOW
  if(digitalRead(leftEncoderPin)==0){ //Check if pin2 is at low as well
    leftTicks--; //If so, then it is moving forward
  }
  else{
    leftTicks++; //Otherwise, it is moving backwards
  }
}

void onRightInterrupt() {
  //We know that pin1 just switched to LOW
  if(digitalRead(rightEncoderPin)==0){ //Check if pin2 is at low as well
    rightTicks--; //If so, then it is moving forward
  }
  else{
    rightTicks++; //Otherwise, it is moving backwards
  }
}

void driveLeft(float spd){
  int value = spd * 200;
  if (value > 0) {
    analogWrite(leftMotorPin2, min(value, 255));
  }
  else if (value < 0) {
    analogWrite(leftMotorPin1, min(-1*value, 255));
  }
  else {
    analogWrite(leftMotorPin1, min(value, 0));
    analogWrite(leftMotorPin2, min(value, 0));
  }
}

void driveRight(float spd){
  int value = spd * 200;
  if (value > 0) {
    analogWrite(rightMotorPin2, min(value, 255));
  }
  else if (value < 0) {
    analogWrite(rightMotorPin1, min(-1*value, 255));
  } else {
    analogWrite(rightMotorPin1, min(-1*value, 255));
    analogWrite(rightMotorPin2, min(-1*value, 255));
}
}

/*
float prev_left_error = 0;
float prev_right_error = 0;
float left_error = 0;
float right_error = 0;
float delta_time = 0;
*/
float error = 0 ;
float prev_error = 0;
float P = -10;
float D = -100;
float scale = 500;

float pidControl(float P,float D,float error,float prev_error){
  int delta_error = error - prev_error;
  return P * error + D * delta_error;
}

void loop() {
  digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
  
    error = leftTicks-rightTicks;
    Serial.println(pidOutput);
    pidOutput=pidControl(P,D,error,prev_error)/scale;
    driveLeft(0.25+pidOutput);
    driveRight(0.25-pidOutput);
    prev_error = error;

    if (distance < 30) {
 delay(50);
      driveLeft(1);
      driveRight(-1);
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2); 

      digitalWrite(trigPin, HIGH);
       delayMicroseconds(10); 
 
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
      distance = duration/58.2;
      if (distance < 30) {
        driveLeft(2);
        driveRight(-2);
      }
    }
    
 delay(50);
    /*
    current_time=micros();
    delta_time = current_time-timer;
    if(delta_time>timer_sampling_interval){
      leftSpeed = leftTicks/delta_time;
      rightSpeed = rightTicks/delta_time;
      leftTicks=0;
      rightTicks=0;
      timer = current_time;
    }
    
    left_error = leftSpeed - targetSpeed;
    right_error = rightSpeed - targetSpeed; 
    
    float leftPID = 0;
    float rightPID = 0;
    float P = -0.2;
    float D = -0.9;
    
    leftPID = pidControl(P,D,left_error,prev_left_error);
    rightPID = pidControl(P,D,right_error,prev_right_error);
    prev_left_error = left_error;
    prev_right_error = right_error;
    
    driveLeft(0.25 + leftPID);
    driveRight(0.25 + rightPID); */
}
