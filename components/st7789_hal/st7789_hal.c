#include "msp430g2553.h"
#include "st7789_hal.h"
#include "reg_bit_mapping.h"
#include <stdbool.h>
#include "font_array.h"

#define RESET_FLAG 1                               //0 means reset->off, 1 means reset->ON

void initialise_spi_bus();
static void set_pins_for_spi();
static void set_spi_control_registers();
static void enable_spi_interrupts();
static void set_spi_clock_speed();
void draw_characters(const char* text, uint16_t x, uint16_t y);

void initialise_spi_bus() {
    set_spi_control_registers();
    set_pins_for_spi();
    set_spi_control_registers();
    enable_spi_interrupts();
    set_spi_clock_speed();
}
static void set_spi_control_registers() {
    static bool rst_flag =true;
    static UCA0CTL1_register UCA0CTL1_register_field={0};
    if(rst_flag) {
        UCA0CTL1_register_field.UCA0CTL1_bits.bit0_UCSWRST=0b1;          //set USCI SPI to RESET
        UCA0CTL1_register_field.UCA0CTL1_bits.bit6_7_UCSSELx=0b10;       //set clock source=SMCLK
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
        rst_flag=false;
    }
    else {
        /*set USCI SPI to disable RESET*/
        UCA0CTL1 &= 0xFE;
    }
}
static void set_pins_for_spi() {
    P1DIR |= MOSI | SCK;                          //set pins as output
    P2DIR |= CS | DC | RES;                       //set pins as output
    P1SEL |= MOSI | SCK;                          //peripheral selection
    P1SEL2 |= MOSI | SCK;                         //peripheral selection
}
static void enable_spi_interrupts() {
    IE2=UCA0TXIE;                                //enable SPI transmit interrupt
}
static void set_spi_clock_speed() {
    /*set prescaler to 2*/
    UCA0BR0 = BIT1;
    UCA0BR1 = 0;
}
void draw_characters(const char* text, uint16_t x, uint16_t y) {
    int no_of_characters = strlen(text),l;
    if (no_of_characters>30) no_of_characters=30;
    char padded_buffer[MAX_NO_OF_CHARACTERS+1];
    padded_buffer[MAX_NO_OF_CHARACTERS] ='\0';
    int left_right_x=x;
    for (l=0;(left_right_x-FONT_WIDTH)>=0;l++) left_right_x-=FONT_WIDTH,padded_buffer[l]=' ';
    if (left_right_x) padded_buffer[l]=' ',l++, x+=(FONT_WIDTH-left_right_x);
    for (unsigned char k=0;k<no_of_characters && (l<MAX_NO_OF_CHARACTERS);l++,k++) padded_buffer[l]=*(text+k);
    left_right_x=SCREEN_WIDTH-(x +(FONT_WIDTH*no_of_characters));
    for (;(left_right_x-FONT_WIDTH)>=0;l++) left_right_x-=FONT_WIDTH,padded_buffer[l]=' ';
    for (unsigned char c=0;c<16;c++) {
        for (unsigned char i=0; i<30;i++) {
            unsigned char temp = ascii_8x16_font[padded_buffer[i]-32][c];
            for (unsigned char bit_index=0; bit_index<8;bit_index++) {
                if (temp & (1<<(FONT_WIDTH-bit_index-1))){
                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF=0xFF;
                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF =0XFF;
                }
                else {
                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF=0x00;
                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF=0X00;
                }
            }
        }
    }
}