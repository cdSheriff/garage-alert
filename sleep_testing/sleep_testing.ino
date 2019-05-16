#include <Adafruit_SleepyDog.h>
//#include <arm/sleep.h>
#include <SPI.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize pins
  pinMode(LED_BUILTIN, OUTPUT);

  // set sleepy type (full standby I think?)
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  delay(20000);
  
}


void blinky(byte MS, byte loops) { // LED pin, delay in milliseconds, number of blinks
  for (byte i=0; i<loops; i++)  {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(MS);
    digitalWrite(LED_BUILTIN,LOW);
    delay(MS);
  }
}

void loop() {
  
  
  blinky(500, 10);
  delay(2000);

  blinky(500, 1);
//  int sleepMS = Watchdog.enable(8000);
  int cycles_elapsed = 0;
  int cycles = ceil(60 / 8);

  while (cycles_elapsed < cycles) {
    Watchdog.sleep(8000);
    cycles_elapsed ++;
  }
//  int sleepMS = Watchdog.sleep(8000);

  
//  __WFI();
  blinky(500, 2);
  delay(2000);
}
