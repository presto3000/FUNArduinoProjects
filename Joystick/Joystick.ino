// Pin definitions
const int vrxPin = A0;   // Analog pin for X-axis
const int vryPin = A1;   // Analog pin for Y-axis
const int swPin = 2;     // Digital pin for joystick button

// Tolerance for determining the joystick center
const int tolerance = 20;  // Adjust this value as needed

void setup() {
  // Initialize the serial communication for debugging
  Serial.begin(9600);

  // Set up the button pin as input with pull-up resistor
  pinMode(swPin, INPUT_PULLUP);
}

void loop() {
  // Read the X and Y analog values (range: 0 - 1023)
  int xValue = analogRead(vrxPin);  // X-axis value
  int yValue = analogRead(vryPin);  // Y-axis value

  // Read the button state (pressed: LOW, not pressed: HIGH)
  int buttonState = digitalRead(swPin);

  // Check if the joystick is near the center (within the tolerance range)
  if (xValue >= 512 - tolerance && xValue <= 512 + tolerance) {
    Serial.print("X-axis: Centered");
  } else {
    Serial.print("X-axis: ");
    Serial.print(xValue);
  }

  Serial.print(" | ");

  if (yValue >= 512 - tolerance && yValue <= 512 + tolerance) {
    Serial.print("Y-axis: Centered");
  } else {
    Serial.print("Y-axis: ");
    Serial.print(yValue);
  }

  Serial.print(" | Button: ");
  if (buttonState == LOW) {
    Serial.println("Pressed");
  } else {
    Serial.println("Not pressed");
  }

  // Small delay to make the output more readable
  delay(100);
}