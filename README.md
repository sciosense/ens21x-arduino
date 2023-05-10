# ScioSense ENS21x 
Arduino library for the ENS21x temperature & humidity sensor family with I2C interface from ScioSense.

## Introduction
This project is an Arduino *library*. It implements a driver with examples for the ENS21x sensor family. ENS21x chips are digital temperature & humidity sensors with an I2C interface.

Note that the ENS21x requires a supply voltage of 1.71V .. 3.60V.

## Links
The ENS21x sensors are made by [ScioSense](http://www.sciosense.com).
 - In the library, an implementation for the ENS210 is given. The datasheet and further documents for this sensor can be downloaded here
   https://www.sciosense.com/products/relative-humidity-and-temperature-sensors/ens210-relative-humidity-and-temperature-sensor/
   
## Prerequisites
It is assumed that
 - The Arduino IDE has been installed.
   If not, refer to "Install the Arduino Desktop IDE" on the
   [Arduino site](https://www.arduino.cc/en/Guide/HomePage).
 - The library directory is at its default location.
   For me, that is `C:\Users\sciosense\Documents\Arduino\libraries`.

## Installation

### Installation via Arduino Library Manager
- In the Arduino IDE, navigate to the Arduino Library Manager on the left side (or, alternatively, select Sketch > Include Library > Manage Libraries...)
- Search for `ScioSense_ENS21x`
- Select the library from the search results and press `Install`

### Manual installation
- Download the code from this repo via Download ZIP.
- In Arduino IDE, select Sketch > Include Library > Add .ZIP library... and browse to the just downloaded ZIP file.
- When the IDE is ready this README.md should be located at e.g. `C:\Users\sciosense\Documents\Arduino\libraries\ScioSense_ENS21x\README.md`.

## Build an example
To build an example sketch
 - (Re)start Arduino.
 - Open File > Examples > Examples from Custom Libraries > ScioSense ENS21x > 01_Basic (or any other of the provided examples you wish to run)
 - Make sure Tools > Board lists the correct board.
 - Select Sketch > Verify/Compile.

## Acknowledgements
This library is developed for ScioSense by [at² GmbH](https://www.at2-software.com/en/) 

@at2software

 
### ScioSense is a Joint Venture of ams AG 