//============================================================================
//
// Display is Crystalfontz CFAG12864Q1
//   https://www.crystalfontz.com/product/cfag12864q1tfh
//   https://www.crystalfontz.com/product/cfag12864q1tmi
//
// The controller is a Sitronix ST7567
//   https://www.crystalfontz.com/controllers/Sitronix/ST7567/
//
// Breakout board is Crystalfontz PCB 10072
//   
//| CFAG12864Q1   | Seeeduino| Seeeduino | Seeduino |       Connection                  |
//|     Pin       |  Pin SPI |  Pin 8080 | Pin 6800 |       Description                 |
//|---------------|----------|-----------|----------|-----------------------------------|
//| 1     (CSB)   | D10      | A0        | A0       | Chip Select                       |
//| 2     (RSTB)  | A1       | A1        | A1       | Reset                             |
//| 3     (A0)    | A3       | A3        | A3       | Data/Command                      |
//| 4     (RWR)   | DNC      | (WR) A2   | (RWR) A2 | Read/Write 6800 H = Read L = Write|
//|               |          |           |          | -or- Write 8080                   |
//| 5     (ERD)   | DNC      | (RD) A4   | (E) A4   | Enable 6800 -or- Read 8080        |
//| 6-11  (D0-D5) | D0-D5    | D0-D5     | D0-D5    | Data Pins 0-5                     |
//| 12    (D6)    | SCK      | D6        | D6       | Data Pin 6 / SCK                  |
//| 13    (D7)    | MOSI     | D7        | D7       | Data Pin 7 / MOSI                 |
//| 14    (VDD)   | 3.3V     | 3.3V      | 3.3V     | Power                             |
//| 15    (GND)   | GND      | GND       | GND      | Ground                            |
//| 16-20 (NC)    | DNC      | DNC       | DNC      | See schematic for more details    |
//| 21    (V0)    | DNC      | DNC       | DNC      | LCD driving voltage - negative    |
//| 22    (XV0)   | DNC      | DNC       | DNC      | LCD driving voltage - positive    |
//| 23-24 (NC)    | DNC      | DNC       | DNC      | See schematic for more details    |
//| 25    (VG)    | DNC      | DNC       | DNC      | LCD driving voltage - segments    |
//| 26-30 (NC)    | DNC      | DNC       | DNC      | See schematic for more details    |
//| 31    (C86)   | GND      | GND       | VDD      | Selects 8080 or 6800 in Parallel  |
//| 32    (PSB)   | GND      | VDD       | VDD      | Selects SPI or Parallel           |
//| 33-34 (NC)    | DNC      | DNC       | DNC      | See schematic for more details    |
//
//============================================================================
#include <avr/io.h>
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\SPI.cpp
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\SPI.h
#include <SPI.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
// This is supposed to be set somewhere in the AVR Studio, but I can't find
// the place :( In any case, our dev board, an Arduino UNO R3 runs at 16MHz,
// and this define makes the delays come out correctly.
#define F_CPU 16000000UL
//============================================================================
//
// *********SET INTERFACE*********
// Can be SPI or PARALLEL
//#define SPI_4W
//#define PARALLEL_8080
//#define PARALLEL_6800

#ifndef SPI_4W
 #ifndef PARALLEL_8080
  #ifndef PARALLEL_6800
   #error "You must define an interface"
  #endif
 #endif
#endif

#if defined(SPI_4W) & defined(PARALLEL_8080) & defined(PARALLEL_6800) || defined(SPI_4W) & defined(PARALLEL_8080) || defined(SPI_4W) & defined(PARALLEL_6800) || defined(PARALLEL_8080) & defined(PARALLEL_6800)
 #error "You can only define one interface"
