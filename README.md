# Bird Feeder Monitor - with Arduino Yun
![Bird Feeder Monitor](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/main-bird-feeder-image.jpg) 

This is a project to monitor the number of birds that visit my bird feeder, as well as record the amount of time spent feeding. I used an Arduino Yún and a capacitive touch sensor, Adafruit CAP1188, to detect and record the birds feeding. On a routine basis, the accumulated data is sent to a Google Docs spreadsheet to record the number and time spent by the birds visiting the feeder.

Data is only transmitted during a set period of time prior to sunrise and after sunset.

## Parts
![Project Enclosure](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/Project-enclosurest.jpg)

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
![Wiring Diagram](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/bird-feeder-project_bb.jpg)

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

```
result = pywapi.get_weather_from_yahoo('USTX0905','imperial')
```

This will allow the script to fetch the sunrise and sunset for your location. Instructions for modifying 'sendgdocs.py' are located in Step 6.

Once both scripts have been modified you must move them onto the micro-SD card directory '/mnt/sda1/' of the Arduino Yun.

## Wiring the Bird Feeder
![Top View](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/top-view-of-copper-tape-on-feeder.jpg)
![Bottom View](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/bird-feeder-bottom.jpg)

Each of the perches on the feeder was covered with 1/4" wide self adhesive copper foil tape. A small hole was drilled through the tape and perch, and a wire was soldered to the foil tape and routed beneath the feeder.

Note: With the bird feeder shown above, I recommend a gap between the ends of each foil stripe of 1 1/4" - 1 1/2". I discovered that the larger birds, such as grackles and doves, are capable of touching two foil strips at the same time if they are placed to close together.

A block of wood was shaped and glued to the bottom of the feeder to provide a level area to mount the Project Box containing the CAP1188. Velcro tape was applied to the Project Box as well as the wooden block to provide a means of attaching.

In order to refill the bird feeder, I unplug the power inside the garage. Next, I disconnect the DB-9 connector from the Project Box attached to the bottom of the feeder, which allows me to lift the feeder off the pipe with the Project Box still connected to the bottom. Once the feeder has been refilled, I place it back on the pipe stand; connect the DB-9 cable; and plug in the power.

## Creating a Google Doc Form
![Google Doc Form](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/Google-Docs.jpg)

In order to send data a Google Docs spreadsheet, you must first create a Form with all of the required fields. In my example, I have six 'cnt' fields and six 'time' fields that are integer input. For example, the fields are named 'cnt1', 'time1', 'cnt2', 'time2', etc. Once your finished with the Form, you click "View live form" to see the finished form. While viewing the Form, right click the page and select "View page source". Search and find all of the HTML "input" fields in the source code. Make a note of the name for each of the fields you entered on the form. This information is needed to create your Scenario in PushingBox.

## Configuring PushingBox
![PushingBox Scenario](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/PushingBox-Scenario.jpg)
![PushingBox Services](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/PushingBox-Services.jpg)

Make a note of the url address of the Form you created previously (while viewing the completed form), and copy that address. It should look similar to this address:
```
https://docs.google.com/forms/d/42QRHPzZzI4fdMZdC4...EbF8juE/viewform
```
This address is used to create your PushingBox Service, except that it must end with '/formResponse' instead of '/viewform'. Finally, be sure to change the Method used by the Service to POST.

Creating a Scenario in PushingBox will require the data gathered previously from the form for each of the input fields. Create a CustomURL type Scenario as seen in the photo above. It should look similar to this:
```
entry.184762354=$cnt1$&entry.1970438117=$ti...6352124=$cnt6$&entry.54370319=$time6$&&submit=Submit
```
Each entry should match the 'cnt' and 'time' fields of your form. End the string on fields with '&&submit=Submit' as seen above.

The Device ID created with your Scenario will be needed in the 'sendgdocs.py' python script in order to transmit data to Google Docs via PushingBox.

## The Data
![Graph Avg Count per Day](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/graph-average-time-per-count.jpg)
![Graph Total Count](https://github.com/sbkirby/bird_feeder_monitor_yun/blob/master/images/graph-total-count.jpg)

This program is currently configured to gather and send data to Google Docs every 20 minutes. That interval can be easily modified within the sketch

The data sent is the "count" of the number of times a bird (or some other object) touches the copper foil on the perch. It also sends the total amount of time (seconds) a bird touched the sensor while feeding.

I've experienced varying results. It all depends on the feed I'm providing, and the birds that are in the area. If the grackles are in the area, they can empty the bird feeder in sort order. They're able to scatter the feed with their beaks everywhere very quickly.

I have two bird feeders, but only one has the monitor attached. Therefore, my data indicates that I receive between 1,000 to 1,400 counts between refills, and the capacity of the feeder is 6 lbs. However, some of those counts are double counts as result of birds straddling more than one perch. In any case, it has been fun watching the feeder, and examining the data.

