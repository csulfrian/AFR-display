/*
  Program to display Air Fuel Ratio taken from an
  Innovate Motorsports LC-2 wideband O2 sensor analog output
  https://innovatemotorsports.com/products/lc2.php

  Displays on a SparkFun 7 segment serial LED display
  https://www.sparkfun.com/products/11629
  Implementing SparkFun serial 7 segment display elements

    Circuit:
    Arduino -------------- Serial 7-Segment
    5V   ----------------- VCC
    GND  ----------------- GND
    3   ------------------ RX

  Written by: Chris Sulfrian 2013
  http://github.com/csulfrian
*/

#include <SoftwareSerial.h>

#define rxPin 255 // 255 because we don't need a receive pin
#define txPin 3 // Connect the Arduino pin 3 to the rx pin on the 7 segment display

// set up a new serial port
SoftwareSerial LEDserial=SoftwareSerial(rxPin, txPin);

const int analogInPin = A0;  // Analog input pin that the LC1 analog out 2 (brown wire) is attached to

int afr = 0;
int lc2Rx = 0;

void setup() {
  // define pin mode for tx pin:
  pinMode(txPin, OUTPUT);
  LEDserial.begin(9600);
  Serial.begin(9600);
}

void loop() {

  // This segment takes the input from the LC1 and maps it to the output display
  lc2Rx = analogRead(analogInPin); // reads the analog in value         
  afr = map(lc2Rx, 0, 1023, 735, 2239); // maps the input to the range of the AFR x 10^2

  // This segment displays the result of the above calculation
  LEDserial.print("v"); // this is the reset display command
  delay(1); // allow a very short delay for display response
  LEDserial.print("w"); // send the command char for decimal control
  LEDserial.write(0x02); // turn on the decimal after the second digit

  // This segment determines if the output needs to be shifted to the right on the display to match up with the decimal point
  // if the value is less than 1000 (AFR = 10.00), shift everything to the right one place and blank the first digit
  if (afr < 1000) { 
    LEDserial.print("x");
    LEDserial.print(afr);
  } 
  // if the value is above 1000, display all four digits
  else {
    LEDserial.print(afr);
  }

  // This segment sends the two values to the serial monitor, for debugging purposes  
  Serial.print("LC1 signal = ");
  Serial.print(lc2Rx); 
  Serial.print("    AFR = ");
  Serial.println(afr);

  delay(80);   // wait for a ~1/12 second, (it took 4 msec to tx chars)
  digitalWrite(13, LOW);    // set the display off
  delay(5);   // wait for a 1/20 second
}
