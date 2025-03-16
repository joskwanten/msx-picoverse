# MSX PicoVerse - The MSX experience driven by the RaspBerry Pi Pico

The MSX PicoVerse is an open-source initiative aimed at developing multi-function cartridges for the MSX line of computers, utilizing variations of the Raspberry Pi Pico development boards with the RP2040 and RP2350 integrated chips. 

This project aspires to enhance the MSX experience by enabling users to load ROMs, connect to wireless networks, or even create new hardware through a software-defined approach.

The project is still in development, and we are looking for contributors to help us develop the hardware and software. If you are interested in contributing, please reach out.

> **Note:** At this moment there is no guarantee that the boards and or software will work as expected. This project is still a work in progress. If you decide to build the hardware, you do so at your own risk.

## Hardware

Hardware was designed in two classes, first covering the RP2040 chip variants and the second covering the RP2350B chip variants. The designs are based on the Raspberry Pico development boards, which are widely available and have a large community of developers.

### PicoVerse 2040 Cartridge

The RP2040 is a dual-core ARM Cortex-M0+ microcontroller operating originally at 133 MHz, featuring 264 KB of SRAM and support for external flash memory. It offers a variety of interfaces, including GPIO pins, SPI, I²C, UART, ADCs, PWM channels, and USB support.

| Prototype PCB (front) | Prototype PCB (back) |
|---------|---------|
| ![Image 1](images/20241230_001854885_iOS.jpg) | ![Image 2](images/20241230_001901504_iOS.jpg) | 

PicoVerse 2040 cartridges were designed for development boards that expose 30 GPIO pins and are **NOT compatible** with standard Raspberry Pi Pico boards. 

The goal was to use only the GPIO pins available on the boards and avoid the use of IO pin expanders or other components that could increase the complexity (and cost) of the design. 

The PicoVerse 2040 hardware offers almost 16MB to load multiple ROM files into the cartridge flash memory and select which one to boot the MSX. The cartridge has a USB port that can be used to connect it to a PC with Windows or Linux and transfer the ROMs to the Pico flash memory.

A few boards were designed for the PicoVerse 2040 Cartridge. Most of them have the GPIO pins connected to the MSX bus through level shifters. The level shifters are used to convert the 3.3V signals from the Pico board to 5V signals that are used by the MSX bus. 

One of the boards was designed to have the majority of the GPIO pins connected directly to the MSX bus without level shifters. There is a debate about the safety of connecting the Pico GPIOs directly to 5V, as it can damage the board as it is not officially documented that the GPIO pins can be connected directly to 5V. So use the board with the level shifters if you are concerned about that. 