#endif
// 
// 
// 
//============================================================================
#ifdef SPI_4W
// LCD SPI & backlight control lines
//   ARD   | Port | LCD
// #10/D10 |  PB2 | LCD_CS_NOT
// #11/D11 |  PB3 | LCD_MOSI (hardware SPI)
// #12/D12 |  PB4 | LCD_BL
// #13/D13 |  PB6 | LCD_SCK (hardware SPI)
#define CLR_CS    (PORTB &= ~(0x04))
#define SET_CS    (PORTB |=  (0x04))
#define CLR_BL    (PORTB &= ~(0x10))
#define SET_BL    (PORTB |=  (0x10))
//
// LCD control lines
//   ARD  | Port | LCD
// #15/A1 |  PC1 | LCD_RES_NOT
// #17/A3 |  PC3 | LCD_A0
//
#define CLR_RESET (PORTC &= ~(0x02))
#define SET_RESET (PORTC |=  (0x02))
#define CLR_A0    (PORTC &= ~(0x08))
#define SET_A0    (PORTC |=  (0x08))
#endif //SPI_4W

#ifdef PARALLEL_8080
// LCD Data is connected to port D
//   ARD  | Port | LCD
//  #0/D0 |  PD0 | LCD_D0
//  #1/D1 |  PD1 | LCD_D1 
//  #2/D2 |  PD2 | LCD_D2
//  #3/D3 |  PD3 | LCD_D3
//  #4/D4 |  PD4 | LCD_D4
//  #5/D5 |  PD5 | LCD_D5
//  #6/D6 |  PD6 | LCD_D6
//  #7/D7 |  PD7 | LCD_D7
//
#define LCD_DATA    (PORTD)
//
// LCD control lines
//   ARD  | Port | LCD
// #14/A0 |  PC0 | LCD_CS_NOT
// #15/A1 |  PC1 | LCD_RES_NOT
// #16/A2 |  PC2 | LCD_NOT_WR
// #17/A3 |  PC3 | LCD_A0
// #18/A4 |  PC4 | LCD_NOT_RD
//
#define CLR_CS    (PORTC &= ~(0x01))
#define SET_CS    (PORTC |=  (0x01))
#define CLR_RESET (PORTC &= ~(0x02))
#define SET_RESET (PORTC |=  (0x02))
#define CLR_WR    (PORTC &= ~(0x04))
#define SET_WR    (PORTC |=  (0x04))
#define CLR_A0    (PORTC &= ~(0x08))
#define SET_A0    (PORTC |=  (0x08))
#define CLR_RD    (PORTC &= ~(0x10))
#define SET_RD    (PORTC |=  (0x10))
//
// LCD backlight control
//   ARD  | Port | LCD
//  #9/D9 |  PB1 | LCD_BL
//
#define CLR_BL    (PORTB &= ~(0x02))
#define SET_BL    (PORTB |=  (0x02))
#endif //PARALLEL_8080

#ifdef PARALLEL_6800
// LCD Data is connected to port D
//   ARD  | Port | LCD
//  #0/D0 |  PD0 | LCD_D0
//  #1/D1 |  PD1 | LCD_D1 
//  #2/D2 |  PD2 | LCD_D2
//  #3/D3 |  PD3 | LCD_D3
//  #4/D4 |  PD4 | LCD_D4
//  #5/D5 |  PD5 | LCD_D5
//  #6/D6 |  PD6 | LCD_D6
//  #7/D7 |  PD7 | LCD_D7
//
#define LCD_DATA    (PORTD)
//
// LCD control lines
//   ARD  | Port | LCD
// #14/A0 |  PC0 | LCD_CS_NOT
// #15/A1 |  PC1 | LCD_RES_NOT
// #16/A2 |  PC2 | LCD_NOT_WR
// #17/A3 |  PC3 | LCD_A0
// #18/A4 |  PC4 | LCD_NOT_RD
//
#define CLR_CS    (PORTC &= ~(0x01))
#define SET_CS    (PORTC |=  (0x01))
#define CLR_RESET (PORTC &= ~(0x02))
#define SET_RESET (PORTC |=  (0x02))
#define CLR_RWR    (PORTC &= ~(0x04))
#define SET_RWR    (PORTC |=  (0x04))
#define CLR_A0    (PORTC &= ~(0x08))
#define SET_A0    (PORTC |=  (0x08))
//#define CLR_E    (PORTC &= ~(0x10))
#define SET_E    (PORTC |=  (0x10))
//
// LCD backlight control
//   ARD  | Port | LCD
//  #9/D9 |  PB1 | LCD_BL
//
#define CLR_BL    (PORTB &= ~(0x02))
#define SET_BL    (PORTB |=  (0x02))


