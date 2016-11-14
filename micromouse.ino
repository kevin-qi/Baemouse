
#include "Arduino.h"
#include "Motors.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Motors motor = new Motors(5, 6, 9, 10, 0, 4, 1, 7);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("hello world1");
  motor.m_run();
  Serial.print("hello world2");
}




