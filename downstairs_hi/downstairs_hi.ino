// adapted from radioHead tx_raw demo
// -*- mode: C++ -*-
// Author: Colin Dess
// github: cdsheriff
// 05/08/2019
// Attached to N/C N/O reed switch
//
// main works like this:
// 1. probe reed switch to see if door is open, closed, or errored
// 1a. send uint8 array based on result (bare send, no listening for response)
// 2. flash LED to signal that something was sent
// 3. standby sleep for 10 minutes
// 3a. setup includes 'SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk' which changes from light sleep to total standby
// 3b. watchdog loops on 8 second check, once reaches 600sec/8sec loops triggers interrupt to wake
//
// shitty $10 multimeter measures 2mA quiescence and 15mA during the brief probe/send

#include <SPI.h>
#include <RH_RF69.h>
//#include <Adafruit_SleepyDog.h>

/************ Radio Setup ***************/

// define some immutable things
#define frequency     915.0 // duh
#define radio_CS      4     // radio chip select (where the pin is?)
#define radio_INT     3     // radio interrupt pin
#define radio_RST     2     // radio reset pin
//#define LED           13    // onboard LED pin

// Singleton instance of the radio driver
RH_RF69 rf69(radio_CS, radio_INT);

//int16_t packetnum = 0;  // packet counter, we increment per xmission

int8_t reedState[] = {0, 0, 0, 0};
//int sleepCyclesElapsed;
//int sleepCycles;

void setup() {
  Serial.begin(9600);
//  Serial.println("SGP30 test");
  Serial.println("start setup");
//  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  // setting up the LED as a powerable thing, and defining the reed switch pins
  pinMode(LED_BUILTIN, OUTPUT);  

  // setting the radio as powerable, unpower the radio reset pin
  pinMode(radio_RST, OUTPUT);
  digitalWrite(radio_RST, LOW);

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

  Serial.print("RFM69 radio @");  Serial.print((int)frequency);  Serial.println(" MHz");

  Blink(LED_BUILTIN, 1000, 10);
//  delay(2000);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);
}



void loop() {
//  doorTest();
  Serial.println("starting loop");
  radioTest();
  delay(10000);
  Blink(LED_BUILTIN, 200, 3);
// pass mathed sleep cycles
// passing raw seconds was one possible cuase of sleep timing error
//  Sleep((int) 600/8);
}

void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}

void radioTest() {
  reedState[0] = random(1);
  reedState[1] = random(1);
  reedState[2] = random(1);
  reedState[3] = 0;
  rf69.send((uint8_t *)reedState, sizeof(reedState));
  Serial.println("sent!");
}
