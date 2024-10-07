// PINS:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
const int ledPin2 = 12;   // the number of the second LED pin

int buttonState = 0;        // current state of the button
int lastButtonState = 0;    // previous state of the button
int ledState = LOW;         // current state of the LED

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

}

void loop() {
  // read the current state of the button
  buttonState = digitalRead(buttonPin);

  // check if the button has been pressed (transition from LOW to HIGH)
  if (buttonState == HIGH && lastButtonState == LOW) 
  {
    // toggle the LED state
    ledState = !ledState;
    // update the LED to the new state
    digitalWrite(ledPin, ledState);
    digitalWrite(ledPin2, ledState);
    // add a small delay to debounce the button
    delay(200);
  }

  // save the current button state for the next loop
  lastButtonState = buttonState;
}