#endif

//
//============================================================================
// Graphic data to display on LCD.
// Image2Code may be helpful to create your own screen image:
//    https://forum.crystalfontz.com/showthread.php/5854
//    http://sourceforge.net/p/image2code/code/HEAD/tree/
const char cfao12864d3_logo[8][128] PROGMEM = 
{
  {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x80,0x00,0xC0,0x00,
  0x00,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0xC0,0x40,0x40,0x40,
  0x40,0x00,0xC0,0x40,0x40,0x40,0x40,0x00,0x00,0x00,0x00,0x80,0x40,0x40,
  0x40,0x80,0x00,0xC0,0x00,0x00,0xC0,0x00,0x80,0x40,0x40,0x40,0x80,0x00,
  0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x40,0x00,
  0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00
  },

  {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,
  0xFE,0x07,0x03,0x01,0x00,0x00,0x01,0x03,0x07,0x8E,0xC4,0xE0,0xF0,0xF8,
  0xFC,0xFE,0xFF,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x01,0x03,0xFF,0xFF,
  0xFE,0xFC,0xF8,0xF0,0xC0,0x00,0x0C,0x12,0x12,0x12,0x09,0x00,0x1F,0x08,
  0x04,0x08,0x1F,0x00,0x0F,0x11,0x11,0x11,0x0F,0x00,0x1F,0x00,0x00,0x00,
  0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x0A,0x12,
  0x12,0x03,0x00,0x06,0x01,0x01,0x06,0x00,0x0D,0x12,0x12,0x12,0x0D,0x00,
  0x00,0x00,0x10,0x10,0x1F,0x10,0x10,0x00,0x1F,0x12,0x12,0x12,0x10,0x00,
  0x98,0x85,0x82,0x05,0x18,0x00,0x10,0x10,0x1F,0x10,0x10,0x00,0x00,0x00,
  0x00,0x00
  },
  
  {
  0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,
  0xF9,0xF8,0xF8,0xF8,0xF8,0xF8,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0x00,0x00,0x00,0x98,0x98,0x98,0x98,0x98,0x98,0x99,0x99,
  0x99,0x99,0x99,0x99,0x99,0x98,0x98,0x98,0x98,0x98,0x98,0x98,0x00,0x08,
  0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,
  0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x80,0xC8,0xE0,0x70,0x38,0x1C,
  0x0E,0x06,0x06,0x0E,0x3C,0xF8,0xE0,0x08,0x00,0x00,0x08,0x00,0x00,0x08,
  0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x80,0xE0,0x78,0x1E,0x07,
  0x09,0x00,0x01,0x0B,0x07,0x1C,0xF8,0xC0,0x00,0x08,0x00,0x00,0x00,0x00,
  0x00,0x00},

  {
  0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0x00,0x00,0x00,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
  0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x00,0x38,
  0x78,0xD8,0xD8,0x98,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
  0x18,0x98,0xD8,0xF8,0x78,0x3C,0x1E,0x1F,0x1B,0x19,0x18,0x18,0x18,0x18,
  0x18,0x18,0x18,0x18,0x18,0x18,0x1B,0x1F,0x3C,0xF8,0xD8,0x18,0x18,0x18,
  0x18,0x18,0x18,0x18,0x18,0x18,0x98,0xF8,0x7E,0x1F,0x19,0x18,0x18,0x18,
  0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1B,0x1F,0x1C,0x18,0x00,0x00,0x00,
  0x00,0x00
  },

  {
  0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,
  0x9F,0x1F,0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0x00,0x00,0x00,0x19,0x19,0x19,0x19,0x19,0x19,0x99,0x99,
  0x99,0x99,0x99,0x99,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x00,0x10,
  0x00,0x00,0x11,0x03,0x07,0x1E,0x0C,0x18,0x18,0x18,0x18,0x1C,0x0C,0x0E,
  0x17,0x03,0x01,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,
  0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x17,0x0E,0x38,0xF0,
  0xC0,0xC0,0xD0,0x60,0x30,0x1C,0x07,0x01,0x10,0x00,0x00,0x10,0x00,0x00,
  0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x00,0x00,
  0x00,0x00},

  {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,
  0x7F,0xE0,0xC0,0x80,0x00,0x00,0x80,0xC0,0xE0,0x73,0x27,0x0F,0x1F,0x3F,
  0x7F,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x3F,0x1F,
  0x0F,0x07,0xFB,0xFD,0x06,0x02,0x02,0x06,0x0C,0x00,0x00,0xFE,0xFE,0x40,
  0x40,0x40,0x40,0x00,0x06,0x3E,0xF8,0x88,0x88,0xF8,0x3E,0x06,0x00,0xF8,
  0xFC,0x06,0x02,0x22,0x22,0x3C,0x3E,0x00,0x00,0x00,0x00,0xFE,0xFE,0x00,
  0x00,0x00,0x00,0x0E,0x1E,0x32,0x62,0xC2,0x82,0x00,0x00,0xBC,0xFE,0x42,
  0x42,0xFE,0xBC,0x00,0x00,0xFC,0xFE,0x42,0x42,0x7E,0x3C,0x00,0x00,0x18,
  0x78,0xE8,0x88,0xFE,0xFE,0x00,0x00,0xF8,0xFC,0x06,0x02,0x0A,0x0E,0xFE,
  0xFA,0x00
  },

  {
  0xC0,0xE0,0x30,0x10,0x10,0x30,0x60,0x00,0x00,0xF0,0xF0,0x00,0x00,0x00,
  0x00,0xE0,0xF1,0x13,0x03,0x03,0x03,0x01,0xF0,0xF0,0x00,0x00,0x00,0x00,
  0x00,0x60,0x71,0x13,0x13,0x13,0xF3,0xE3,0x03,0x01,0x00,0x00,0xF0,0xF0,
  0x00,0x00,0x01,0x33,0xF6,0xC4,0x44,0x46,0xC3,0xF0,0x30,0x07,0xF7,0xF4,
  0x14,0x14,0x14,0x14,0x10,0x00,0xF1,0xF7,0x07,0x01,0x00,0x00,0x00,0x01,
  0xC3,0xE6,0x34,0x14,0x14,0x36,0xE3,0xC0,0x00,0x02,0xF2,0xF7,0x07,0x00,
  0xC0,0xE0,0xF0,0xF3,0x07,0x04,0x04,0x07,0xF3,0xF0,0x00,0x03,0x07,0x34,
  0x74,0xD7,0x93,0x10,0x10,0x13,0x17,0x14,0x04,0x06,0x02,0x00,0x00,0x00,
  0x00,0x01,0x03,0x07,0x07,0x00,0x00,0x01,0x03,0x06,0x04,0x04,0x06,0x03,
  0x01,0x00
  },

  {
  0x0F,0x1F,0x30,0x20,0x20,0x30,0x18,0x00,0x00,0x3F,0x3F,0x21,0x21,0x21,
  0x21,0x3F,0x1E,0x00,0x30,0x38,0x0C,0x06,0x03,0x03,0x06,0x0C,0x38,0x30,
  0x00,0x1C,0x3E,0x22,0x23,0x21,0x39,0x18,0x00,0x20,0x20,0x20,0x3F,0x3F,
  0x20,0x20,0x20,0x00,0x01,0x0F,0x3C,0x3C,0x0F,0x01,0x00,0x00,0x3F,0x3F,
  0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x22,0x22,0x22,0x22,0x20,0x00,
  0x0F,0x1F,0x30,0x20,0x20,0x30,0x1F,0x0F,0x00,0x00,0x3F,0x3F,0x1C,0x0F,
  0x03,0x00,0x3F,0x3F,0x00,0x20,0x20,0x20,0x3F,0x3F,0x20,0x20,0x20,0x20,
  0x20,0x20,0x21,0x23,0x26,0x2C,0x38,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00
  }
};

