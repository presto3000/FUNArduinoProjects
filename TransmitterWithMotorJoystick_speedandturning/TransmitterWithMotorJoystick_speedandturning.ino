#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pin definitions for joystick
const int vrxPin = A0;   // Analog pin for X-axis
const int vryPin = A1;   // Analog pin for Y-axis
const int swPin = 2;     // Digital pin for joystick button

// Previous motor and turn speeds to track the last set speed
int motorSpeed = 0;
int turnSpeed = 0;
int prevMotorSpeed = 0;  // Store the previous motor speed
int prevTurnSpeed = 0;   // Store the previous turn speed

const int maxSpeed = 255;   // Max speed for forward/backward motion
const int maxTurnSpeed = 255; // Max speed for turning
const int rampStep = 40;     // Step value for smooth acceleration

// Initialize RF24 radio on pins 9 (CE) and 10 (CSN)
RF24 radio(9, 10);  

const byte address[6] = "00002";  // Address for communication

// Tolerance for determining the joystick center
const int tolerance = 20;  // Adjust this value as needed
const int centerValue = 512; // Center value for joystick

void setup() {
  Serial.begin(9600);         // Initialize serial communication for debugging
  radio.begin();              // Start the RF24 radio
  radio.openWritingPipe(address);  // Set up the address for transmitting
  radio.setPALevel(RF24_PA_MIN);   // Set power level (try higher if range is an issue)
  radio.stopListening();           // Set as transmitter

  // Set up the button pin as input with pull-up resistor
  pinMode(swPin, INPUT_PULLUP);
}

void loop() {
  // Read the X and Y analog values (range: 0 - 1023)
  int xValue = analogRead(vrxPin);  // X-axis value
  int yValue = analogRead(vryPin);  // Y-axis value

  // Read the button state (pressed: LOW, not pressed: HIGH)
  int buttonState = digitalRead(swPin);

  // Target speeds for motors (forward/backward) and turning (left/right)
  int targetMotorSpeed = 0;
  int targetTurnSpeed = 0;

  // Forward/Backward control for motors based on joystick Y-axis
  if (yValue < centerValue - tolerance) {
    // Moving forward, map joystick Y-value
    targetMotorSpeed = map(yValue, 0, centerValue - tolerance, maxSpeed, 0);  
  } else if (yValue > centerValue + tolerance) {
    // Moving backward, map joystick Y-value
    targetMotorSpeed = map(yValue, centerValue + tolerance, 1023, 0, maxSpeed);  
    targetMotorSpeed = -targetMotorSpeed;  // Set negative speed for backward
  }

  // Left/Right turning control based on joystick X-axis
  if (xValue < centerValue - tolerance) {
    // Turning left, map joystick X-value
    targetTurnSpeed = map(xValue, 0, centerValue - tolerance, maxTurnSpeed, 0);  
    targetTurnSpeed = -targetTurnSpeed;  // Set negative speed for left turn
  } else if (xValue > centerValue + tolerance) {
    // Turning right, map joystick X-value
    targetTurnSpeed = map(xValue, centerValue + tolerance, 1023, 0, maxTurnSpeed);  
  }

  // Ramp the speeds for smooth acceleration
  motorSpeed = rampSpeed(motorSpeed, targetMotorSpeed, rampStep);
  turnSpeed = rampSpeed(turnSpeed, targetTurnSpeed, rampStep);

  // If joystick returns to center, reset motor and turn speeds
  if (abs(yValue - centerValue) <= tolerance) {
    motorSpeed = 0;  // Joystick centered vertically
  }
  if (abs(xValue - centerValue) <= tolerance) {
    turnSpeed = 0;   // Joystick centered horizontally
  }

  // Check if we need to send data (only send if speed has changed)
  if (motorSpeed != prevMotorSpeed || turnSpeed != prevTurnSpeed || buttonState == LOW) {
    // Prepare the data to send (motor speed, turn speed, button state)
    char data[30];  // Buffer for sending data
    snprintf(data, sizeof(data), "S:%d T:%d B:%d", motorSpeed, turnSpeed, buttonState);

    // Send data
    radio.write(&data, sizeof(data));  // Send the joystick data
    Serial.print("Message sent: ");
    Serial.println(data);  // Print the sent message for debugging

    // Update previous speed values
    prevMotorSpeed = motorSpeed;
    prevTurnSpeed = turnSpeed;
  }

  // Small delay to prevent flooding the receiver
  delay(100);
}

// Function to gradually ramp the speed
int rampSpeed(int currentSpeed, int targetSpeed, int step) {
  if (currentSpeed < targetSpeed) 
  {
    return min(currentSpeed + step, targetSpeed); // Gradually increase speed
  } else if (currentSpeed > targetSpeed) 
  {
    return max(currentSpeed - step, targetSpeed); // Gradually decrease speed
  } else {
    return currentSpeed; // No change if already at target speed
  }
}