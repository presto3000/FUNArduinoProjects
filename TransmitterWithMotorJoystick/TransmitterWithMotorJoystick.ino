#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pin definitions for joystick
const int vrxPin = A0;   // Analog pin for X-axis
const int vryPin = A1;   // Analog pin for Y-axis
const int swPin = 2;     // Digital pin for joystick button

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

  // Initialize motor speed
  int motorSpeed = 0;  // Speed for motors

  // Forward/Backward control for all motors
  if (yValue < centerValue - tolerance) {
    // Map joystick Y-value for forward motion
    motorSpeed = map(yValue, 0, centerValue - tolerance, 255, 0);  // Move forward
  } else if (yValue > centerValue + tolerance) {
    // Map joystick Y-value for backward motion
    motorSpeed = map(yValue, centerValue + tolerance, 1023, 0, 255);  // Move backward
    motorSpeed = -motorSpeed;  // Set negative speed for backward motion
  }

  // Prepare the data to send
  char data[30];  // Buffer for sending data
  snprintf(data, sizeof(data), "S:%d B:%d", motorSpeed, buttonState);

  // Check if joystick moved or button pressed, then send data
  if (motorSpeed != 0 || buttonState == LOW) {
    radio.write(&data, sizeof(data));  // Send the joystick data
    Serial.print("Message sent: ");
    Serial.println(data);  // Print the sent message for debugging
  }

  // Small delay to prevent flooding the receiver
  delay(150);  // Adjust delay as needed
}