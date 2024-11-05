#include <ESP8266WiFi.h>
#include <espnow.h>

#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

// Motor pins for L298N
struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;
  int pinEn;
};

// Define motor pins (right motor and left motor)
std::vector<MOTOR_PINS> motorPins = 
{
  {4, 0, 5},    // Right motor - IN1, IN2, ENA
  {14, 12, 13}, // Left motor  - IN3, IN4, ENB
};

#define RIGHT_MOTOR 0
#define LEFT_MOTOR 1

#define MAX_MOTOR_SPEED 255

#define SIGNAL_TIMEOUT 1000  // Signal timeout in milliseconds
unsigned long lastRecvTime = 0;

struct PacketData
{
  byte xAxisValue;
  byte yAxisValue;
  byte zAxisValue;
};
PacketData receiverData;

// Callback function when data is received
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) 
{
  if (len == 0)
  {
    return;
  }
  memcpy(&receiverData, incomingData, sizeof(receiverData));
  String inputData = "values " + String(receiverData.xAxisValue) + "  " + 
                     String(receiverData.yAxisValue) + "  " + 
                     String(receiverData.zAxisValue);
  Serial.println(inputData);

  // Process movement based on received values
  if (receiverData.yAxisValue < 75)
  {
    processCarMovement(FORWARD);  
  }
  else if (receiverData.yAxisValue > 175)
  {
    processCarMovement(BACKWARD);     
  }
  else if (receiverData.xAxisValue > 175)
  {
    processCarMovement(RIGHT);   
  }
  else if (receiverData.xAxisValue < 75)
  {
    processCarMovement(LEFT);    
  }
  else
  {
    processCarMovement(STOP);     
  }

  lastRecvTime = millis();   
}

void processCarMovement(int inputValue)
{
  switch(inputValue)
  {
    case FORWARD:
      rotateMotor(RIGHT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(LEFT_MOTOR, MAX_MOTOR_SPEED);                  
      break;
  
    case BACKWARD:
      rotateMotor(RIGHT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(LEFT_MOTOR, -MAX_MOTOR_SPEED);   
      break;
  
    case LEFT:
      rotateMotor(RIGHT_MOTOR, MAX_MOTOR_SPEED);
      rotateMotor(LEFT_MOTOR, -MAX_MOTOR_SPEED);   
      break;
  
    case RIGHT:
      rotateMotor(RIGHT_MOTOR, -MAX_MOTOR_SPEED);
      rotateMotor(LEFT_MOTOR, MAX_MOTOR_SPEED);  
      break;
  
    case STOP:
    default:
      rotateMotor(RIGHT_MOTOR, STOP);
      rotateMotor(LEFT_MOTOR, STOP);    
      break;
  }
}

void rotateMotor(int motorNumber, int motorSpeed)
{
  if (motorSpeed < 0)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);    
  }
  else if (motorSpeed > 0)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);       
  }
  else
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);      
  }
  
  analogWrite(motorPins[motorNumber].pinEn, abs(motorSpeed));
}

void setUpPinModes()
{
  for (int i = 0; i < motorPins.size(); i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
    pinMode(motorPins[i].pinEn, OUTPUT);
    rotateMotor(i, STOP);  
  }
}

void setup() 
{
  setUpPinModes();
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() 
{
  // Check for signal timeout
  unsigned long now = millis();
  if (now - lastRecvTime > SIGNAL_TIMEOUT) 
  {
    processCarMovement(STOP); 
  }
}
