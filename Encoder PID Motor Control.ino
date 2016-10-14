int leftEncoderPin=2;
int rightEncoderPin=3;

int leftMotorPin=6;
int rightMotorPin=9;

volatile int leftTicks=0;
volatile int rightTicks=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(leftEncoderPin, INPUT);
  pinMode(rightEncoderPin, INPUT);
  attachInterrupt(leftEncoderPin, onLeftTick, CHANGE);
  attachInterrupt(rightEncoderPin, onRightTick, CHANGE);
}

void onLeftTick() {
  leftTicks++;
}

void onRightTick() {
  rightTicks++;
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
  int error = leftTicks - rightTicks;
  int delta_error = error - prev_error;
  float P = -1;
  float D = -100;
  float pidOutput = P * error + D * error;
  prev_error = error;
  driveLeft(0.75 + 0.25*pidOutput);
  driveRight(0.75 - 0.25 * pidOutput);
}