//============================================================================

#ifdef SPI_4W
void lcd_cmd_send(uint8_t data)
  {
  // Select the LCD's command register (~125nS setup)
  CLR_A0;
  //  send in the address and value via SPI:
  SPI.transfer(data);
  }
//----------------------------------------------------------------------------
void lcd_data_send(uint8_t data)
  {
  // Select the LCD's data register (~125nS setup)
  SET_A0;
  //  send in the address and value via SPI:
  SPI.transfer(data);
  }
#endif //SPI_4W


#ifdef PARALLEL_8080
void lcd_cmd_send(uint8_t data)
  {
  // Press the data onto the port (~250nS setup)
  LCD_DATA = data;
  // Select the LCD's command register (~125nS setup)
  CLR_A0;
  // Make a low pulse on WR to clock the data (~125nS pulse width)
  CLR_WR;
  SET_WR;
  }
//----------------------------------------------------------------------------
void lcd_data_send(uint8_t data)
  {
  // Press the data onto the port (~250nS setup)
  LCD_DATA = data;
  // Select the LCD's data register (~125nS setup)
  SET_A0;
  // Make a low pulse on WR to clock the data (~125nS pulse width)
  CLR_WR;
  SET_WR;
  }
#endif //PARALLEL_8080

#ifdef PARALLEL_6800
void lcd_cmd_send(uint8_t data)
  {
  // Press the data onto the port (~250nS setup)
  LCD_DATA = data;
  // Select the LCD's command register (~125nS setup)
  CLR_A0;
  // Make a low pulse on RWR to clock the data (~125nS pulse width)
  CLR_RWR;
  SET_RWR;
  }
