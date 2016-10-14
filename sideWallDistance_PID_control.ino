int leftSensorPin=2;
int rightSensorPin=3;

int leftMotorPin=6;
int rightMotorPin=9;

volatile int leftTicks=0;
volatile int rightTicks=0;

int left_dist = 0;
int right_dist = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
  attachInterrupt(leftSensorPin, onLeftTick, CHANGE);
  attachInterrupt(rightSensorPin, onRightTick, CHANGE);
}

//Update left wall distance
void onLeftTick() {
  left_dist=analogRead(leftSensorPin);
}

//Update right wall distance
void onRightTick() {
  left_dist=analogRead(leftSensorPin);
}

//Adjust left motor speed
void driveLeft(float spd){
  analogWrite(leftMotorPin, spd*255);
}

//Adjust right motor speed
void driveRight(float spd){
  analogWrite(rightMotorPin, spd*255);
}

//return pidOutput
float pidOutput(float error, float delta_error, float P, float D){
  return P * error + D * delta_error;
}

int prev_left_error = 0;
int prev_right_error = 0;

void loop() {
  //Set desired distance from side walls
  float set_distance = 10;
  
  // Find left and right deviations from set distance
  int left_error = left_dist - set_distance;
  int right_error = right_dist - set_distance;

  // Find derivative errors
  int left_delta_error = left_error - prev_left_error;
  int right_delta_error = right_error - prev_left_error;

  // Store previous error
  prev_left_error = left_error;
  prev_right_error = right_error;

  // Set P and D values
  float P = -1;
  float D = 100;

  // Calculate pidOutput
  float left_pidOutput = pidOutput(left_error,left_delta_error,P,D);
  float right_pidOutput = pidOutput(right_error,right_delta_error,P,D);

  // Adjust motors to account for error
  driveLeft(0.75 + 0.25*left_pidOutput);
  driveRight(0.75 + 0.25*right_pidOutput);
}
