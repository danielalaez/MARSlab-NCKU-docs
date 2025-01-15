#include <Servo.h>

Servo esc;  // Create a Servo object for the ESC
const int escPin = 9;  // Define the pin connected to the ESC signal wire
int pwmValue = 1000;  // Default PWM value (neutral throttle)

void setup() {
  esc.attach(escPin, 1000, 2000);  // Attach the ESC with min and max pulse width in microseconds
  
  Serial.begin(115200);
  Serial.println("Calibrating ESC...");
  /**/
  // Step 1: Send max throttle to enter calibration mode
  esc.writeMicroseconds(2000);
  Serial.println("Sending max throttle (2000 us)");
  delay(10000);  // Wait for the ESC to recognize max throttle

  // Step 2: Send min throttle to complete calibration
  esc.writeMicroseconds(1000);
  Serial.println("Sending min throttle (1000 us)");
  delay(10000);  // Wait for the ESC to recognize min throttle

  // Step 3: Control
  Serial.println("ESC control started.");
  Serial.println("Enter a value between 1000 and 2000 to control the throttle:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read user input
    pwmValue = input.toInt();  // Convert input to an integer

    // Validate the input range
    if (pwmValue >= 1000 && pwmValue <= 2000) {
      esc.writeMicroseconds(pwmValue);  // Send the PWM signal to the ESC
      Serial.print("Throttle set to: ");
      Serial.println(pwmValue);
    } else {
      Serial.println("Invalid input. Please enter a value between 1000 and 2000.");
    }
  }
}
