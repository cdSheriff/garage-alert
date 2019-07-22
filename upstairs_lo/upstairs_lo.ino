/////////////////////////////////////REDO////////////////////////////////////////////

// rf69 demo tx rx.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_server.
// Demonstrates the use of AES encryption, setting the frequency and modem 
// configuration

#include <SPI.h>
#include <RH_RF69.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define frequency     915.0 // duh
#define radio_CS      8     // radio chip select (where the pin is?)
#define radio_INT     3     // radio interrupt pin
#define radio_RST     4     // radio reset pin
#define LED           13    // onboard LED pin
#define RED           A0    // red light for door open
#define GREEN         A1    // green light for door closed
#define RED_PI        A2    // door open pin to Pi
#define GREEN_PI      A3    // door closed pin to Pi

// Singleton instance of the radio driver
RH_RF69 rf69(radio_CS, radio_INT);

//int16_t packetnum = 0;  // packet counter, we increment per xmission

int8_t reedState[] = {0, 0}; // in case I wanna save the sent info

unsigned long previousMils = 0; // update the time counter this way

const long timeoutMils = 15*60*1000; // set the timeout in milliseconds

void setup() 
{
//  Serial.begin(115200);
  //while (!Serial) { delay(1); } // wait until serial console is open

  // set the onboard LED as powerable for status blinky or whatever
  pinMode(LED, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED_PI, OUTPUT);
  pinMode(GREEN_PI, OUTPUT);     

  // setting the radio as powerable, unpower the radio reset pin
  pinMode(radio_RST, OUTPUT);
  digitalWrite(radio_RST, LOW);

//  Serial.println("Feather RFM69 RX Test!");
//  Serial.println();

  // manual reset the radio by pumping some power to the reset pin
  digitalWrite(radio_RST, HIGH);
  delay(10);
  digitalWrite(radio_RST, LOW);
  delay(10);
  
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(frequency)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
//  pinMode(LED, OUTPUT);

//  Serial.print("RFM69 radio @");  Serial.print((int)frequency);  Serial.println(" MHz");

// initialize lights as error for sanity
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN_PI, HIGH);
  digitalWrite(RED_PI, HIGH);
}


void loop() {

  unsigned long currentMils = millis();

//  Serial.println("starting loop");
  
  if (currentMils - previousMils < timeoutMils) {

//    Serial.println("current mils still low");


    if (rf69.available()) {
      Serial.println("ending wait");
  //  if (rf69.waitAvailableTimeout(500)) {
      // Should be a message for us now   
      uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (rf69.recv(buf, &len)) {
        if (!len) return;
        Serial.println("gonna blink to show it came!");
  //      Blink(LED, 200, 6);
        digitalWrite(LED, HIGH);
  
        testBuf(buf);
  
        digitalWrite(LED, LOW);
      } else {
        Serial.println("timeout!");
        return;
      }
    }

    previousMils = currentMils;
  } else {
    Serial.println("timed out. Error code lights and restart listening");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED_PI, HIGH);
    digitalWrite(GREEN_PI, HIGH);

    previousMils = currentMils;
  }

//  Serial.println("end main");
}


void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}

//void unBlink(byte PIN, byte MS, byte loops) {
//  
//}
//
//void timeOut(byte seconds) {
//  
//}

void testBuf(uint8_t* BUF) {
  if (BUF[0] == 0 && BUF[1] == 1) { // open door
    Serial.println("door open!");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED_PI, HIGH);
    digitalWrite(GREEN_PI, LOW);
  } else if (BUF[0] == 1 && BUF[1] == 0) {
    Serial.println("door closed!");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED_PI, LOW);
    digitalWrite(GREEN_PI, HIGH);
  } else if (BUF[0] == 0 && BUF[1] == 0) {
    Serial.println("sensor error!");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED_PI, HIGH);
    digitalWrite(GREEN_PI, HIGH);
  } else {
    Serial.println("sent garbage!");
  };
}
