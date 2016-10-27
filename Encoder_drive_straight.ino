int leftInterrupt=0; //Actualy pin 2
int leftEncoderPin=4;
int rightInterrupt=1; //Actually pin 3
int rightEncoderPin=7;

int leftMotorPin=6;
int rightMotorPin=9;

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
  attachInterrupt(leftInterrupt, onLeftInterrupt, FALLING);
  attachInterrupt(rightInterrupt, onRightInterrupt, FALLING); 
}

void onLeftInterrupt() {
  //We know that pin1 just switched to LOW
  if(leftEncoderPin==0){ //Check if pin2 is at low as well
    leftTicks++; //If so, then it is moving forward
  }
  else{
    leftTicks--; //Otherwise, it is moving backwards
  }
}

void onRightInterrupt() {
  //We know that pin1 just switched to LOW
  if(rightEncoderPin==0){ //Check if pin2 is at low as well
    rightTicks++; //If so, then it is moving forward
  }
  else{
    rightTicks--; //Otherwise, it is moving backwards
  }
}

void driveLeft(float spd){
  analogWrite(leftMotorPin, spd*255);
}

void driveRight(float spd){
  analogWrite(rightMotorPin, spd*255);
}

int prev_error = 0;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(rightTicks-leftTicks);
  int error = leftTicks - rightTicks;
  int delta_error = error - prev_error;
  float P = -1;
  float D = -100;
  float pidOutput = P * error + D * error;
  prev_error = error;
  driveLeft(0.75 + 0.25*pidOutput);
  driveRight(0.75 - 0.25 * pidOutput);
}
