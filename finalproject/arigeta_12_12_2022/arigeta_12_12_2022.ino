#include "Stepper.h"
#include <ESP32Servo.h>
#include "Arduino.h"
#include <ezButton.h>
#include "FirebaseESP32.h"
#include "WiFi.h"

// establish firebase connection
#define FIREBASE_HOST "https://ps70-final-project-4e7f5-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "lsrmqk4U3cFEiXLyO5p5zXM7arYvjJfoLJiRoX0h"    
#define WIFI_SSID "MAKERSPACE" 
#define WIFI_PASSWORD "12345678"

// create empty firebase variable
int fireInt;

// create instance of firebase class
FirebaseData firebaseData;

// define stepper motor connections
int dirPinX = 5; 
int stepPinX = 10;
int dirPinY = 9;
int stepPinY = 4;

// servo motor connections
int leftPinZ = 22;
int rightPinZ = 21;

// define limit switch pins
int limitX = 25;
int limitY = 26;

// create stepper motor instances
Stepper StepperX(dirPinX, stepPinX);
Stepper StepperY(dirPinY, stepPinY);

// create limit switch class instances
ezButton limitSwitchX(limitX);
ezButton limitSwitchY(limitY); 

// create servo class instances
Servo servoLeft;
Servo servoRight;

// create servo position variables
int posL; 
int posR; 

// create button pin and state variables
const int button = 12;
int buttonState = 0;
int lastButtonState = 0;

// create variable to hold number of loops coming from ESP
int loopInt;

// function to read the buttom state
bool buttonStatus(){

    // read the button state
    buttonState = digitalRead(button);

    // if the button has been pressed, return true
    if (lastButtonState == 0 && buttonState == 1)
    {
      return true;
    }
    // if it hasn't been pressed, return false
    else
    {
      return false;
    }
    
    // set last button state to current button state
    lastButtonState = buttonState;
    delay(100); 
}

// read the status of the x axis limit switch
bool limitStatusX() { 

  limitSwitchX.loop(); 

  if(limitSwitchX.isPressed() or limitSwitchX.getState() == LOW)
  { 
    return true;
  }
  else
  {
    return false;
  }
}

// read the status of the y axis limit switch
bool limitStatusY() { 

  limitSwitchY.loop(); 

  if(limitSwitchY.isPressed() or limitSwitchY.getState() == LOW)
  { 
    return true;
  }

  else
  {
    return false;
  }
}

void setup() {

  Serial.begin(9600);

  // try connecting to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   

  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                 // print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
  Firebase.reconnectWiFi(true);
  Firebase.set(firebaseData, "/LOOP_NUMBER", 1);  // set initial integer to 1

  Serial.begin(9600);

  // if the button has not been pressed, keep getting the fire int
  while (buttonStatus() == false)
  {
    Serial.println("BUTTON HAS NOT BEEN PRESSED"); // write to the monitor that the button has not been pressed

    // get loop status input from firebase
    Firebase.get(firebaseData, "/LOOP_NUMBER");
    // set firebase value to loop interger for machine painting
    fireInt = firebaseData.intData();
    Serial.println("Value coming from firebase:");
    Serial.println(fireInt);
    delay(500);

  }
  // button has been pressed
  Serial.println("BUTTON HAS BEEN PRESSED");
  // MOVE ONTO MAIN MACHINE CODE

  // initialize servo "out of water" positions
  servoLeft.write(120);
  servoRight.write(60);

  // set limit switch debounce time
  limitSwitchX.setDebounceTime(50); 
  limitSwitchY.setDebounceTime(50); 

  // while the limit switch isn't pressed, move stepper x left
  while (limitStatusX() == false)
  {
    StepperX.moveLeft(3);
    delayMicroseconds(5000);
  }

  // while the limit switch isn't pressed, move stepper y left (forward)
  while (limitStatusY() == false) 
  {
    StepperY.moveLeft(4);
    delayMicroseconds(5000);
  } 

  // attach servos to their respective pins
  servoLeft.attach(leftPinZ);
  servoRight.attach(rightPinZ);

}  

void loop() {

  // get stepper X and Y to center of water basin
  for (int i = 0; i < 1; i++)
  {
    StepperX.moveRight(540);
    StepperY.moveRight(480);
    delayMicroseconds(5000);
  } 

  // delay 2 seconds before painting
  delay(2000);

  // initiate stepper X brush movement
  for (int i = 0; i < fireInt; i++)
  {
    // left brush dips down
    for (posL = 120; posL >= 30; posL -=1)
    {
      servoLeft.write(posL);
      delay(20);
    } 
    // left brush comes up, back to initial pos
    delay(100);
    servoLeft.write(120);
    delay(1000);

    // stepper moves left
    StepperX.moveLeft(150);
    delay(500);

    // right brush moves down
    for (posR = 60; posR <=140; posR +=1)
    {
      servoRight.write(posR);
      delay(20);
    }
    // right brush comes up, back to initial pos
    delay(100); 
    servoRight.write(60);
    delay(1000);

    // stepper x moves right  
    StepperX.moveRight(150);
    delay(1000);
  }
  
  // 2 second delay before homing the machine
  delay(500);

  // home stepper X again
  while (limitStatusX() == false)
  {
    StepperX.moveLeft(4);
    delayMicroseconds(5000);
  }

  // home stepper Y again
  while (limitStatusY() == false) 
  {
    StepperY.moveLeft(4);
    delayMicroseconds(5000);
  } 

   // get stepper X and Y to (0,0)
  for (int i = 0; i < 1; i++)
  {
    StepperX.moveRight(540);
    StepperY.moveRight(540);
    delayMicroseconds(5000);
  } 

  exit(0);

}