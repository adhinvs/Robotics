#include <ESP8266WiFi.h>
#include <espnow.h>

// Right motor pins
const int IN1 = 4;    // D2 - GPIO4
const int IN2 = 0;    // D3 - GPIO0
// Left motor pins 
const int IN3 = 14;   // D5 - GPIO14
const int IN4 = 12;   // D6 - GPIO12

unsigned long lastRecvTime = 0;
const int TIMEOUT = 1000;  // 1 second timeout

struct PacketData {
  byte xAxis;
  byte yAxis;
  byte zAxis;
};
PacketData data;

void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
  if (len == 0) return;
  
  memcpy(&data, incomingData, sizeof(data));
  lastRecvTime = millis();

  // Print raw accelerometer values
  Serial.print("Accelerometer Data - X: ");
  Serial.print(data.xAxis);
  Serial.print(" Y: ");
  Serial.print(data.yAxis);
  Serial.print(" Z: ");
  Serial.println(data.zAxis);

  // Forward
  if (data.yAxis < 75) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Action: Moving Forward");
  }
  // Backward
  else if (data.yAxis > 175) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Action: Moving Backward");
  }
  // Right
  else if (data.xAxis > 175) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Action: Turning Right");
  }
  // Left
  else if (data.xAxis < 75) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Action: Turning Left");
  }
  // Stop
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("Action: Stopped");
  }
  Serial.println("------------------------");
}

void setup() {
  Serial.begin(115200);
  
  // Set motor pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Stop motors initially
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  // Setup ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW failed to init");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Ready to receive accelerometer data");
  Serial.println("Movement triggers:");
  Serial.println("Forward: Y < 75");
  Serial.println("Backward: Y > 175");
  Serial.println("Right: X > 175");
  Serial.println("Left: X < 75");
  Serial.println("------------------------");
}

void loop() {
  // Stop motors if no signal received for 1 second
  if (millis() - lastRecvTime > TIMEOUT) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("Timeout - Motors Stopped");
  }
}