The boards highlighted below have the same features, the only difference is the presence of level shifters in the first one. They both use the [Purple RP2040 board](https://s.click.aliexpress.com/e/_oCiLj5D) that is available on AliExpress and exposes 30 GPIOs.

|Cartridge Design Files|BOM|Image|
|-----------------------|------------------|------------------|
|[PicoVerse 2040](multirom/hardware/ALIEXPRESS-RP2040-PURPLE/1.0)|[BOM](https://htmlpreview.github.io/?https://github.com/cristianoag/msx-picoverse/2040/hardware/ALIEXPRESS-RP2040-PURPLE/1.0/bom/ibom.html)|<img src="images/2025-01-20_21-15.png" width="200"/>|
|[PicoVerse 2040 5V](multirom/hardware/ALIEXPRESS-RP2040-PURPLE-5V/1.2)|[BOM](https://htmlpreview.github.io/?https://github.com/cristianoag/msx-picoverse/2040/hardware/ALIEXPRESS-RP2040-PURPLE-5V/1.2/bom/ibom.html)|<img src="images/2025-01-20_21-22.png" width="200"/>|

### PicoVerse 2350 Cartridge

The RP2350, introduced in August 2024, is a high-performance microcontroller featuring a dual-core, dual-architecture design with selectable Arm Cortex-M33 or Hazard3 RISC-V cores, operating originally at 150 MHz. It includes 520 KB of on-chip SRAM, supports up to 16 MB of external QSPI flash or PSRAM, and offers multiple communication interfaces (2× UART, 2× SPI, 2× I²C), 24 PWM channels, up to 8 ADC channels, 48 GPIO pins, USB 1.1 support, and 12 PIO state machines for flexible interfacing.


| Prototype PCB (front) | Prototype PCB (back) |
|---------|---------|
| ![Image 1](images/20250208_180923511_iOS.jpg) | ![Image 2](images/20250208_181032059_iOS.jpg) |

PicoVerse 2350 cartridges are based on development boards that expose 48 GPIO pins and are **NOT compatible** with conventional Raspberry Pi Pico 2 development boards, which typically expose only 26 GPIO pins. 

Expanding on the features of the PicoVerse 2040, the PicoVerse 2350 cartridge not only supports loading ROMs into flash memory for execution but also includes a microSD card slot. This allows users to utilize Nextor for loading and saving files to the microSD card.

Additionally, the hardware features an ESP8266 WiFi module for wireless network connectivity and an Adafruit UA-1334 I2C module for audio output.


|Cartridge Design Files|BOM|Image|
|-----------------------|------------------|------------------|
|TBI|||

## Software

The software for the PicoVerse cartridges is open source. **It is open source but not free software.** 

It is licensed under the [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/). You can use the software for personal use, but you cannot use it for commercial purposes. 

You can modify the software and re-publish it, but you must give appropriate credit to the original author and distribute your modifications under the same license. **DO NOT USE THE SOFTWARE FOR COMMERCIAL PURPOSES** without express authorization from the author.

#### PicoVerse 2040 Cartridge Software

As this was the first cartridge to be developed, there are a few software options available. You can check options with descriptions and download links in the [software](2040/software) folder.

The main software for the PicoVerse 2040 cartridge is [MultiRom](2040/software/multirom). This software offers the following features:

|Menu - Page1|Menu - Page2|
|---|---|
|![alt text](/images/Multirom1.png)|![alt text](/images/Multirom2.png)|

* A PC tool to collect all ROM files in the current folder and create a UF2 file that can be used to program the PICO board.
* When the cartridge is inserted into the MSX computer, it offers a menu to select the ROM to boot.
* Support for loading ROMs from the cartridge flash up to 16MB in the following mapper formats:
  * Plain16  
  * Plain32  
  * Linear0  
  * Konami SCC  
  * Konami (without SCC)  
  * ASCII8  
  * ASCII16 
  * NEO-8 
  * NEO-16

More information about the software can be found in the [MultiRom](2040/software/multirom) folder.


#### PicoVerse 2350 Cartridge Software

The software for the PicoVerse 2350 Multirom Cartridge is being developed. You can check options with descriptions and download links in the [software](2350/software) folder.

The main software for the PicoVerse 2350 cartridge is [MultiRom](2350/software/multirom). This software offers the following features:

|Main Menu|Nextor 1|Nextor 2|
|---|---|---|
|![alt text](/images/2025-03-16-13-28-04.png)|![alt text](/images/2025-03-16-13-31-58.png)|![alt text](/images/2025-03-16-13-31-32.png)|

* A PC tool to collect all ROM files in the current folder and create a UF2 file that can be used to program the PICO board.
* When the cartridge is inserted into the MSX computer, it offers a menu to select the ROM to boot.
* Nextor support for loading and saving files to the microSD card. Nextor is the first option available on the menu. You can use that option to run Nextor OS and SofaRun.
* Support for loading MSX ROMs up to 16MB directly from the cartridge flash in the following mapper formats:
  * Plain16  
  * Plain32  
  * Linear0  
  * Konami SCC  
  * Konami (without SCC)  
  * ASCII8  
  * ASCII16 
  * NEO-8 
  * NEO-16

More information about the software can be found in the [MultiRom](2350/software/multirom) folder.

## License 

![Open Hardware](images/ccans.png)

This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/).

* If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
* You may not use the material for commercial purposes.
* You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

**ATTENTION**

This project was made for the retro community and not for commercial purposes. So only retro hardware forums and individual people can build this project.

**THE SALE OF ANY PART OF THIS PROJECT WITHOUT EXPRESS AUTHORIZATION IS PROHIBITED!**