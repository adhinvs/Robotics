int motor1pin1 = 1;
int motor1pin2 = 2;
int ENA = 9;  // Motor 1 speed control
int motor2pin1 = 3;
int motor2pin2 = 4;
int ENB = 10; // Motor 2 speed control

int joystickX = A0; // Joystick X-axis connected to pin A0
const int NEUTRAL_VALUE = 517; // Steady state value for joystick
const int FORWARD_THRESHOLD = 530; // Threshold to detect forward movement
const int BACKWARD_THRESHOLD = 490; // Threshold to detect backward movement

void setup() {
  // Motor control pins
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Initialize the serial monitor
  Serial.begin(9600);

  // Set initial motor state to stopped
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  analogWrite(ENB, 0);
}

void loop() {
  int xValue = analogRead(joystickX); // Read X-axis value from joystick

  // Print joystick X value to Serial Monitor
  Serial.print("Joystick X value: ");
  Serial.println(xValue);

  if (xValue > FORWARD_THRESHOLD) {
    // Move both motors forward
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(ENA, map(xValue, FORWARD_THRESHOLD, 1023, 0, 255)); // Adjust speed based on joystick value
    
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(ENB, map(xValue, FORWARD_THRESHOLD, 1023, 0, 255)); // Adjust speed based on joystick value
    
    Serial.println("Motor Direction: Forward");
  } else if (xValue < BACKWARD_THRESHOLD) {
    // Move both motors backward
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(ENA, map(xValue, 0, BACKWARD_THRESHOLD, 255, 0)); // Adjust speed based on joystick value
    
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(ENB, map(xValue, 0, BACKWARD_THRESHOLD, 255, 0)); // Adjust speed based on joystick value
    
    Serial.println("Motor Direction: Backward");
  } else {
    // Stop both motors if the joystick is near the neutral position
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    analogWrite(ENA, 0);

    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
    analogWrite(ENB, 0);

    Serial.println("Motor Status: Stopped");
  }

  delay(100); // Add a small delay for readability
}
