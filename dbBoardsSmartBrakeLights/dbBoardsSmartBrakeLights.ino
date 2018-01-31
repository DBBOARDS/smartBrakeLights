/*-----------------------------------------------------------------------------------------------------------
dbBoardsSmartBrakeLights.ino

  Summary:
    This program reads the adxl345 sensor data and uses the z value to determine if the rider is applying
    the brakes on the bike. If the bike is slowing down, the brake lights (10 ws2812 RGB LEDs) change to
    full brightness to help keep themselves and others safe and aware.
  
  Utilizing:
    Sparkfun's ADXL345 Library https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library
    Adafruit NeoPixel Library: https://github.com/adafruit/Adafruit_NeoPixel
   
  Programmer:
    Duncan Brandt @ DB Boards, LLC
    Created: Jan 29, 2018
  
  Development Environment Specifics:
    Arduino 1.6.11
  
  Hardware Specifications:
    DB Boards SPI ADXL345, DB3000
    Arduino Drawing Board (UNO) DB1000
    WS2812 Strip (2 Connected in Series) DB7000

  Beerware License:
    This program is free, open source, and public domain. The program is distributed as is and is not
    guaranteed. However, if you like the code and find it useful I would happily take a beer should you 
    ever catch me at the local.
*///---------------------------------------------------------------------------------------------------------
#include <SparkFun_ADXL345.h>  // https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library
ADXL345 adxl = ADXL345(10);    // USE FOR SPI COMMUNICATION, ADXL345(chipSelectPin);
int x,y,z;                     // Variable used to store accelerometer data
int zBounce = 0;               // Used to keep the brake lights from flickering during braking

#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#define PIN 6                  // Pin 6 for communication with LED strip
// 10  Number of pixels in strip, NEO_GRB  Pixels are wired for GRB bitstream, NEO_KHZ800  800 KHz bitstream
Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, PIN, NEO_GRB + NEO_KHZ800);
//-----------------------------------------------------------------------------------------------------------
void setup(){                  // The setup Program runs one time at power up or after reset
  strip.begin();               // Activate the LED strand
  strip.show();                // Initialize all pixels to 'off'
  adxl.powerOn();              // Power on the ADXL345
  adxl.setRangeSetting(4);     // Range settings 2g(highest sensetivity), 4g, 8g or 16g(lowest sensetivity)
  adxl.setSpiBit(0);           // Configure the device to be in 4 wire SPI mode
}
//-----------------------------------------------------------------------------------------------------------
void loop(){                   // The loop program follows the setup program and repeats forever
  adxl.readAccel(&x, &y, &z);  // Read the accelerometer values and store them in x, y, and z
  if(zBounce < 2){             // If the brakes were on in the last two loops
    if(z < -20){               // If the brakes are applied now
      for(int i = 0; i < 10; i++){                    // For all ten LEDs
        strip.setPixelColor(i, strip.Color(255,0,0)); // Turn the LED to bright Red
      }
      strip.show();            // Show the new LEDs bright Red Brake Light
      delay(2000);             // Leave the brake lights on for 2 seconds before checking again
      zBounce = 0;             // Restart the flicker debounce clock
    }
    else{                      // If the brakes appear to be off
      zBounce++;               // Add one to the flicker debounce clock
    }
  }
  else{                        // If the brakes have been off for two loops
    for(int a = 0; a < 10 a++){// For all the LEDs on the strip         
      strip.setPixelColor(i, strip.Color(100,0,0));   // Turn one LED to light red
      a++;                     // Skip to the next LED
      strip.setPixelColor(i, strip.Color(0,0,0));     // Turn every other LED off
    }
    strip.show();              // Show the new LEDs light Red saftey light 
    delay(80);                 // Give the debounce time to work correctly
  }
}
//-----------------------------------------------------------------------------------------------------------
