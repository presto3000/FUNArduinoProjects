// 74HC595 test
// Pin definitions
const int latchPin = 4; // ST_CP
const int clockPin = 3; // SH_CP
const int dataPin = 7;  // DS

void setup() {
  // Set pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // Loop through each LED
  for (int i = 0; i < 8; i++) {
    // Shift out the data to the shift register
    shiftOut(dataPin, clockPin, MSBFIRST, 1 << i);
    
    // Latch the output
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);

    // Delay to see the LEDs turn on and off
    delay(500);
  }
  
  // Loop back in reverse
  for (int i = 6; i >= 0; i--) {
    shiftOut(dataPin, clockPin, MSBFIRST, 1 << i);
    
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);

    delay(500);
  }
}