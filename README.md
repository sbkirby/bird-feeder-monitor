# Bird Feeder Monitor - with Arduino Yun
![Bird Feeder Monitor](https://github.com/sbkirby/bird-feeder-monitor/blob/master/images/main-bird-feeder-image.jpg) 

This is a project to monitor the number of birds that visit my bird feeder, as well as record the amount of time spent feeding. I used an Arduino Yún and a capacitive touch sensor, Adafruit CAP1188, to detect and record the birds feeding. On a routine basis, the accumulated data is sent to a Google Docs spreadsheet to record the number and time spent by the birds visiting the feeder.

Data is only transmitted during a set period of time prior to sunrise and after sunset.

## Parts
![Project Enclosure](https://github.com/sbkirby/bird-feeder-monitor/blob/master/images/Project-enclosurest.jpg)

These are a list of the parts that I used to assemble my project. You could use a variety of project boxes to create your project, but these are the items I had on hand at the time.

1. 1 6x3x2" Project Enclosure
2. 1 3x2x1" Project Enclosure
3. 1 roll 1/4" [Copper Foil Tape](https://www.adafruit.com/product/1128)
4. 1 [CAP1188 8-Key Capacitive Touch Sensor](https://www.adafruit.com/product/1602)
5. 1 Arduino Yun
6. 1 micro-SD card
7. 2 DB-9 male connectors
8. 2 DB-9 female connectors
9. 1 Bird Feeder (CedarWorks Plastic Hopper Bird Feeder)
10. 1 Tail Light connector from automotive supply store

## Wiring
![Wiring Diagram](https://github.com/sbkirby/bird-feeder-monitor/blob/master/images/bird-feeder-project_bb.jpg)

The Arduino Yun and the CAP1188 are wired to provide a hard reset of sensor at startup. There are other capacitive touch sensors available with either one, five or eight sensors. I chose eight because my bird feeder has six sides.

### Wiring:

* CAP1188 SDA == Yún Digital 2 
* CAP1188 SCK == Yún Digital 3 
* CAP1188 RST == Yún Digital 9 
* CAP1188 VIN == Yún 3.3V or 5V 
* CAP1188 GND == Yún GND 
* CAP1188 C1-C8 == Connect to wires on each perch

Power for the Arduino was provided externally, by running a wire underground from my garage, and up through the pipe used as the bird feeder stand. The wire was connected to a 5-VDC power supply in the garage. This project should work with batteries, but I didn't want the hassle of changing batteries on a routine basis.

I constructed a 16" long cable with DB-9 connectors on both ends to connect the Project Box with the Arduino Yun and the box containing the CAP1188. The capacitive sensor needs to be located as close to the perches as possible.

## Installing Python Packages and Scripts
The CAP1188 requires that you download and install the libraries for this sensor. The library can be found at the following site:

[https://github.com/adafruit/Adafruit_CAP1188_Library/archive/master.zip](https://github.com/adafruit/Adafruit_CAP1188_Library/archive/master.zip)

Instructions for installing the library and examples are located in a README.txt file within the zip container.

This program monitors the sunrise & sunset for your specific location, and begins counting and timing at a specified time before sunrise and for an equal amount of time after sunset. Before and after that time, no data is sent to your spreadsheet. This project uses a python script to read the sunrise and sunset information from Yahoo! weather each evening or at startup get these times.

The following python library needs to downloaded and installed on the Arduino Yún.

### python-weather-api
pywapi - [https://code.google.com/p/python-weather-api/](https://code.google.com/p/python-weather-api/)

Instructions for installing this library is located at the above website.

### Python Scripts
The location ID in the 'getastonomy.py' python script needs to be modified to include your location. It is currently configured for Missouri City, Texas. One way to find your ID is to go to the following website:

[http://www.intellicast.com/](http://www.intellicast.com/)

Enter your location, and your location ID will appear as part of the URL address. Replace the USTX0905 in the line of the script with your location ID.

result = pywapi.get_weather_from_yahoo('USTX0905','imperial')

This will allow the script to fetch the sunrise and sunset for your location. Instructions for modifying 'sendgdocs.py' are located in Step 6.

Once both scripts have been modified you must move them onto the micro-SD card directory '/mnt/sda1/' of the Arduino Yun.
