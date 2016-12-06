/*
   Notes for circuit building:
   - Left is white wheel, Right is black wheel
   - LEFT/RIGHT Encoder Pins: A to D2/D3, B to D4/D7
   - LEFT/RIGHT Motor Pins: M1 to 26f/26e, M2 to 23f/23e ~~~ This is opposite (relative to h-bridge) b/c the wheels are mirrored
   - LEFT/RIGHT Motor Control Pins: D5/D9 to 22f/27e, D6/D10 to 27f/22e
   - Make sure Encoder power pin is connected to 5v and not the battery!!!
   - Make sure to ground the arduino with the battery!!!


   circumference = 12.57 cm
   1 revolution = 30 ticks
   12.57 cm  per 30 ticks implies that 100 ticks is 42cm. 43 ticks is 18 cm (about the distance of 1 cell).
*/

int leftInterrupt = 0; //Actualy pin 2
int leftEncoderPin = 4;
int rightInterrupt = 1; //Actually pin 3
int rightEncoderPin = 7;

int leftMotorPin1 = 5;
int leftMotorPin2 = 6;
int rightMotorPin1 = 9;
int rightMotorPin2 = 10;

volatile int leftTicks = 0;
volatile int rightTicks = 0;

volatile long timer_sampling_interval = 10; //sampling time in microseconds
volatile long time_out = 100;
double vel_sampling_interval = 2; //In number of ticks

volatile long timer = 0;
volatile long current_time = 0;



int targetSpeed = 50;

double pidOutput = 0;

void setup() {
  delay(3000);
  Serial.begin(9600);
  //Initialize pins
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
  if (digitalRead(leftEncoderPin) == 0) { //Check if pin2 is at low as well
    leftTicks--; //If so, then it is moving forward
  }
  else {
    leftTicks++; //Otherwise, it is moving backwards
  }
}


void onRightInterrupt() {
  //We know that pin1 just switched to LOW
  if (digitalRead(rightEncoderPin) == 0) { //Check if pin2 is at low as well
    rightTicks--; //If so, then it is moving forward
  }
  else {
    rightTicks++; //Otherwise, it is moving backwards
  }
}

void setSpeedLeft(float spd) {
  int value = spd * 255;
  if (value>0){
    analogWrite(leftMotorPin2, min(value,255));
  }
  else if (value < 0){
    analogWrite(leftMotorPin1, min(-1*value,255));
  }
  else{
    analogWrite(leftMotorPin1, min(value,0));
    analogWrite(leftMotorPin2, min(value,0));
  }
    
  
}

void setSpeedRight(float spd) {
  int value = spd * 255;
  if (value>0){
    analogWrite(rightMotorPin2, min(value,255));
  }
  else if (value < 0){
    analogWrite(rightMotorPin1, min(-1*value,255));
  }
  else{
    analogWrite(rightMotorPin1, min(value,0));
    analogWrite(rightMotorPin2, min(value,0));
  }
}

/*
  double prev_left_error = 0;
  double prev_right_error = 0;
  double left_error = 0;
  double right_error = 0;
  double delta_time = 0;
*/
double error = 0 ;
double prev_error = 0;
double P = -10;
double D = -100;
double scale = 500;

double pidControl(double P, double D, double error, double prev_error) {
  int delta_error = error - prev_error;
  return P * error + D * delta_error;
}

int temp = 0;
int targetDist = 50; //100 = 42.5 cm
int target_left = targetDist + leftTicks;
int target_right = targetDist + rightTicks;
int remaining = targetDist;

  double leftSpeed = 0;
  double rightSpeed = 0;
  int prevLeftTicks = leftTicks;
  int prevRightTicks= rightTicks;
  double prevLeftSpeed = leftSpeed;
  double prevRightSpeed = rightSpeed;
  double prevactualVelX = prevLeftSpeed+prevRightSpeed;
  double leftAcc = 0;
  double rightAcc = 0;
  double delta_time = 1;
  double Acc = 0;
  double Acc_sum = 0;
  double samples = 0;
  double dec = 50; //  ticks per second squared
  double velX [11];
  double velW [11];
  double idealVelX=0;
  double idealVelW=0;
  double velErrorX;
  double velErrorW;
  double posErrorX;
  double posErrorW;
  double oldPosErrorX;
  double errorX;
  double errorW;
  double accelX=0.1;
  double accelW = 0.1;
   
  double actualVelX;
  double actualVelW;
  int i = 0;

