#include <WiFi.h>                                // esp32 library
#include <FirebaseESP32.h>                       // firebase library
#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 19
#define IN2 18
#define IN3 10
#define IN4 4

#define FIREBASE_HOST "https://ps70-week-9-default-rtdb.firebaseio.com/"  // the project name address from firebase id
#define FIREBASE_AUTH "2eCvifu0nm6sZbEiz8NMaCrHV9wRS21cMwPCOcDF"                          // the secret key generated from firebase
#define WIFI_SSID "MAKERSPACE"                                // input your home or public wifi name
#define WIFI_PASSWORD "12345678"                            // password of wifi ssid

String fireInt = "";                                        

//Define FirebaseESP32 data object
FirebaseData firebaseData;

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  // set the speed at 5 rpm
  myStepper.setSpeed(5);
  // initialize the serial port
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                          // try to connect with wifi

  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                // print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                  // connect to firebase
  Firebase.reconnectWiFi(true);
  Firebase.set(firebaseData, "/STEPPER", fireInt);              // set initial string of "OFF"

}

void loop() {

  Firebase.get(firebaseData, "/STEPPER");                     // get led status input from firebase
  fireInt = firebaseData.intData();                        // change to e.g. intData() or boolData()

if (fireInt == "ON") {
    Serial.println("ON");
     myStepper.step(-stepsPerRevolution);
  }
else if (fireInt =="OFF") {
  Serial.println("OFF");
  myStepper.step(0);
  }
}