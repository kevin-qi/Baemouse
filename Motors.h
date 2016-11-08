/*
  Motors.h - Library for using two motors.
  Created by Baemax.
*/

#ifndef Motors_h
#define Motors_h

//#include "Arduino.h"

class Motors {
  private:
    volatile int leftTicks;
    volatile int rightTicks;
    int prev_error;
    int prev;
    int error;
    int delta_error;
    float P;
    float D;
    float scale;
    float pidOutput;

    //Speed variables (Units: pulses/microsecond)
    volatile int leftSpeed;
    volatile int rightSpeed;
    
    volatile long timer_sampling_interval; //sampling time in microseconds

    volatile long timer;
    volatile long current_time;
    
    int targetSpeed;
    
    
    int leftMotorPin1;
    int leftMotorPin2;
    int rightMotorPin1;
    int rightMotorPin2;
    int leftInterrupt;
    int leftEncoderPin;
    int rightInterrupt;
    int rightEncoderPin;

  
  public:
    Motors(int m_leftMotorPin1, int m_leftMotorPin2, int m_rightMotorPin1, int m_rightMotorPin2, 
      int m_leftInterrupt, int m_leftEncoderPin, int m_rightInterrupt, int m_rightEncoderPin);
      
    void onLeftInterrupt();
    
    void onRightInterrupt();
    
    void driveLeft(float spd);
    
    void driveRight(float spd);

    float pidControl(float P,float D,float error,float prev_error);

    void m_run();
};

#endif







