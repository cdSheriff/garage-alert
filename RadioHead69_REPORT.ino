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

// Recommended 868 or 915
#define RF69_FREQ 915.0

// Address to send packets to!
#define DEST   1
// Who am I
#define HOME     2

#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4
#define LED           13
#define RED           A0
#define GREEN         A1

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, HOME);


int16_t packetnum = 0;  // increment each packet sent, do I care though?

void setup() 
{
  Serial.begin(115200);
  //while (!Serial) { delay(1); } // wait until serial console is open, remove if not tethered to computer

  pinMode(LED, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);     
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather Addressed RFM69 TX Test!");
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

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}


// Dont put this on the stack:
//uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
//uint8_t buf[] = {0,0};
//uint8_t data[] = "  OK";
uint8_t data[] = {0,0};

void loop() {
  delay(4000);  // 4 second pause between asks

  char radiopacket[20] = "Door check #";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  
  // Send a message to the DESTINATION!
  if (rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), DEST)) {
    // Now wait for a reply from the server
    uint8_t len = sizeof(data);
    uint8_t from;   
    if (rf69_manager.recvfromAckTimeout((uint8_t*)&data, &len, 2000, &from)) {

      Serial.print("Ack from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.println("]");
      
      if (data[0] == 0 && data[1] == 1) { // door is open
        Blink(RED, 200, 1);
      } else if (data[0] == 1 && data[1] == 0) { // door is closed
        Blink(GREEN, 200, 1);
      }
      Blink(LED, 40, 3); // board LED to confirm its doing worky things
    } else {
      Serial.println("No reply :(");
    }
  } else {
    Serial.println("Sending failed (no ack)");
  }
}

void Blink(byte PIN, byte MS, byte loops) { // LED pin, delay in milliseconds, number of blinks
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(MS);
    digitalWrite(PIN,LOW);
    delay(MS);
  }
}
