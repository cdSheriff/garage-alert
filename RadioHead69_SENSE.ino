// rf69 demo tx rx.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging client
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_server.
// Demonstrates the use of AES encryption, setting the frequency and modem 
// configuration

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

/************ Radio Setup ***************/

// recommended 868 or 915
#define RF69_FREQ 915.0

// who am i? (server address)
#define HOME     1




//#if defined(ARDUINO_SAMD_FEATHER_M0) // Feather M0 w/Radio
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4
#define LED           13
//#endif

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, HOME);


int16_t packetnum = 0;  // packet counter, we increment per xmission

uint8_t reedState[] = {0, 0}; // reed sensor starts open

void setup() 
{
  Serial.begin(115200);
  //while (!Serial) { delay(1); } // wait until serial console is open, remove if not tethered to computer

  pinMode(LED, OUTPUT);
  pinMode(A0, INPUT_PULLUP); // reed switch N/O
  pinMode(A1, INPUT_PULLUP); // reed switch N/C
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather Addressed RFM69 RX Test!");
  Serial.println();

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}


// Dont put this on the stack:
//uint8_t data[] = "And hello back to you";
//uint8_t dataOpen[] = "Door open";
//uint8_t dataClosed[] = "Door closed";
// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void loop() {
  if (rf69_manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAck(buf, &len, &from)) {
      buf[len] = 0; // zero out remaining string
      
      Serial.print("Got packet from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.print("] : ");
      Serial.println((char*)buf);
      Blink(LED, 40, 3); //blink LED 3 times, 40ms between blinks

      
      // Send a reply back to the originator client
//      if (!rf69_manager.sendtoWait(data, sizeof(data), from))
//        Serial.println("Sending failed (no ack)");

//       Test reed sensor, send response based on reed sensor status
      if (digitalRead(A0) == HIGH && digitalRead(A1) == LOW) {
        reedState[0] = 0;
        reedState[1] = 1;
//          Serial.println(sizeof(reedState));
        for(int i = 0; i < sizeof(reedState); i++)
        {
          Serial.println(reedState[i]);
        }
        if (!rf69_manager.sendtoWait(reedState, sizeof(reedState), from))
        Serial.println("Sending failed (no ack)");
      } else if (digitalRead(A0) == LOW && digitalRead(A1) == HIGH) {
        reedState[0] = 1;
        reedState[1] = 0;
        if (!rf69_manager.sendtoWait(reedState, sizeof(reedState), from))
        Serial.println("Sending failed (no ack)");
      } else {
        reedState[0] = 0;
        reedState[1] = 0;
        if (!rf69_manager.sendtoWait(reedState, sizeof(reedState), from))
        Serial.println("Sending failed (no ack)");
      };
//      }
    };
  };
};


void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}