bool checkRemainingDistance(int ticks){
  return ((actualVelX*actualVelX)/(2*dec))<ticks;
}

//The next 4 functions are to simulate a ring buffer which idk how to implement otherwise.
void pushX(double actualVelX){
  for(int i=0;i++;i<10){
    velX[i] = velX[i+1];
  }
  velX[10] = actualVelX;
}

void pushW(double actualVelW){
  for(int i=0;i++;i<10){
    velW[i] = velW[i+1];
  }
  velW[10] = actualVelW;
}

double findValidX(){
  double x;
  for(int i=0;i++;i<10){
    if(velX[i]!=0){
      x = velX[i];
    }
  }
  return x;
}


double findValidW(){
  double x;
  for(int i=0;i++;i<10){
    if(velW[i]!=0){
      x = velW[i];
    }
  }
  return x;
}

void updateState(){
   //Calculate actualVelX & actualVelW
   //Serial.println(delta_time);
     if(delta_time > timer_sampling_interval){
        leftSpeed = ((double)(leftTicks)-double(prevLeftTicks))*1000/delta_time;
        rightSpeed = ((double)(rightTicks)-double(prevRightTicks))*1000/delta_time;
        actualVelX = (leftSpeed+rightSpeed)/2;
        actualVelW = (leftSpeed - rightSpeed);
        if(actualVelX == 0){
          pushX(actualVelX);
          actualVelX = findValidX();
        }
        
      Serial.println(actualVelX);

      prevLeftTicks=leftTicks;
      prevRightTicks=rightTicks;      
      timer = current_time;
      
   }
}


double Kp=-0.002;
  double Kd=-0.1;
void setPWM(double targetVelX, double targetVelW){
  if(idealVelX<targetVelX){
    idealVelX+=accelX;
  }
  else if(idealVelX > targetVelX){
    idealVelX-=accelX;
  }
  else{
    idealVelX=idealVelX;
  }
  
  if(idealVelW<targetVelW){
    idealVelW+=accelW;
  }
  else if(idealVelW > targetVelW){
    idealVelW-=accelW;
  }
  else{
    idealVelW=idealVelW;
  }
  velErrorX = idealVelX - actualVelX;
  velErrorW = idealVelW - actualVelW;
  
  posErrorX = velErrorX * delta_time/1000;
  posErrorW = velErrorW * delta_time/1000;
  
  errorW = Kp*posErrorW+Kd*velErrorW;
  errorX = Kp*(posErrorX) + Kd*velErrorX;
  //Serial.print(errorW);
  //Serial.print("    ");
  //Serial.print(errorX);
  //Serial.println("");
  setSpeedLeft(errorX/10+errorW/50);
  setSpeedRight(errorX/10-errorW/50);
}


void loop() {
  current_time=millis(); 
  delta_time = current_time-timer;
  updateState();
  setPWM(25,0);
  //Serial.println(actualVelX);
  //setSpeedLeft(0.25);
  //setSpeedRight(0.25);
  
  /*current_time=millis(); 
    delta_time = current_time-timer;
 
  error = leftTicks - rightTicks;
  pidOutput = pidControl(P, D, error, prev_error) / scale;
  remaining = (target_left-leftTicks+target_right-rightTicks)/2;
  if(checkRemainingDistance(remaining)){
    //Serial.println(checkRemainingDistance(remaining));
    //Serial.println(remaining);
    setSpeedLeft(0.2 + pidOutput);
    setSpeedRight(0.2 - pidOutput);
  }
  else{
    setSpeedLeft(pidOutput);
    setSpeedRight(-pidOutput);
    //Serial.println(checkRemainingDistance(remaining));
    Serial.println(remaining);
  }
  /*Serial.print(remaining);
  Serial.print("     ");
  Serial.print((leftTicks+rightTicks)/2);
  Serial.println("");
  temp = rightTicks;
  prev_error = error;
  */

}
