# Font Array

This component acts as the graphical lookup table for rendering text on the ST7789 display.

## Technical Details
Because the ST7789 is a pixel-based TFT (not a character LCD like the HD44780), it does not natively understand ASCII text. Every character must be drawn pixel by pixel. Hence, there is the need for an ascii to pixel character mapping using the array as lookup. 

* **2D Mapping**: This directory contains `ascii_8x16_font`, a massive 2D array. 
* **Dimensions**: Each character is mapped as an 8x16 grid (8 pixels wide, 16 pixels high).
* **Usage**: The ST7789 HAL uses the ASCII decimal value of a character (offset by 32, the space character) to index into this array, translating `1`s and `0`s into hexadecimal color codes (e.g., `0xFFFF` for white, `0x0000` for black).
