# garage-alert
Garage door open sensor, RF comms to apartment, but submarine style red light light in the apartment

Making this repository to hold myself accountable so I actually build it

Look into LoRa (like the Dragino LoRa board I read about)
Also sparkfun weather shield for monitoring the temp and humidity down there during winter

Starting with Adafruit rfm69hcw\
powered by 500mAh battery\
shitty multimeter reads ~30mA send\
around 2mA when looping through watchdog\
Haven't achieved sleepydog's promised quiescence of 0.3mA when using sleepdog package\
SAMD21 chip should be able to achieve 5uA quiescence but board bricks with __WDI()\

Have a battery powered device in the garage with sensors. Default is super sleep, wake every X minutes and listen for a short period, then go back to super sleep\
Have a wall powered unit that when activated, sends a signal every X seconds to request data\
Garage unit receives request, and send data back (sending costs energy, so sending would be a power expensive default). Goes back to super sleep\
Wall unit receives data and displays results\
