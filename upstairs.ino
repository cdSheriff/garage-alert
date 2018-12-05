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


#define frequency 915.0       // Recommended 868 or 915
#define DEST          1       // Address to send packets to!
#define HOME          2       // Who am I
#define radio_chipSelect  8
#define radio_interrupt   3
#define radio_reset   4
#define LED           13
#define RED           A0
#define GREEN         A1

// Singleton instance of the radio driver
RH_RF69 rf69(radio_chipSelect, radio_interrupt);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, HOME);


int16_t packetnum = 0;  // increment each packet sent, do I care though?

void setup() 
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);     
  pinMode(radio_reset, OUTPUT);
  digitalWrite(radio_reset, LOW);

  Serial.println("Feather Addressed RFM69 TX Test!");
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
      Serial.print(" [RSSI :"); Serial.print(rf69.lastRssi()); Serial.println("]");
      
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
