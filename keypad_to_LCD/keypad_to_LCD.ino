/*
  LiquidCrystal Library - Serial Input

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch displays text sent over the serial port
 (e.g. from the Serial Monitor) on an attached LCD.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5 NO 4
 * LCD D5 pin to digital pin 4 NO 5
 * LCD D6 pin to digital pin 3 NO 6
 * LCD D7 pin to digital pin 2 NO 7 
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalSerial
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);



const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A11, A12, A13, A14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A8, A9, A10}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char storeKey;

String addKey = "";

String passKey = "11111111";

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop() {
  // when characters arrive over the serial port...
//  if (customKeypad.getKey){
//    
//  }
  char customKey = customKeypad.getKey();
  
  if (customKey){
    storeKey = customKey;
    addKey = addKey + customKey;

    Serial.print("Input: ");
    Serial.print(customKey);
//    Serial.print(", Output: ");
//    Serial.print(storeKey);
    Serial.print(", add: ");
    Serial.println(addKey);

    if (addKey.length() == 8 && addKey == passKey) {
      lcd.setCursor(0, 1);
      lcd.clear();
      lcd.print("SUCCESS!");
      addKey = "";
      
    } else if (addKey.length() == 8 && addKey != passKey) {
      lcd.setCursor(0, 1);
      lcd.clear();
      lcd.print("WRONG CODE!");
      addKey = "";
    }
//    Serial.print("Input: ");
//    Serial.print(customKey);
////    Serial.print(", Output: ");
////    Serial.print(storeKey);
//    Serial.print(", add: ");
//    Serial.println(addKey);

//    lcd.setCursor(0, 1);
//    lcd.print(addKey);
//  }
  
  //  if (Serial.available()) {
  //    Serial.println("initiating talkies");
  //    // wait a bit for the entire message to arrive
  //    delay(100);
  //    // clear the screen
  //    lcd.clear();
  ////    // read all the available characters
  //    while (Serial.available() > 0) {
  //      // display each character to the LCD
  ////      lcd.write(Serial.read());
  //    lcd.write(customKey);
  //    }
  //  }
  }
}
