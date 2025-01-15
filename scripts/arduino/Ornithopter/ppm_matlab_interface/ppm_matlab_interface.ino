// Digital pin PPM output
const int ppmPin = 3;

// Initialize PPM channels: Throttle, Roll, Pitch, Yaw, and others
int ppmChannels[8] = {500, 500, 500, 500, 500, 500, 500, 500};

// PPM signal parameters
const int frameRate = 22000;  // 22 ms frame rate (22 ms = 22000 microseconds)
const int pulseSpacing = 400; // Gap between pulses in microseconds

// Serial buffer to store incoming bytes
byte serialBuffer[6];
int bufferIndex = 0;

// Flag to indicate new data received
bool newDataReceived = false;

void setup() {
  // Set the PPM output pin to OUTPUT
  pinMode(ppmPin, OUTPUT);

  // Initialize Serial communication
  Serial.begin(115200);
}

void loop() {
  // Read and process serial data
  while (Serial.available() > 0) {
    byte incomingByte = Serial.read();
    
    // Store the incoming byte in the buffer
    serialBuffer[bufferIndex++] = incomingByte;
    
    // If a complete message (6 bytes) is received, process it
    if (bufferIndex >= 6) {
      bufferIndex = 0; // Reset buffer index for the next message
      processSerialData(); // Decode the received data
      newDataReceived = true; // Set the flag to indicate new data is available
    }
  }

  // Generate PPM signal only if new data has been received
  if (newDataReceived) {
    generatePPMFrame();
    newDataReceived = false; // Reset the flag after generating the frame
  }
}

void processSerialData() {
  // Decode little-endian data
  int desiredRoll  = (serialBuffer[1] << 8) | serialBuffer[0];
  int desiredPitch = (serialBuffer[3] << 8) | serialBuffer[2];
  int desiredYaw   = (serialBuffer[5] << 8) | serialBuffer[4];

  // Validate the data
  if (desiredRoll >= 500 && desiredRoll <= 1550 &&
      desiredPitch >= 500 && desiredPitch <= 1550 &&
      desiredYaw >= 500 && desiredYaw <= 1550) {
    // Assign validated values to the corresponding channels
    ppmChannels[1] = desiredRoll;  // Roll -> Channel 2
    ppmChannels[2] = desiredPitch; // Pitch -> Channel 3
    ppmChannels[3] = desiredYaw;   // Yaw -> Channel 4
    newDataReceived = true;        // Set the flag to generate a new frame
  } else {
    newDataReceived = false;       // Discard invalid data
  }
}

void generatePPMFrame() {
  int totalPulseTime = pulseSpacing; // Start with initial gap

  // Start the PPM signal frame
  digitalWrite(ppmPin, HIGH);
  delayMicroseconds(pulseSpacing);

  // Generate pulses for each channel
  for (int i = 0; i < 8; i++) {
    digitalWrite(ppmPin, LOW);
    delayMicroseconds(ppmChannels[i]);
    totalPulseTime += ppmChannels[i];

    digitalWrite(ppmPin, HIGH);
    delayMicroseconds(pulseSpacing);
    totalPulseTime += pulseSpacing;
  }

  // Sync pulse
  int syncPulse = frameRate - totalPulseTime;
  if (syncPulse > 0) {
    digitalWrite(ppmPin, LOW);
    delayMicroseconds(syncPulse);
  }
}
