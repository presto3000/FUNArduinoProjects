                                                                                                                                                                                                                                                                                                                                                                 #include <SPI.h>
// Define motor pins for L298N Module 1
int in1 = 9;
int in2 = 10;
int enA = 5; // PWM-capable

int in3 = A2;
int in4 = A3;
int enB = 6; // PWM-capable

// Define motor pins for L298N Module 2
int in5 = 4;
int in6 = A1;
int enC = 3; // PWM-capable

int in7 = A0;
int in8 = 2;
int enD = 11; // PWM-capable

void setup() { 
  // Set up motor control pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);                        
  pinMode(in7, OUTPUT);
  pinMode(in8, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(enD, OUTPUT);
  
  // Initialize motors to stop
  stopMotors();
}

void loop() 
{
  moveForward(255);
  delay(3000);  // Run both motors for 3 seconds
  stopMotors();
  delay(1000);  // Wait 1 second before repeating the loop
}

void moveForward(int speed) {
  // Move all motors forward at a specified speed (0-255)

  // L298N Module 1 (Motors 1 and 2)
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, speed);  // Speed control for Motor 1
  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed);  // Speed control for Motor 2

  // L298N Module 2 (Motors 3 and 4)
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, speed);  // Speed control for Motor 3
  
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
  analogWrite(enD, speed);  // Speed control for Motor 4
}

void stopMotors() {
  // Stop all motors

  // L298N Module 1 (Motors 1 and 2)
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);    // Stop Motor 1
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);    // Stop Motor 2

  // L298N Module 2 (Motors 3 and 4)
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  analogWrite(enC, 0);    // Stop Motor 3
  
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);
  analogWrite(enD, 0);    // Stop Motor 4
}