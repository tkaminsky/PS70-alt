// define classes

#ifndef _STEPPER_H_
#define _STEPPER_H_
#include "Arduino.h"

class Stepper {

  int _stepsPerRevolutionRight;
  int _stepsPerRevolutionLeft;
  // define timer variables
  // set step and direction pin variables
  int _dirPin;
  int _stepPin;

  public:

    Stepper(int dirPin, int stepPin) {
      // set variables to pin inputs
      _dirPin = dirPin;
      _stepPin = stepPin;
      pinMode(_stepPin, OUTPUT);
      pinMode(_dirPin, OUTPUT);

    }

    // define stepper movement function
    void moveLeft(int stepsPerRevolutionLeft) {

      _stepsPerRevolutionLeft = stepsPerRevolutionLeft;

      // Set the spinning direction left, or counter-clockwise:
      digitalWrite(_dirPin, HIGH);
        
      // spin the stepper motor
      for (int i = 0; i < _stepsPerRevolutionLeft; i++) 
      {
        // These four lines result in 1 step:
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(_stepPin, LOW);
        delayMicroseconds(1000);
      }
      
    } 

    // define stepper movement function
    void moveRight(int stepsPerRevolutionRight) {

      _stepsPerRevolutionRight = stepsPerRevolutionRight;

      // Set the spinning direction right, or clockwise:
      digitalWrite(_dirPin, LOW);
        
      // spin the stepper motor
      for (int i = 0; i < stepsPerRevolutionRight; i++) 
      {
        // These four lines result in 1 step:
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(_stepPin, LOW);
        delayMicroseconds(1000);
      }
      
    }   
};

#endif // _STEPPER_H_