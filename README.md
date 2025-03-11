# ScioSense ENS21x Arduino Library
Arduino library for the ENS21x temperature & humidity sensor family with I2C interface from ScioSense.

<img src="images/ens21x.png" width="400">

The ENS21x is a family of high-performance digital temperature and humidity sensors produced by
[ScioSense](http://www.sciosense.com). With industry leading accuracies down to 0.1°C temperature and 0.8% relative 
humidity, their rapid response and reliable, long-term performance the ENS21x family addresses the fields of home 
appliances, building and automotive HVAC, cold chain management, personal health and wellness monitoring, industrial 
automation and instrumentation.

The ENS21x family includes the ENS210, ENS211, ENS212, ENS213A and the ENS215.

## Links
* [Further information about the ENS21x](https://www.sciosense.com/ens21x-family-of-high-performance-digital-temperature-and-humidity-sensors/)
* [Datasheet](https://www.sciosense.com/wp-content/uploads/2024/04/ENS21x-Datasheet.pdf)
* Buy the ENS210 on [Mouser](https://mou.sr/3P3DWmK) or [Digikey](https://www.digikey.nl/en/products/detail/sciosense/ENS210-LQFM/6490747)
* Buy the ENS210 evaluation kit on [Mouser](https://mou.sr/44GNQAi)

 
## Prerequisites
It is assumed that
 - The Arduino IDE has been installed.
   If not, refer to "Install the Arduino Desktop IDE" on the
   [Arduino site](https://www.arduino.cc/en/Guide/HomePage).
- The library directory is at its default location. Normally this is `C:\Users\[your_username]\Documents\Arduino\libraries`.

You might need to add your board to the Arduino IDE. This library was tested with the [Espressif ESP32](https://www.espressif.com/en/products/socs/esp32). 
For the installation of the ESP32 in the Arduino IDE, see [Arduino ESP32 Installation](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)


## Installation

### Installation via Arduino Library Manager (coming soon)
- In the Arduino IDE, navigate to the Arduino Library Manager on the left side (or, alternatively, select Sketch > 
Include Library > Manage Libraries...)
- Search for `ScioSense_ENS21x`
- Select the library from the search results and press `Install`

### Manual installation
- Download the code from this repository via "Download ZIP".
- In Arduino IDE, select Sketch > Include Library > Add .ZIP library... and browse to the just downloaded ZIP file.
- When the IDE is ready this README.md should be located at `C:\Users\[your_username]\Documents\Arduino\libraries\ScioSense_ENS21x\README.md`.


## Wiring

### General
Please make sure that you use the correct supply voltage: The ENS21x runs at VDD = 1.71...3.60 V.

### Example with ESP32 (I2C)
This example shows how to wire a [ESP32DevKitC](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html#get-started-esp32-devkitc-board-front) 
with the ENS21x flex foil for I2C communication.

| ENS21x flex foil | ESP32 |
|:----------------:|:-----:|
|       VDD        |  3V3  |
|       GND        |  GND  |
|       SDA        |  G21  |
|       SCL        |  G22  |

<img src="images/i2c_pinout_esp32.png" width="1000">

## Build an example
To build an example sketch
 - (Re)start Arduino.
 - Open File > Examples > Examples from Custom Libraries > ScioSense_ENS21x > 01_Basic (or any other of the provided 
examples you wish to run)
 - Make sure Tools > Board lists the correct board.
 - Select Sketch > Verify/Compile.

## Contributing
Contributions in the form of issue opening or creating pull requests are very welcome!

## Acknowledgements
This library is developed for ScioSense by [at² GmbH](https://www.at2-software.com/en/) 

@at2software

### ScioSense is a Joint Venture of ams AG 