//----------------------------------------------------------------------------
void lcd_data_send(uint8_t data)
  {
  // Press the data onto the port (~250nS setup)
  LCD_DATA = data;
  // Select the LCD's data register (~125nS setup)
  SET_A0;
  // Make a low pulse on RWR to clock the data (~125nS pulse width)
  CLR_RWR;
  SET_RWR;
  }
#endif //PARALLEL_6800


//----------------------------------------------------------------------------
void lcd_page_address_set(uint8_t page)
  {
  lcd_cmd_send(0b10110000 | (page & 0b1111));
  }
//----------------------------------------------------------------------------
void lcd_column_address_set(uint8_t column)
  {
  lcd_cmd_send(0b00010000 | (column >> 4));
  lcd_cmd_send(0b00000000 | (column & 0b1111));
  }
//----------------------------------------------------------------------------
// Sets the contrast, 0 = light, 63 = dark, 31 = about right.
void lcd_set_contrast(uint8_t contrast)
  {
  // The Electronic Volume (Double Byte Command)
  // This is the command to set the contrast.
  // The range that this command can control is determined
  // by the resistor ratio command immediately above.
  lcd_cmd_send(0x81);  //The Electronic Volume Mode Set
  lcd_cmd_send(contrast & 0x3F);    //Electronic Volume Register Set
  }
//----------------------------------------------------------------------------
void lcd_fill(uint8_t pattern)
  {
  uint8_t
    page;
  uint8_t
     col;

  for (page = 0; page < 9; ++page)
    {
    lcd_page_address_set(page);
    lcd_column_address_set(0);
    for (col = 0; col < 128; ++col)
      {
      lcd_data_send(pattern);
      }
    }
  }
