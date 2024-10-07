#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>
// include the library code:


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//=================================================================
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address is 0x27, 16 columns, 2 rows

RF24 radio(9, 10);  // CE, CSN pins

const byte address[6] = "00002";  // Same address as transmitter

void setup() {
  lcd.init();              // Initialize the LCD
  lcd.backlight();         // Turn on the backlight
  lcd.print("");  // Print a message to the LCD

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);  // Open the address for receiving
  radio.setPALevel(RF24_PA_MIN);      // Set power level
  radio.startListening();             // Set as receiver
}

void loop() {
  if (radio.available()) 
  {
    char text[32] = "";  // Buffer for incoming message
    radio.read(&text, sizeof(text));  // Try to read the message

    if (text[0] != '\0') 
    {  // Check if the message is not empty
      Serial.print("Received message: ");
      Serial.println(text);

      // Clear the LCD before printing a new message
      lcd.clear();
      
      // Print the received message
      lcd.setCursor(0, 0);  // Reset cursor to first column, first row
      lcd.print(text);
      
      // Optionally, print time (seconds since reset) on the second row
      lcd.setCursor(0, 1);  // Set cursor to first column, second row
      lcd.print("Time: ");
      lcd.print(millis() / 1000);
    } 
  }
}
