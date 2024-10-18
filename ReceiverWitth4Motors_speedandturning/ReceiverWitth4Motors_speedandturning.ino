#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pin definitions for motors
int in1 = A5;   // Motor 1 direction
int in2 = 10;   // Motor 1 direction
int enA = 5;    // Motor 1 PWM control

int in3 = A2;   // Motor 2 direction
int in4 = A3;   // Motor 2 direction
int enB = 6;    // Motor 2 PWM control

int in5 = 4;    // Motor 3 direction
int in6 = A1;   // Motor 3 direction
int enC = 3;    // Motor 3 PWM control

int in7 = A0;   // Motor 4 direction
int in8 = 2;    // Motor 4 direction
int enD = 9;    // Motor 4 PWM control

// Initialize RF24 radio on pins 7 (CE) and 8 (CSN)
RF24 radio(7, 8);  

const byte address[6] = "00002";  // Address for communication

void setup() {
  Serial.begin(9600);         // Initialize serial communication for debugging
  radio.begin();              // Start the RF24 radio
  radio.openReadingPipe(1, address);  // Set up the address for receiving
  radio.setPALevel(RF24_PA_MIN);  // Set power level
  radio.startListening();      // Start listening for data

  // Set up motor control pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  pinMode(enC, OUTPUT);
  
  pinMode(in7, OUTPUT);
  pinMode(in8, OUTPUT);
  pinMode(enD, OUTPUT);
}

void loop() {
   // Check if data is available
  if (radio.available()) {
    char data[30]; // Buffer for receiving data
    radio.read(&data, sizeof(data));  // Read the data

    // Print the received message for debugging
    Serial.print("Received: ");
    Serial.println(data);

    // Parse the received data (S:motorSpeed T:turnSpeed B:buttonState)
    int motorSpeed, turnSpeed, buttonState;
    sscanf(data, "S:%d T:%d B:%d", &motorSpeed, &turnSpeed, &buttonState);

    // Handle forward/backward movement (motorSpeed)
    if (motorSpeed > 0) {
      moveAllForward(motorSpeed);  // Move forward at the specified speed
    } else if (motorSpeed < 0) {
      moveAllBackward(-motorSpeed); // Move backward at the specified speed (negate motorSpeed)
    } else {
      // If no forward/backward command, we handle turning or stop
      if (turnSpeed < 0) {
        turnRight(-turnSpeed);  // Corrected: Turn right when joystick is moved left
      } else if (turnSpeed > 0) {
        turnLeft(turnSpeed);  // Corrected: Turn left when joystick is moved right
      } else {
        stopMotors();  // Stop all motors if both motorSpeed and turnSpeed are zero
      }
    }
  }
}

void moveAllForward(int speed) {
  // Move all motors forward at the specified speed
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, speed);  // Speed control for Motor 1
  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed);  // Speed control for Motor 2

  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, speed);  // Speed control for Motor 3
  
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
  analogWrite(enD, speed);  // Speed control for Motor 4
}

void moveAllBackward(int speed) {
  // Move all motors backward at the specified speed
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed);  // Speed control for Motor 1
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speed);  // Speed control for Motor 2

  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, speed);  // Speed control for Motor 3
  
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, speed);  // Speed control for Motor 4
}

void turnLeft(int speed) {
  // Turn left: Motors 1 and 4 forward, Motors 2 and 3 backward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, speed); // Speed for Motor 1
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speed); // Speed for Motor 2

  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, speed); // Speed for Motor 3
  
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
  analogWrite(enD, speed); // Speed for Motor 4
}

void turnRight(int speed) {
  // Turn right: Motors 1 and 4 backward, Motors 2 and 3 forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed); // Speed for Motor 1
  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed); // Speed for Motor 2

  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, speed); // Speed for Motor 3
  
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, speed); // Speed for Motor 4
}

void stopMotors() {
  // Stop all motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);    // Stop Motor 1
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);    // Stop Motor 2

  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  analogWrite(enC, 0);    // Stop Motor 3
  
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);
  analogWrite(enD, 0);    // Stop Motor 4
}