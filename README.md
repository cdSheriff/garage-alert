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


-----------------OLD---------------
Have a battery powered device in the garage with sensors. Default is super sleep, wake every X minutes and listen for a short period, then go back to super sleep\
Have a wall powered unit that when activated, sends a signal every X seconds to request data\
Garage unit receives request, and send data back (sending costs energy, so sending would be a power expensive default). Goes back to super sleep\
Wall unit receives data and displays results\

------------NEW------------------
Have a battery powered unit downstairs with sensors\
Unit wakes every X minutes (currently 10), query sensors, send data\
Upstairs unit is always listening, on a X minute timeout (currently 15)\
Because radiohead loops until packet available, can just sit and chill until data is there\
Then read and parse data to display light code\

I guess I could reverse like od version, and have downstairs wait for an ask to then send a reply, but the 15mA vs 30mA listen/send difference seems so minimal for the sub-second query/send routine that I don't want to do the extra coding work. This also makes it easier to program a timeout upstairs\
