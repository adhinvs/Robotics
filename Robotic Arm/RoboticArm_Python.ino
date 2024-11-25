#include <Servo.h>

Servo servoClaw;
Servo servoNeck;
Servo servoBody;
Servo servoBase;

const int CLAW_PIN = 9;
const int NECK_PIN = 10;
const int BODY_PIN = 11;
const int BASE_PIN = 12;

String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
  
  servoClaw.attach(CLAW_PIN, 500, 2500);
  servoNeck.attach(NECK_PIN, 500, 2500);
  servoBody.attach(BODY_PIN, 500, 2500);
  servoBase.attach(BASE_PIN, 500, 2500);
  
  // Initial positioning
  servoClaw.writeMicroseconds(1400);
  servoNeck.writeMicroseconds(1700);
  servoBody.writeMicroseconds(2050);
  servoBase.writeMicroseconds(2050);
}

void loop() {
  if (stringComplete) {
    // Split the input string by comma
    int servoPulses[4];
    int index = 0;
    
    char* token = strtok((char*)inputString.c_str(), ",;");
    while (token != NULL && index < 4) {
      servoPulses[index] = atoi(token);
      token = strtok(NULL, ",;");
      index++;
    }
    
    // Set servo positions
    servoClaw.writeMicroseconds(servoPulses[0]);
    servoNeck.writeMicroseconds(servoPulses[1]);
    servoBody.writeMicroseconds(servoPulses[2]);
    servoBase.writeMicroseconds(servoPulses[3]);
    
    // Reset for next transmission
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    
    if (inChar == ';') {
      stringComplete = true;
    }
  }
}
