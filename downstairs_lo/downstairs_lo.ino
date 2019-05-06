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


#define frequency 915.0       // recommended 868 or 915
#define HOME          1       // who am i? (server address)
#define radio_chipSelect  8
#define radio_interrupt   3
#define radio_reset   4
#define LED           13


// Singleton instance of the radio driver
RH_RF69 rf69(radio_chipSelect, radio_interrupt);

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
  pinMode(radio_reset, OUTPUT);
  digitalWrite(radio_reset, LOW);

  Serial.println("Feather Addressed RFM69 RX Test!");
  Serial.println();

  // manual reset (literally pressing the reset button by assigning it a high value)
  digitalWrite(radio_reset, HIGH);
  delay(10);
  digitalWrite(radio_reset, LOW);
  delay(10);
  
  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults to 434.0MHz, modulation GFSK_Rb250Fd250, 13dB, no encryption
  // So we need to set freq, power, and encryption key because those are all wrong
  if (!rf69.setFrequency(frequency)) {
    Serial.println("setFrequency failed");
  }

  // Have to set dB. range 14-20. Always true because reasons
  rf69.setTxPower(20, true);

  // Encryption key. Set to zeros for GitHub. Change when uploading to boards
  uint8_t key[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  rf69.setEncryptionKey(key);

  Serial.print("RFM69 radio @");  Serial.print((int)frequency);  Serial.println(" MHz");
}

// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void loop() {
  if (rf69_manager.available())
  {
    // Get message from base, and print it along with base ID and RSSI
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAck(buf, &len, &from)) {
      buf[len] = 0; // zero out remaining string
      
      Serial.print("Got packet from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.print("] : ");
      Serial.println((char*)buf);
      Blink(LED, 40, 3); //board blinking to confirm it is doing talky things

      
      // Evaluate door position, and reply to base

      // Test reed sensor, send response based on reed sensor status
      if (digitalRead(A0) == HIGH && digitalRead(A1) == LOW) {
        reedState[0] = 0;
        reedState[1] = 1;
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
    };
  };
};


void Blink(byte PIN, byte MS, byte loops) { // LED pin, delay in milliseconds, number of blinks
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(MS);
    digitalWrite(PIN,LOW);
    delay(MS);
  }
}