//----------------------------------------------------------------------------
void lcd_init()
  {
  //For this demonstration, the display can just be selected forever.
  //If your hardware shares the data bus (nothing wrong with that) then
  //you will want to manage the CS line.
  CLR_CS;
  //For this simple demonstration code, we never read the LCD, so we
  //will just park the read line high. If you want to read the LCD, you
  //would want to control the RD line.
  //
  // This initialization code is based on the ST7565P Ver2.1a datasheet, the
  // section titled:
  //         "2. When the built-in power is not being used
  //          immediately after turning on the power"
  // Page 53 of:
  //   http://www.crystalfontz.com/controllers/Sitronix/ST7565P/
  //Hardware reset pulse
  CLR_RESET;
  //"Wait for the power stabilized"
  _delay_ms(50);
  SET_RESET;
  //Wait for reset circuit finished (<1ms)
  _delay_ms(1);
  //The display is now initialized with its power-on defaults.
  // Enter Power Save (Compound Command)
  //  1: Display Off 0xAE
  //  2: Display All Points ON 0xA5
  // Now we are in sleep mode
  lcd_cmd_send(0xAE);
  lcd_cmd_send(0xA5);
  // LCD bias setting
  //  0xA2 = 1/9 bias @ 1/65 duty
  //  0xA3 = 1/7 bias @ 1/65 duty
  lcd_cmd_send(0xA2);
  // ADC Select (Segment Driver Direction Select)
  // (flips display R<>L)
  //  0xA0 = Normal (for our LCD glass layout)
  //  0xA1 = Reverse (for our LCD glass layout)
  lcd_cmd_send(0xA0);
  // Common Output Mode Select
  // (flips display T<>B)
  //  0xC0 = Normal (for our LCD glass layout)
  //  0xC8 = Reverse (for our LCD glass layout)
  lcd_cmd_send(0xC0);
  // V0 Voltage Regulator Internal Resistor Ratio Set
  // (essentially the gain of the internal op-amp, you can think
  //  of it as setting the range over which the contrast can be
  //  adjusted.)
  //  0x20 = Lower voltage, lighter contrast
  //   . . .
  //  0x27 = Higher voltage, darker contrast
  //
  // A setting of 0x26 seems about right for this LCD
  lcd_cmd_send(0x26);
  // The Electronic Volume (Double Byte Command)
  // This is the command to set the contrast. For our LCD
  // and a resistor ratio of 6, a setting of 31 is just about
  // perfect.
  // The range that this command can control is determined
  // by the resistor ratio command immediately above.
  lcd_cmd_send(0x81);  //The Electronic Volume Mode Set
  lcd_cmd_send(31);    //Electronic Volume Register Set
  // The Booster Ratio (Double Byte Command)
  // This ratio much match the hardware capacitor configuration.
  // Our demo board uses 4x configuration.
  lcd_cmd_send(0xF8);  //Booster Ratio Select Mode Set
  lcd_cmd_send(0x00);  //Booster Ratio Register Set: 00 = 2x,3x,4x
  //Exit Power Save (Compound Command)
  // 1: Display On 0xAF
  // 2: Display All Points OFF 0xA4
  // Now we are not in sleep mode
  lcd_cmd_send(0xAF);
  lcd_cmd_send(0xA4);
  // Power Controller Set
  // This command must match the hardware. Our demo board
  // depends on Boost, regulator and follower all enabled.  
  lcd_cmd_send(0x2F);
  }
