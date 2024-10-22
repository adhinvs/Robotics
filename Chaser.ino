// Define the number of LEDs
const int numLEDs = 9;

int del = 250;

// Define the LED pin numbers
const int ledPins[numLEDs] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

// Function prototypes
void basicChaser();
void runningLight();
void bounce();
void alternateChaser();
void theaterChase();
void waveEffect();
void knightRider();
void pulse();
void heartBeat();
void randomBlink();
void crossFade();
void fallingLeaves();
void walkingLED();
void fireflyEffect();
void chaserWithPause();
void cylonEffect();
void zigzagPattern();
void chasingPairs();

void setup() {
  // Initialize LED pins as outputs
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  
  basicChaser();
  delay(del);
  basicChaser();
  delay(del);

  runningLight();
  delay(del);
  runningLight();
  delay(del);

  bounce();
  delay(del);
  bounce();
  delay(del);

  alternateChaser();
  delay(del);
  alternateChaser();
  delay(del);

  theaterChase();
  delay(del);
  theaterChase();
  delay(del);

  waveEffect();
  delay(del);
  waveEffect();
  delay(del);

  knightRider();
  delay(del);
  knightRider();
  delay(del);

  pulse();
  delay(del);
  pulse();
  delay(del);

  heartBeat();
  delay(del);
  heartBeat();
  delay(del);

  randomBlink();
  delay(del);
  randomBlink();
  delay(del);

  crossFade();
  delay(del);
  crossFade();
  delay(del);

  fallingLeaves();
  delay(del);
  fallingLeaves();
  delay(del);

  walkingLED();
  delay(del);
  walkingLED();
  delay(del);

  fireflyEffect();
  delay(del);
  fireflyEffect();
  delay(del);

  chaserWithPause();
  delay(del);
  chaserWithPause();
  delay(del);

  cylonEffect();
  delay(del);
  cylonEffect();
  delay(del);

  zigzagPattern();
  delay(del);
  zigzagPattern();
  delay(del);

  chasingPairs();
  delay(del);
  chasingPairs();
  delay(del);
}

void basicChaser() {
  Serial.println("Starting Basic Chaser");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void runningLight() {
  Serial.println("Starting Running Light");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(1000);
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void bounce() {
  Serial.println("Starting Bounce");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
  }
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
  }
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void alternateChaser() {
  Serial.println("Starting Alternate Chaser");
  for (int i = 0; i < numLEDs; i += 2) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < numLEDs; i += 2) {
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 1; i < numLEDs; i += 2) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 1; i < numLEDs; i += 2) {
    digitalWrite(ledPins[i], LOW);
  }
}

void theaterChase() {
  Serial.println("Starting Theater Chase");
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < numLEDs; i += 3) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(100);
    for (int i = 0; i < numLEDs; i += 3) {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void waveEffect() {
  Serial.println("Starting Wave Effect");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void knightRider() {
  Serial.println("Starting Knight Rider");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void pulse() {
  Serial.println("Starting Pulse");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void heartBeat() {
  Serial.println("Starting Heart Beat");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numLEDs; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(100);
    for (int j = 0; j < numLEDs; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(100);
  }
}

void randomBlink() {
  Serial.println("Starting Random Blink");
  for (int i = 0; i < numLEDs; i++) {
    int randomLED = random(0, numLEDs);
    digitalWrite(ledPins[randomLED], HIGH);
    delay(200);
    digitalWrite(ledPins[randomLED], LOW);
  }
}

void crossFade() {
  Serial.println("Starting Cross Fade");
  for (int i = 0; i < numLEDs; i += 2) {
    digitalWrite(ledPins[i], HIGH);
    if (i + 1 < numLEDs) {
      digitalWrite(ledPins[i + 1], HIGH);
    }
    delay(200);
    digitalWrite(ledPins[i], LOW);
    if (i + 1 < numLEDs) {
      digitalWrite(ledPins[i + 1], LOW);
    }
  }
}

void fallingLeaves() {
  Serial.println("Starting Falling Leaves");
  for (int i = 0; i < numLEDs; i++) {
    int randomLED = random(0, numLEDs);
    digitalWrite(ledPins[randomLED], HIGH);
    delay(200);
    digitalWrite(ledPins[randomLED], LOW);
  }
}


void walkingLED() {
  Serial.println("Starting Walking LED");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void fireflyEffect() {
  Serial.println("Starting Firefly Effect");
  for (int i = 0; i < numLEDs; i++) {
    int randomLED = random(0, numLEDs);
    digitalWrite(ledPins[randomLED], HIGH);
    delay(random(100, 400));
    digitalWrite(ledPins[randomLED], LOW);
  }
}

void chaserWithPause() {
  Serial.println("Starting Chaser with Pause");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
  delay(500); // Pause before reversing
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void cylonEffect() {
  Serial.println("Starting Cylon Effect");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void zigzagPattern() {
  Serial.println("Starting Zigzag Pattern");
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void chasingPairs() {
  Serial.println("Starting Chasing Pairs");
  for (int i = 0; i < numLEDs; i += 2) {
    digitalWrite(ledPins[i], HIGH);
    if (i + 1 < numLEDs) {
      digitalWrite(ledPins[i + 1], HIGH);
    }
    delay(200);
    digitalWrite(ledPins[i], LOW);
    if (i + 1 < numLEDs) {
      digitalWrite(ledPins[i + 1], LOW);
    }
  }
}
