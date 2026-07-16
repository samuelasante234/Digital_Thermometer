#ifndef ST7789_HAL_H
#define ST7789_HAL_H

#include "msp430g2553.h"
#include <stdint.h>

#define MOSI BIT1
#define SCK BIT4
#define CS BIT0
#define DC BIT1
#define RES BIT2

#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define MAX_NO_OF_CHARACTERS 30
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

void initialise_spi_bus();

#endif