//============================================================================
void setup()
  {
#ifdef SPI_4W
  //Set the port directions
  // LCD SPI & backlight control lines
  //   ARD   | Port | LCD
  // #10/D10 |  PB2 | LCD_CS_NOT
  // #11/D11 |  PB3 | LCD_MOSI (hardware SPI)
  // #12/D12 |  PB4 | LCD_BL
  // #13/D13 |  PB6 | LCD_SCK (hardware SPI)
  DDRB |= 0x5C;
  
  //
  // LCD control lines
  //   ARD  | Port | LCD
  // #15/A1 |  PC1 | LCD_RES_NOT
  // #17/A3 |  PC3 | LCD_A0
  //
  DDRC |= 0x0A;
  
  //Drive the ports to a reasonable starting state.
  CLR_CS;
  SET_RESET;
  CLR_A0;
  
  
  // initialize SPI. By default the clock is 4MHz. The chip is good to 10 MHz
  SPI.begin();  
  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
#endif //SPI_4W

#ifdef PARALLEL_8080
  //Set the port directions
  // LCD Data is connected to port D
  //   ARD  | Port | LCD
  //  #0/D0 |  PD0 | LCD_D0
  //  #1/D1 |  PD1 | LCD_D1 
  //  #2/D2 |  PD2 | LCD_D2
  //  #3/D3 |  PD3 | LCD_D3
  //  #4/D4 |  PD4 | LCD_D4
  //  #5/D5 |  PD5 | LCD_D5
  //  #6/D6 |  PD6 | LCD_D6
  //  #7/D7 |  PD7 | LCD_D7
  // Set all pins of port D to outputs as default
  DDRD  = 0xFF;

  // LCD control lines
  //   ARD  | Port | LCD
  // #14/A0 |  PC0 | LCD_CS_NOT
  // #15/A1 |  PC1 | LCD_RES_NOT
  // #16/A2 |  PC2 | LCD_NOT_WR
  // #17/A3 |  PC3 | LCD_A0
  // #18/A4 |  PC4 | LCD_NOT_RD
  // Set used pins of port C to output
  DDRC |= 0x1F;
  
  // LCD backlight control
  //   ARD  | Port | LCD
  //  #9/D9 |  PB1 | LCD_BL
  // Set used pin of port B to output
  DDRB |= 0x02;
  
  //Drive the ports to a reasonable starting state.
  LCD_DATA=0;
  CLR_CS;
  SET_RESET;
  SET_WR;
  CLR_A0;
  SET_RD;
  #endif //PARALLEL_8080

  #ifdef PARALLEL_6800
  //Set the port directions
  // LCD Data is connected to port D
  //   ARD  | Port | LCD
  //  #0/D0 |  PD0 | LCD_D0
  //  #1/D1 |  PD1 | LCD_D1 
  //  #2/D2 |  PD2 | LCD_D2
  //  #3/D3 |  PD3 | LCD_D3
  //  #4/D4 |  PD4 | LCD_D4
  //  #5/D5 |  PD5 | LCD_D5
  //  #6/D6 |  PD6 | LCD_D6
  //  #7/D7 |  PD7 | LCD_D7
  // Set all pins of port D to outputs as default
  DDRD  = 0xFF;

  // LCD control lines
  //   ARD  | Port | LCD
  // #14/A0 |  PC0 | LCD_CS_NOT
  // #15/A1 |  PC1 | LCD_RES_NOT
  // #16/A2 |  PC2 | LCD_NOT_WR
  // #17/A3 |  PC3 | LCD_A0
  // #18/A4 |  PC4 | LCD_NOT_RD
  // Set used pins of port C to output
  DDRC |= 0x1F;
  
  // LCD backlight control
  //   ARD  | Port | LCD
  //  #9/D9 |  PB1 | LCD_BL
  // Set used pin of port B to output
  DDRB |= 0x02;
  
  //Drive the ports to a reasonable starting state.
  LCD_DATA=0;
  CLR_CS;
  SET_RESET;
  SET_RWR;
  CLR_A0;
  SET_E;
  #endif //PARALLEL_6800
  }
