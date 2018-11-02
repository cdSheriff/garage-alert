# garage-alert
Garage door open sensor, RF comms to apartment, but submarine style red light light in the apartment

Making this repository to hold myself accountable so I actually build it

Look into LoRa (like the Dragino LoRa board I read about)
Also sparkfun weather shield for monitoring the temp and humidity down there during winter

Starting with Adafruit rfm69hcw 900
powered by 500mAh battery
25mA draw when listening, 20 hours w/o optimization
10mA when asleep, 50 hours with sleepy time
0.3mA when using sleepdog package https://github.com/adafruit/Adafruit_SleepyDog, 1600 hours if super asleep

Have a battery powered device in the garage with sensors. Default is super sleep, wake every X minutes and listen for a short period, then go back to super sleep /n
Have a wall powered unit that when activated, sends a signal every X seconds to request data
Garage unit receives request, and send data back (sending costs energy, so sending would be a power expensive default). Goes back to super sleep
Wall unit receives data and displays results
