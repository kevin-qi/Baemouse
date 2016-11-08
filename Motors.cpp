
#include "Arduino.h"
#include "Motors.h"

Motors::Motors(int m_leftMotorPin1, int m_leftMotorPin2, int m_rightMotorPin1, int m_rightMotorPin2, 
      int m_leftInterrupt, int m_leftEncoderPin, int m_rightInterrupt, int m_rightEncoderPin)
{
  Serial.begin(9600);
  
  leftMotorPin1 = m_leftMotorPin1;
  leftMotorPin2 = m_leftMotorPin2;
  rightMotorPin1 = m_rightMotorPin1;
  rightMotorPin2 = m_rightMotorPin2;
  
  leftInterrupt = m_leftInterrupt;
  leftEncoderPin = m_leftEncoderPin;
  rightInterrupt = m_rightInterrupt;
  rightEncoderPin = m_rightEncoderPin;

  leftTicks = 0;
  rightTicks = 0;
  prev_error = 0;
  prev = 0;
  error = leftTicks - rightTicks;
  delta_error = error - prev_error;
  P = -10;
  D = -100;
  scale = 500;
  pidOutput = 0;

  //Speed variables (Units: pulses/microsecond)
  leftSpeed=0; 
  rightSpeed=0;
  
  timer_sampling_interval = 50; //sampling time in microseconds

  timer = 0;
  current_time = 0;
  
  targetSpeed=50;

  
  Serial.print("hello world");
  
}

void Motors::onLeftInterrupt() {
  //We know that pin1 just switched to LOW
  if(digitalRead(leftEncoderPin)==0){ //Check if pin2 is at low as well
    leftTicks--; //If so, then it is moving forward
  }
  else{
    leftTicks++; //Otherwise, it is moving backwards
  }
}

void Motors::onRightInterrupt() {
  //We know that pin1 just switched to LOW
  if(digitalRead(rightEncoderPin)==0){ //Check if pin2 is at low as well
    rightTicks--; //If so, then it is moving forward
  }
  else{
    rightTicks++; //Otherwise, it is moving backwards
  }
}

void Motors::driveLeft(float spd) {
  int value = spd * 200;
  if (value > 255)
    value = 255;
  if (value < 0)
    value = 0;
  analogWrite(leftMotorPin2, value);
}

void Motors::driveRight(float spd) {
  int value = spd * 200;
  if (value > 255)
    value = 255;
  if (value < 0)
    value = 0;
  analogWrite(rightMotorPin2, value);
}

float Motors::pidControl(float P,float D,float error,float prev_error){
  int delta_error = error - prev_error;
  return P * error + D * delta_error;
}

void Motors::m_run() {
  error = leftTicks-rightTicks;
  Serial.println(pidOutput);
  pidOutput=pidControl(P,D,error,prev_error)/scale;
  driveLeft(0.25+pidOutput);
  driveRight(0.25-pidOutput);
  prev_error = error;
}








