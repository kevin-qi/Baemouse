int leftEncoderPinA = 7;
int leftEncoderPinB = 8;
int rightEncoderPinA = 2;
int rightEncoderPinB = 4;
int leftSensor = 9;
int rightSensor = 10;
int frontSensor = 11;
int leftMotorPinA = 6
int leftMotorPinB = 9;
int rightMotorPinA = 3
int rightMotorPinB = 5;

volatile int leftTicks = 0;
volatile int rightTicks = 0;

void setup() { 
  pinMode(leftEncoderPinA, INPUT); 
  pinMode(leftEncoderPinB, INPUT); 
  pinMode(rightEncoderPinA, INPUT);
  pinMode(rightEncoderPinB, INPUT);
  attachInterrupt(leftEncoderPin, onLeftTick, CHANGE);
  attachInterrupt(rightEncoderPin, onRightTick, CHANGE);
  
  
}

void loop() { 

  int prev_error = 0;
  int error = leftTicks - rightTicks;    
  int delta_error = error - prev_error;
  float P = -1;
  float D = -100;
  float pidOutput = P * error + D * delta_error;
  prev_error = error;
  driveLeft(0.75 + 0.25 * pidOutput);
  driveRight(0.75 - 0.25 * pidOutput);
}

void onLeftTick() { 
  leftTicks++;
}

void onRightTick() { 
  rightTicks++;
}

void driveLeft(float spd) { 
  analogWrite(leftMotorPinA, spd * 255);
  analogWrite(leftMotorPinB, 0);
}

void driveRight(float spd) { 
  analogWrite(rightMotorPinA, spd * 255);
  analogWrite(rightMotorPinB, 0);
}
