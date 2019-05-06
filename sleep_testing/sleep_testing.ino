#include <Adafruit_SleepyDog.h>
#include <SPI.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever

void sleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // select full powerdown. Only watchdog can wake

  power_adc_disable(); // turn off ADC too

  sleep_mode(); // this turns on sleep

  sleep_disable(); // turns off sleep
  power_all_enable();
}

void blinky() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
//  delay(500);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(4000);                       // wait for a second

  sleep()
}
