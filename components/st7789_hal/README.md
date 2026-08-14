# ST7789 Hardware Abstraction Layer (HAL)

This module provides the interface between the MSP430 and the ST7789 TFT display controller over a 3-wire SPI bus. Because there is no luxury of a DMA peripheral, transfer is strictly polling.

## Architecture
* **Hardware SPI**: Utilizes the MSP430's `USCI_A0` module configured in Master, Synchronous mode (SPI).
* **Clock Configuration**: Driven by SMCLK, with prescalers tightly configured to push pixels as fast as the hardware allows(16MHz default).
* **Control Pins**: Data/Command (`DC`) and Reset (`RES`) pins are bit-banged via standard GPIO (`P2OUT`).

## Core Functions
* `initialise_spi_bus()`: Sets up pin multiplexing and the USCI registers utilizing the custom `reg_bit_mapping`.
* `st7789_wakeup()`: Executes the strict power-on sequence required by the ST7789 (Software Reset -> Sleep Out -> Color Mode -> Display On).
* `st7789_set_window()`: Calculates the CASET (Column) and RASET (Row) addresses to define the bounding box for upcoming pixel data.
* `draw_characters()`: Parses a C-string, looks up the bitmap in the `font_array`, and pushes raw 16-bit color data into the SPI transmit buffer (`UCA0TXBUF`). It incorporates busy-wait loops on `TXIFG` to prevent buffer overflow.
