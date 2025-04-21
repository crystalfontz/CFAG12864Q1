# CFAG12864Q1-TFH and CFAG12864Q1-TMI Demo Code

This example Seeeduino (Arduino clone) code is for the Crystalfontz "Q" family of 128x64 graphical displays. These displays are transflective which means they can be read in sunlight with no backlight necessary as well as in dark environments with the backlight on.

For more information on our full list of graphical displays, please click [here](https://www.crystalfontz.com/c/graphic-lcd-displays/17).
For more information on our full list of sunlight readable displays, please click [here](https://www.crystalfontz.com/c/sunlight-readable-displays/38).

## Display information
Here are links to the raw display:\
[CFAG12864Q1-TFH](https://www.crystalfontz.com/product/CFAG12864Q1TFH) - dark pixels on light gray background\
[CFAG12864Q1-TMI](https://www.crystalfontz.com/product/CFAG12864Q1TMI) - light pixels on blue background

Here are links to the display + 10072 breakout board:\
[CFAG12864Q1-TFH-E1](https://www.crystalfontz.com/product/CFAG12864Q1TFHE1) - dark pixels on light gray background\
[CFAG12864Q1-TMI-E1](https://www.crystalfontz.com/product/CFAG12864Q1TMIE1) - light pixels on blue background

Here are links to the full dev kit. It is comprised of the display + 10072 breakout board + Seeeduino:\
[CFAG12864Q1-TFH-E1-2](https://www.crystalfontz.com/product/CFAG12864Q1TFHE12) - dark pixels on light gray background\
[CFAG12864Q1-TMI-E1-2](https://www.crystalfontz.com/product/CFAG12864Q1TMIE12) - light pixels on blue background


## Switching Interfaces
In the header section of the code, only one interface should be defined.\
To set the interface to **4-Wire SPI**, make sure that #define SPI_4W is not commented out\
To set the interface to **8080 Parallel**, make sure that #define PARALLEL_8080 is not commented out

## Connection Details
#### Connection details for the [CFAG12864Q1](https://www.crystalfontz.com/products/cfag12864q1tfh) 

  
| CFAG12864Q1<br>Pin  | Seeeduino<br>Pin SPI| Seeeduino<br>Pin 8080 | Seeeduino<br>Pin 6800 |       Connection<br>Description     |
|:-------------:|:--------:|:---------:|:--------:|------------------------------------|
| 1     (CSB)   | D10      | A0        | A0       | Chip Select                        |
| 2     (RSTB)  | A1       | A1        | A1       | Reset                              |
| 3     (A0)    | A3       | A3        | A3       | Data/Command                       |
| 4     (RW/R)  | DNC      | A2        | A2       | 6800 Read/Write H = Read L = Write<br>- or-<br>8080 Write|
| 5     (E/RD)  | DNC      | (RD) A4   | (E) A4   | Enable 6800<br>-or-<br>Read 8080         |
| 6-11  (D0-D5) | DNC      | D0-D5     | D0-D5    | Data Pins 0-5                      |
| 12    (D6)    | SCK      | D6        | D6       | Data Pin 6 / SCK                   |
| 13    (D7)    | MOSI     | D7        | D7       | Data Pin 7 / MOSI                  |
| 14    (VDD)   | 3.3V     | 3.3V      | 3.3V     | Power                              |
| 15    (GND)   | GND      | GND       | GND      | Ground                             |
| 16-20 (NC)    | DNC      | DNC       | DNC      | See schematic for more details     |
| 21    (V0)    | DNC      | DNC       | DNC      | LCD driving voltage - negative     |
| 22    (XV0)   | DNC      | DNC       | DNC      | LCD driving voltage - positive     |
| 23-24 (NC)    | DNC      | DNC       | DNC      | See schematic for more details     |
| 25    (VG)    | DNC      | DNC       | DNC      | LCD driving voltage - segments     |
| 26-30 (NC)    | DNC      | DNC       | DNC      | See schematic for more details     |
| 31    (C86)   | GND      | GND       | VDD      | Selects 8080 or 6800 in Parallel   |
| 32    (PSB)   | GND      | VDD       | VDD      | Selects SPI or Parallel            |
| 33-34 (NC)    | DNC      | DNC       | DNC      | See schematic for more details     |


The build files including the schematic can be downloaded 
    [on our site.](https://www.crystalfontz.com/products/document/3516/CFA_10072v0p1_BUILD_PACKAGE.zip) \
The controller datasheet can be found here: [Sitronix ST7567](https://www.crystalfontz.com/controllers/Sitronix/ST7567/)



## Additional Accessories
Additional accessories for the products can be found at the bottom of each of the product pages. 
