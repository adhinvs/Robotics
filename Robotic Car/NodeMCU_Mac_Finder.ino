#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200); // Match this baud rate with the Serial Monitor
  delay(1000);          // Small delay to let the serial monitor start
  Serial.println("Starting ESP8266...");
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Empty loop, just for testing
}
