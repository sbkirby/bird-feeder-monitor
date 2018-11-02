// Copyright (C) 2014  Stephen B. Kirby

/*************************************************** 
  This is a library for the CAP1188 I2C/SPI 8-chan Capacitive Sensor

  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
#include <Bridge.h>
#include <Console.h> 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

// Reset Pin is used for I2C or SPI - pin 9 on Arduino
#define CAP1188_RESET  9

// For I2C, connect SDA to your Arduino's SDA pin, SCL to SCL pin
// On UNO/Duemilanove/etc, SDA == Analog 4, SCL == Analog 5
// On Leonardo/Micro & YUN, SDA == Digital 2, SCL == Digital 3   <=== YUN
// On Mega/ADK/Due, SDA == Digital 20, SCL == Digital 21

// Use I2C, with reset pin
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

unsigned long birdTimer[6];    // temporary timer for each node
unsigned long birdResult[6];   // total time per node
int birdCnt[6];       // number of times node touched
int birdTouch[6];     // temporary flag indicating a touch for each node
unsigned long birdSend = 0;     // counter before sending data
// 20 minutes: totalCnt = 24000 and delaytime = 50
int delaytime = 50;  // delay time between checks (50)
unsigned long totalCnt = 24000;   // (minutes * 60 * 1000)/delaytime = totalCnt
boolean debug = false;
boolean transmit = true;
// minutes from midnight to Sunrise and Sunset
int sunrise = 0;
int sunset = 0;
// minutes prior to sunrise and after sunset
int minplusmin = 20;
// current time - minutes from midnight
int totalMinutes = 0;
// data string
String data = "";

Process p;
Process date;

void setup() {
  Bridge.begin();
  Console.begin();
  
  // wait
  delay(5000);
  
  reset();         // resets variables
  getAstronomy();  // get sunrise and sunset
  checkTime();     // check current time
  
  if (debug) {
    while (!Console); // wait for Console port to connect.
    Console.println("Bird Feeder Counter!");
  }

  if (debug) {
    // print main variables
    printVariables();
  }

  // Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (debug) {
    if (!cap.begin()) {
      Console.println("CAP1188 not found");
      while (1);
    }
    Console.println("CAP1188 found!");
  }
  else {
    cap.begin();
  }
  
  // run an initial date process. Should return:
  // hh:mm 
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%R");
    date.run();
  }
  
  // print current time hh:mm
  if (debug) {
    Console.print("Time Check = "); 
    while (date.available() > 0) {
      Console.println(date.readString());
    }
  }
    
}

void loop() {
  // check the CAP1188
  uint8_t touched = cap.touched();
  
  // if NOT TOUCHED
  if (touched == 0) {
    int j;
    for (j = 0; j < 6; j++) {
      // if TOUCHED previously add time to birdResult
      if (birdTouch[j]) {
        birdResult[j] = birdResult[j] + (millis() - birdTimer[j]);
        birdTimer[j] = millis();
      }
      birdTouch[j] = 0;
    }
    checkCounter();
    return;
  }
  
  // if TOUCHED check status of each capacitor
  for (uint8_t i=0; i<6; i++) {
    if (touched & (1 << i)) {
      // if NOT touched previously
      if (!birdTouch[i]) {
        birdTouch[i] = 1;
        birdCnt[i] = birdCnt[i] + 1;
        birdTimer[i] = millis();
      }
      // if TOUCHED previously
      else {
        birdResult[i] = birdResult[i] + (millis() - birdTimer[i]);
        birdTimer[i] = millis();
      }
    }
  }
  checkCounter();
}

/***************************************************/

// Count up to totalCnt and then send data to PushingBox
void checkCounter() {
  birdSend = birdSend + 1;
  
  if (birdSend > totalCnt) {
    if (debug) { 
      Console.println("Data Sent!");
    }    
    // check time to see if data can be transmitted
    checkTime();
    // if between sunrise and sunset then transmit data
    if ((totalMinutes > sunrise) & (totalMinutes < sunset)) {
      SendData(); // send data via PushBox
    }
    // update sunrise and sunset
    if ((totalMinutes > 60) & (totalMinutes < 120)) {
      getAstronomy();
    }
    // reset counter and variables
    birdSend = 0;
    reset();
  }
  else {
    if (debug & (birdSend % 100 == 0)) {      
      int j;
      for (j = 0; j < 6; j++) {
        Console.print("C"); 
        Console.print(j+1); 
        Console.print("\t");
        Console.print(birdCnt[j]);
        Console.print("\t");
        Console.print(birdResult[j]);
        Console.print("\t");
      }
      Console.println("");
    }
  }
  delay(delaytime);  // pause
  return;
}

/***************************************************/

// Reset bird counting array
void reset() {
    int j;
    for (j = 0; j < 6; j++) {
      birdTouch[j] = 0;
      birdCnt[j] = 0;
      birdTimer[j] = millis();
      birdResult[j] = 0;
    }
    return;  
}

/***************************************************/

// This method makes a HTTP connection to the server and sends data to PushingBox
void SendData() {
  
  data="";  
  int j;
  for (j = 0; j < 6; j++) {
    data +=" ";
    data += birdCnt[j];
    data +=" ";
    data += (birdResult[j]/1000);  // convert milli seconds to seconds
  }
  
  if (transmit) {
    p.runShellCommand("python /mnt/sda1/sendgdocs.py " + data); //sends the data via PushingBox
    while(p.running()); //wait till the process ends
  }
  // note the time that the connection was made:
  if (debug) {      
    Console.println("Data sent! " + data);
  }
}

/***************************************************/

// Get Sunrise and Sunset
void getAstronomy() {
  //
  String timeString = "";

  // Run python script to fetch sunrise and sunset times
  Process pr;		// Create a process and call it "p"
  pr.begin("python");	// Process that launch the "curl" command
  pr.addParameter("/mnt/sda1/getastronomy.py"); // Add the URL parameter to "curl"
  pr.run();		// Run the process and wait for its termination
  
  // A process output can be read with the stream methods
  while (pr.available() > 0) {
    timeString = pr.readString();
    // find the delimiters and length of string
    int UpArrow = timeString.indexOf("^");
    int StringEnd = timeString.length();

    // get the substrings for hour and minute:
    String sunriseString = timeString.substring(0, UpArrow);
    String sunsetString = timeString.substring(UpArrow + 1, StringEnd);
    
    // convert to integer
    sunrise = sunriseString.toInt() - minplusmin;
    sunset  = sunsetString.toInt() + minplusmin;    
  }
}

/***************************************************/

// Check time of day via number of minutes since midnight
void checkTime() {
  
  // initialize totalMinutes
  //totalMinutes = 0;
  
  // restart the date process:
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%R");
    date.run();
  }

  //if there's a result from the date process, parse it:
  while (date.available() > 0) {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();

    // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.length();

    // get the substrings for hour, minute second:
    String hourString = timeString.substring(0, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);

    // convert to ints, saving the previous second:
    int hours = hourString.toInt();
    int minutes = minString.toInt();
    totalMinutes = (hours * 60) + minutes;
  }
  
  if (debug) {
    // print main variables
    Console.println("************* checkTime() *************");
    printVariables();
  }
}

/***************************************************/

// Pring variables - used during debug mode
void printVariables() {
    // print main variables
    Console.print("Sunrise Sunset = ");
    Console.print(sunrise);
    Console.print(" ");
    Console.println(sunset);
    Console.print("totalMinutes = ");
    Console.println(totalMinutes);
    // Ensure the last bit of data is sent.
    Console.flush();
}
