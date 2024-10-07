#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN pins

const byte address[6] = "00002";  // Address for communication

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);  // Set up the address for transmitting
  radio.setPALevel(RF24_PA_MIN);   // Set power level (try higher if range is an issue)
  radio.stopListening();           // Set as transmitter
}

void loop() {
  const char text[] = "KIcia!";
  radio.write(&text, sizeof(text));  // Send the message
  Serial.println("Message sent: KIcia!");
  delay(1000);
}