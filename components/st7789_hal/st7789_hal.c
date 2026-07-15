#include "msp430g2553.h"
#include "st7789_hal.h"
#include "reg_bit_mapping.h"

void initialise_spi_bus();

void initialise_spi_bus() {
    UCA0CTL1_register UCA0CTL1_register_field ={
        .UCA0CTL1_bits.bit0_UCSWRST=0b1,         //set USCI SPI to RESET
        .UCA0CTL1_bits.bit6_7_UCSSELx=0b10,       //set clock source=SMCLK
    };
    UCA0CTL1=UCA0CTL1_register_field.UCA0CTL1_reg;
    UCA0CTL0_register UCA0CTL0_register_field={
        .UCA0CTL0_bits.bit0_UCSYNC=0b1,          //set for synchronous mode
        .UCA0CTL0_bits.bit1_2_UCMODE=0b0,        //set for 3-pin SPI
        .UCA0CTL0_bits.bit3_UCMST=0b1,           //set for master mode
        .UCA0CTL0_bits.bit4_UC7BIT=0b0,          //set for 8-bit data length
        .UCA0CTL0_bits.bit5_UCMSB=0b1,           //set for MSB first
        .UCA0CTL0_bits.bit6_UCCKPL=0b0,          //set for inactive state LOW (CPOL=0)
        .UCA0CTL0_bits.bit7_UCCKPH=0b1,          //set for trailing edge (CPHA=1)
    };
    UCA0CTL0=UCA0CTL0_register_field.UCA0CTL0_reg;

}