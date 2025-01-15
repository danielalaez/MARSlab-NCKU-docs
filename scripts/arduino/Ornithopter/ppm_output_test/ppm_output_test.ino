// Digital pin PPM output
// Pin 3 > 1k resistor
// Pin GND > 2k resistor & RADIO PIN 2 (starting from bottom left to right)
// Voltage divider (3V) > RADIO PIN 1 (bottom left)
const int ppmPin = 3;

int ppmChannels[8] = {1300, 1300, 1300, 1300, 1100, 1100, 1100, 1100}; // Default at center (1300µs). Min: 1100us, Max: 1500 us.

// PPM signal parameters
const int frameRate = 22000;  // 22 ms frame rate (22 ms = 22000 microseconds)
const int pulseMin = 1100;    // Minimum pulse width (1100µs)
const int pulseMax = 1900;    // Maximum pulse width (1900µs)
const int pulseSpacing = 400; // Gap between pulses in microseconds (space between channels)

// Timing variables
unsigned long lastTime = 0;

void setup() {
  // Set the PPM output pin to OUTPUT
  pinMode(ppmPin, OUTPUT);
}

void loop() {
  unsigned long currentTime = micros();
  
  // Generate PPM signal only every 22ms (50Hz)
  if (currentTime - lastTime >= frameRate) {
    lastTime = currentTime;
    
    // Start the PPM signal frame: first set the signal to 5V for 400µs before the first channel
    digitalWrite(ppmPin, HIGH);
    delayMicroseconds(pulseSpacing); // Pulse width before first channel

    // Generate pulses for each channel
    for (int i = 0; i < 8; i++) {
      // Set the signal to LOW (0V) during the pulse width for the current channel
      digitalWrite(ppmPin, LOW);
      delayMicroseconds(ppmChannels[i]); // Pulse width for this channel

      // After the pulse width, set the signal to HIGH for the 400µs gap between channels
      digitalWrite(ppmPin, HIGH);
      delayMicroseconds(pulseSpacing); // Gap between channels
    }

    // After the last channel, set the signal to 5V for another 400µs
    digitalWrite(ppmPin, HIGH);
    delayMicroseconds(pulseSpacing); // Pulse width after the last channel
    digitalWrite(ppmPin, LOW);
  }
}
