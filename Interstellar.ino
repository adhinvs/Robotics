// Define note frequencies for musical notes
#define A4  440
#define B4  494
#define C5  523
#define D5  587
#define E5  659

// Define the melody sequence using the note frequencies
int melody[] = {
  A4, E5, A4, E5, 
  B4, E5, B4, E5,
  C5, E5, C5, E5,
  D5, E5, D5, E5,
  C5, B4
};

// Define the duration of each note (in milliseconds)
int noteDurations[] = {
  500, 500, 500, 500, 
  500, 500, 500, 500,
  500, 500, 500, 500,
  500, 500, 500, 250,
  200, 200
};

#define BUZZER 10 // Define the pin connected to the buzzer
#define LED 12    // Define the pin connected to the LED

void setup() {
  // Set up the buzzer and LED pins as outputs
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Loop through each note in the melody
  for (int i = 0; i < 18; i++) {
    // Get the duration for the current note
    int noteDuration = noteDurations[i];
    
    // Turn on the LED
    digitalWrite(LED, HIGH);  
    
    // Play the note on the buzzer for the specified duration
    tone(BUZZER, melody[i], noteDuration);

    // Wait for the note to finish playing
    delay(noteDuration);

    // Turn off the LED after the note
    digitalWrite(LED, LOW); 
    
    // Control the buzzer and LED based on the note index
    if(i % 2 == 1) { // If the note index is odd
      if (i == 15) { // Special case for index 15
        digitalWrite(BUZZER, HIGH); // To Turn OFF Active Buzzer
        digitalWrite(LED, LOW);
        delay(250); // Wait for 250 ms
      }
      else if (i == 17) { // Special case for index 17
        digitalWrite(BUZZER, HIGH); // To Turn OFF Active Buzzer
        digitalWrite(LED, LOW);
        delay(50); // Wait for 50 ms
      }
      else { // General case for other odd indices
        digitalWrite(BUZZER, HIGH); // To Turn OFF Active Buzzer
        digitalWrite(LED, LOW);
        delay(500); // Wait for 500 ms
      }
    }
    else { // If the note index is even
      digitalWrite(BUZZER, HIGH); // To Turn OFF Active Buzzer
      delay(50);  // Wait for 50 ms
    }
  }
}
