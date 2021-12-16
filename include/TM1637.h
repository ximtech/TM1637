#pragma once

#include "main.h"
#include "DWT_Delay.h"
#include "Fonts/TM1637Font.h"

#ifndef TM1637_DIGITS
#define TM1637_DIGITS 4
#endif

#ifndef TM_PORT
#define TM_PERIPHERAL  LL_AHB1_GRP1_PERIPH_GPIOB
#define TM_PORT        GPIOB
#define TM_CLK_PIN     LL_GPIO_PIN_0
#define TM_DAT_PIN     LL_GPIO_PIN_1
#endif

// Parameters for setNumberPadTM1637()
#define TM_PAD_SPACE 0x00
#define TM_PAD_0 0x3F

// Control the TM1637 chip. This chip is usually attached
// to inexpensive 4 digit numeric displays. It's a generic
// 7-segment driver with key input capabilities though so
// it can be used for other I/O boards.
void initTM1637();
void clearTM1637();   // Clear the 7-segment displays (only)

void setByteTM1637(uint8_t position, uint8_t byte);   // Set a single 7-segment display to the given byte value. This allows direct control of the elements to do spinning animations etc.
void setDigitTM1637(uint8_t position, uint8_t digit); // Display a single digit at the given position. Position is left-to-right, starting with 0.
void setNumberTM1637(uint32_t number);                // Display an unsigned number.
void setNumberHexTM1637(uint32_t number, uint8_t offset, uint8_t width);// Display an unsigned number in hex format at a given offset it, with 0's or spaces to a desired with.
void setCharTM1637(uint8_t position, char ch);        // Draw a character at a given position. Not all characters are supported, check TM1637Font.h for an overview.
void setStringTM1637(const char *value);              // Display a string.
void scrollStringTM1637(const char *value, uint16_t scrollSpeed);            // Scroll characters (scrolls <--- left)
void setDotsTM1637(uint8_t mask);                     // Set which "dots" should be enabled. Mask is mapped right to left (ex. 0x01 = right-most dot)
void setBrightnessTM1637(uint8_t brightness);         // Set the brightness between 0 and 7

// Display an unsigned number at a given offset and pad it with 0's or
// spaces to a desired with. This function is helpful when the numbers can
// fluctuate in length (ex. 100, 5, 1000) to avoid flickering segments.
void setNumberPadTM1637(uint32_t number, uint8_t offset, uint8_t width);