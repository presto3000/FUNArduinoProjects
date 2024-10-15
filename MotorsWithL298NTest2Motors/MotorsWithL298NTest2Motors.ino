
// Arudino Uno with L298N

// Define pins for Motor A
int in1 = 9;
int in2 = 10;
int enA = 5;

// Define pins for Motor B
int in3 = 11;
int in4 = 12;
int enB = 6;

void setup() {
  
  // Set all the motor control pins as outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Enable PWM control pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  
  // Initialize motors to stop
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
}

void loop() {
  
   // ---- Run Motor A and Motor B at the same time ----
  
  // Move Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255);  // Full speed for Motor A

  // Move Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 255);  // Full speed for Motor B

  delay(3000);  // Run both motors for 3 seconds

  // ---- Stop both motors ----
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);    // Stop Motor A

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);    // Stop Motor B

  delay(1000);  // Wait 1 second before repeating the loop
  
}
