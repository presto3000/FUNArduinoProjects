#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pin definitions for motor PWM control
int enA = 5;    // Motor 1 PWM control
int enB = 6;    // Motor 2 PWM control
int enC = 3;    // Motor 3 PWM control
int enD = 9;    // Motor 4 PWM control

// Initialize RF24 radio on pins 7 (CE) and 8 (CSN)
RF24 radio(7, 8);
const byte address[6] = "00002";  // Address for communication

// Shift register pins 74HC595 
const int latchPin = A0;  // RCLK
const int clockPin = 2;  // SRCK
const int dataPin = 4;   // SER

// HC-SR04 pins using analog pins
const int trigPin = A1;   // Trigger pin
const int echoPin = A2;   // Echo pin

// Autonomous mode
bool autonomousMode = false;

void setup() {
  Serial.begin(9600);         // Initialize serial communication for debugging
  radio.begin();              // Start the RF24 radio
  radio.openReadingPipe(1, address);  // Set up the address for receiving
  radio.setPALevel(RF24_PA_MIN);  // Set power level
  radio.startListening();      // Start listening for data

  // Set up motor control PWM pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(enD, OUTPUT);
  
  // Set up shift register pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Check for manual mode radio control first
  if (radio.available()) {
    char data[30]; // Buffer for receiving data
    radio.read(&data, sizeof(data));  // Read the data
    Serial.print("Received: ");
    Serial.println(data);

    // Parse the received data (S:motorSpeed T:turnSpeed B:buttonState)
    int motorSpeed, turnSpeed, buttonState;
    sscanf(data, "S:%d T:%d B:%d", &motorSpeed, &turnSpeed, &buttonState);
    
    // Check if button pressed for autonomous mode
    autonomousMode = (buttonState == 1);
    
    if (!autonomousMode) {
      Serial.println("Manual Control Mode Activated.");

      // Manual control logic
      if (motorSpeed > 0) {
        moveAllForward(motorSpeed);  // Move forward at the specified speed
      } else if (motorSpeed < 0) {
        moveAllBackward(-motorSpeed); // Move backward at the specified speed
      } else {
        // No forward/backward movement, check for turning
        if (turnSpeed < 0) {
          turnRight(-turnSpeed);  // Turn right (negative value for left joystick)
        } else if (turnSpeed > 0) {
          turnLeft(turnSpeed);    // Turn left (positive value for right joystick)
        } else {
          stopMotors();           // Stop if no movement commands
        }
      }
    }
  }

  // Autonomous Mode Logic
  if (autonomousMode) {
    Serial.println("Autonomous Mode Activated.");

    // Measure the distance continuously
    long distance = measureDistance();
    Serial.print("Measured distance: ");
    Serial.print(distance);
    Serial.println(" cm");  // Print distance in centimeters

    // Obstacle detected within 40cm
    if (distance < 40) 
    {
        stopMotors();
        turnRight(200);
        delay(1000);
    } 
    else 
    {
      moveAllForward(200);  // Move forward if no obstacle detected
    }
  }
}


// Function to measure distance using HC-SR04
long measureDistance() {
  // Clear the trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigger to HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in centimeters (speed of sound ~ 34300 cm/s)
  long distance = duration * 0.034 / 2;
  
  return distance;
}

void shiftOutData(byte data) {
  digitalWrite(latchPin, LOW); // Prepare shift register to receive data
  for (int i = 0; i < 8; i++) 
  { // Send 8 bits for motor control
    digitalWrite(clockPin, LOW); // Set clock low
    digitalWrite(dataPin, (data >> i) & 0x01); // Write bit to register
    digitalWrite(clockPin, HIGH); // Set clock high to send bit
  }
  digitalWrite(latchPin, HIGH); // Update output
}

