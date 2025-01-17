#include <Servo.h>

Servo esc;  // Create a Servo object for the ESC
const int escPin = 11;  // Define the pin connected to the ESC signal wire
int pwmValue = 1000;  // Default PWM value (neutral throttle)

// Define the analog pin for the potentiometer
const int potPin = A3;

void setup() {
  // Start the serial communication at 9600 baud rate
  Serial.begin(115200);
  delay(3000);
  esc.attach(escPin, 970, 2020);  // Attach the ESC with min and max pulse width in microseconds
  esc.writeMicroseconds(pwmValue);  // Set default throttle value
  Serial.println("ESC control started");
}

void loop() {
  // Read the value from the potentiometer (0 to 1023)
  int potValue = analogRead(potPin);
  
  // Map the value from the range 0-1023 to 1000-2000
  int mappedValue = map(potValue, 0, 1023, 970, 2020);
  
  // Print the mapped value to the Serial Monitor
  Serial.println(mappedValue);
  if (mappedValue >= 960 && mappedValue <= 2030) {
      esc.writeMicroseconds(mappedValue);
  }
  // Wait for a short period to avoid flooding the serial monitor
  delay(100);
}