//============================================================================
void loop()
  {
  uint8_t
    i;
  uint8_t
    j;

  //Turn the backlight on
  SET_BL;

  //Initialize tle LCD controller
  lcd_init();
    
#if(0)
  //Send the image from the flash to the LCD.
  //This "inline" version takes about 2.8mS to fill the screen from the image in
  //flash, using 8MHz SPI transfers.
  //
  //The SPI clock is active less than half of the time, so if the SPI library were
  //to allow a "start sending SPI using hardware while we can continue in the
  //foreground" approach, there is probably another large gain available.
  for(j=0;j<8;j++)
    {
    // Point the controller to the correct page. This is the Y coordinate,
    // addressed by 8 pixel / 1 byte horizontal bands.
    lcd_page_address_set(j);
    //Reset the X coordinate to 0. Since the memory is 128 wide, this is 
    //not really needed, but it is still good practice.
    lcd_column_address_set(0);
    //Dump the data out for this line.
    // Select the LCD's data register
    SET_A0;
    for(i=0;i<128;i++)
      {
      //  send in the address and value via SPI:
      SPI.transfer(pgm_read_byte(&cfao12864d3_logo[j][i]));
      }
    }

#endif

  while(1)
    {
    //Logo
    //Send the image from the flash to the LCD.
    //This "normal" version takes about 3.1mS to fill the screen from the image in
    //flash, using 8MHz SPI transfers.
    for(j=0;j<8;j++)
      {
      // Point the controller to the correct page. This is the Y coordinate,
      // addressed by 8 pixel / 1 byte horizontal bands.
      lcd_page_address_set(j);
      //Reset the X coordinate to 0. Since the memory is 128 wide, this is 
      //not really needed, but it is still good practice.
      lcd_column_address_set(0);
      //Dump the data out for this line.
      for(i=0;i<128;i++)
        {
        lcd_data_send(pgm_read_byte(&cfao12864d3_logo[j][i]));
        }
      }
    _delay_ms(2500);

    //Clear
    lcd_fill(0x00);    
    _delay_ms(1000);
    
    //Filled
    lcd_fill(0xFF);    
    _delay_ms(1500);
    
    //Odd horizontal lines
    lcd_fill(0xAA);
    _delay_ms(1500);
    
    //Even horizontal lines
    lcd_fill(0x55);
    _delay_ms(1500);
    
    //Odd vertical lines
    for(j=0;j<8;j++)
      {
      // Point the controller to the correct page. This is the Y coordinate,
      // addressed by 8 pixel / 1 byte horizontal bands.
      lcd_page_address_set(j);
      //Reset the X coordinate to 0. Since the memory is 128 wide, this is 
      //not really needed, but it is still good practice.
      lcd_column_address_set(0);
      //Dump the data out for this line.
      for(i=0;i<128;i++)
        {
        lcd_data_send(i&0x01?0xFF:0x00);
        }
      }
    _delay_ms(1500);
    
    //Even vertical lines
    for(j=0;j<8;j++)
      {
      // Point the controller to the correct page. This is the Y coordinate,
      // addressed by 8 pixel / 1 byte horizontal bands.
      lcd_page_address_set(j);
      //Reset the X coordinate to 0. Since the memory is 128 wide, this is 
      //not really needed, but it is still good practice.
      lcd_column_address_set(0);
      //Dump the data out for this line.
      for(i=0;i<128;i++)
        {
        lcd_data_send(i&0x01?0x00:0xFF);
        }
      }
    _delay_ms(1500);
    
    //Odd check
    for(j=0;j<8;j++)
      {
      // Point the controller to the correct page. This is the Y coordinate,
      // addressed by 8 pixel / 1 byte horizontal bands.
      lcd_page_address_set(j);
      //Reset the X coordinate to 0. Since the memory is 128 wide, this is 
      //not really needed, but it is still good practice.
      lcd_column_address_set(0);
      //Dump the data out for this line.
      for(i=0;i<128;i++)
        {
        lcd_data_send(i&0x01?0xAA:0x55);
        }
      }
    _delay_ms(1500);
    
    //Even check
    for(j=0;j<8;j++)
      {
      // Point the controller to the correct page. This is the Y coordinate,
      // addressed by 8 pixel / 1 byte horizontal bands.
      lcd_page_address_set(j);
      //Reset the X coordinate to 0. Since the memory is 128 wide, this is 
      //not really needed, but it is still good practice.
      lcd_column_address_set(0);
      //Dump the data out for this line.
      for(i=0;i<128;i++)
        {
        lcd_data_send(i&0x01?0x55:0xAA);
        }
      }
    _delay_ms(1500);
    }


  //Wait here forever.
  while(1);
  } // void loop()
//============================================================================