void moveAllForward(int speed) {
  byte data = 0b00000000;  // Start with all bits LOW
  
  // Set Motor 1 forward (Q0 = HIGH, Q1 = LOW)
  data |= 0b00000010;  // Set Q0 HIGH
  
  // Set Motor 2 forward (Q2 = HIGH, Q3 = LOW)
  data |= 0b00001000;  // Set Q2 HIGH
  
  // Set Motor 3 forward (Q4 = HIGH, Q5 = LOW)
  data |= 0b00100000;  // Set Q4 HIGH
  
  // Set Motor 4 forward (Q6 = HIGH, Q7 = LOW)
  data |= 0b10000000;  // Set Q6 HIGH
  
  shiftOutData(data);  // Send the combined control byte
  
  // Speed control for each motor
  analogWrite(enA, speed);  // Motor 1
  analogWrite(enB, speed);  // Motor 2
  analogWrite(enC, speed);  // Motor 3
  analogWrite(enD, speed);  // Motor 4
}

void moveAllBackward(int speed) {
   byte data = 0b00000000;  // Start with all bits LOW
  
  // Set Motor 1 backward (Q0 = LOW, Q1 = HIGH)
  data |= 0b00000001;  // Set Q1 HIGH
  
  // Set Motor 2 backward (Q2 = LOW, Q3 = HIGH)
  data |= 0b00000100;  // Set Q3 HIGH
  
  // Set Motor 3 backward (Q4 = LOW, Q5 = HIGH)
  data |= 0b00010000;  // Set Q5 HIGH
  
  // Set Motor 4 backward (Q6 = LOW, Q7 = HIGH)
  data |= 0b01000000;  // Set Q7 HIGH
  
  shiftOutData(data);  // Send the combined control byte
  
  // Speed control for each motor
  analogWrite(enA, speed);  // Motor 1
  analogWrite(enB, speed);  // Motor 2
  analogWrite(enC, speed);  // Motor 3
  analogWrite(enD, speed);  // Motor 4
}

void turnLeft(int speed) {
    byte data = 0b00000000;  // Start with all bits LOW
  
    // Set Motor 1 forward (Q0 = HIGH, Q1 = LOW)
    data |= 0b00000010;  // Set Q0 HIGH (forward)
  
    // Set Motor 2 backward (Q2 = LOW, Q3 = HIGH)
    data |= 0b00000100;  // Set Q3 HIGH (backward)
  
    // Set Motor 3 backward (Q4 = LOW, Q5 = HIGH)
    data |= 0b00010000;  // Set Q5 HIGH (backward)
  
    // Set Motor 4 forward (Q6 = HIGH, Q7 = LOW)
    data |= 0b10000000;  // Set Q6 HIGH (forward)
  
    shiftOutData(data);  // Send the combined control byte
  
    // Speed control for each motor
    analogWrite(enA, speed);  // Motor 1
    analogWrite(enB, speed);  // Motor 2
    analogWrite(enC, speed);  // Motor 3
    analogWrite(enD, speed);  // Motor 4
}

void turnRight(int speed) {
    byte data = 0b00000000;  // Start with all bits LOW
  
    // Set Motor 1 backward (Q0 = LOW, Q1 = HIGH)
    data |= 0b00000001;  // Set Q1 HIGH (backward)
  
    // Set Motor 2 forward (Q2 = HIGH, Q3 = LOW)
    data |= 0b00001000;  // Set Q2 HIGH (forward)
  
    // Set Motor 3 forward (Q4 = HIGH, Q5 = LOW)
    data |= 0b00100000;  // Set Q4 HIGH (forward)
  
    // Set Motor 4 backward (Q6 = LOW, Q7 = HIGH)
    data |= 0b01000000;  // Set Q7 HIGH (backward)
  
    shiftOutData(data);  // Send the combined control byte
  
    // Speed control for each motor
    analogWrite(enA, speed);  // Motor 1
    analogWrite(enB, speed);  // Motor 2
    analogWrite(enC, speed);  // Motor 3
    analogWrite(enD, speed);  // Motor 4
}

  void stopMotors() {
  shiftOutData(0b00000000);  // All motors stopped (Q0-Q7 LOW)
  
  // Stop the PWM signals for each motor
  analogWrite(enA, 0);       // Speed 0 (Motor 1)
  analogWrite(enB, 0);       // Speed 0 (Motor 2)
  analogWrite(enC, 0);       // Speed 0 (Motor 3)
  analogWrite(enD, 0);       // Speed 0 (Motor 4)
}