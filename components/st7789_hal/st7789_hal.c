#include "msp430g2553.h"
#include "st7789_hal.h"
#include "reg_bit_mapping.h"
#include <stdbool.h>
#include <string.h>
#include "font_array.h"

#define DELAY_IN_MS(x) __delay_cycles(16000 * (x))

#define RESET_FLAG 1                               //0 means reset->off, 1 means reset->ON

void initialise_spi_bus();
static void set_pins_for_spi();
static void set_spi_control_registers();
static void set_spi_clock_speed();
static void send_command(const unsigned char cmd);
static void send_data(unsigned char* parameters, unsigned char param_length);
void draw_characters(const char* text, uint16_t x, uint16_t y);
void st7789_wakeup();
static void st7789_set_window(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
void st7789_fill_area(uint16_t y, bool is_whole_screen);
void clear_screen();

void initialise_spi_bus() {
    set_spi_control_registers();
    set_pins_for_spi();
    set_spi_clock_speed();
    set_spi_control_registers();
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
    P2DIR |= DC | RES;                       //set pins as output
    P1SEL |= MOSI | SCK;                          //peripheral selection
    P1SEL2 |= MOSI | SCK;                         //peripheral selection
}
static void set_spi_clock_speed() {
    /*set prescaler to 2*/
    UCA0BR0 = BIT1;
    UCA0BR1 = 0;
}
void clear_screen() {
    st7789_fill_area(0,true);
    while (UCA0STAT & UCBUSY);
    P2OUT |= DC;
    for (uint16_t c=0;c<SCREEN_WIDTH;c++) {
        for (uint16_t i=0; i<SCREEN_HEIGHT;i++) {
            while (!(IFG2 & UCA0TXIFG));
            UCA0TXBUF=0x00;
            while (!(IFG2 & UCA0TXIFG));
            UCA0TXBUF=0X00;
        }
    }
}
void draw_characters(const char* text, uint16_t x, uint16_t y) {
    int no_of_characters = strlen(text),l;
    if (no_of_characters>30) no_of_characters=30;
    char padded_buffer[MAX_NO_OF_CHARACTERS+1];
    padded_buffer[MAX_NO_OF_CHARACTERS] ='\0';
    int character_cursor=x;
    for (l=0;(character_cursor-FONT_WIDTH)>=0;l++) character_cursor-=FONT_WIDTH,padded_buffer[l]=' ';
    if (character_cursor) padded_buffer[l]=' ',l++, x+=(FONT_WIDTH-character_cursor);
    for (unsigned char k=0;k<no_of_characters && (l<MAX_NO_OF_CHARACTERS);l++,k++) padded_buffer[l]=*(text+k);
    character_cursor=SCREEN_WIDTH-(x +(FONT_WIDTH*no_of_characters));
    for (;(character_cursor-FONT_WIDTH)>=0;l++) character_cursor-=FONT_WIDTH,padded_buffer[l]=' ';
    st7789_fill_area(y,false);
    while (UCA0STAT & UCBUSY);
    P2OUT |= DC;
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
static void send_command(const unsigned char cmd) {
    while (UCA0STAT & UCBUSY);
    P2OUT &= ~DC;
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF=cmd;
}
static void send_data(unsigned char* parameters, unsigned char param_length) {
    while (UCA0STAT & UCBUSY);
    P2OUT |= DC;
    for (unsigned char x=0; x<param_length; x++){
        while (!(IFG2 & UCA0TXIFG));
        UCA0TXBUF=parameters[x];
    }
}
void st7789_wakeup() {
    P2OUT |= RES;
    DELAY_IN_MS(10);
    P2OUT &= ~RES;
    DELAY_IN_MS(50);
    P2OUT |= RES;
    DELAY_IN_MS(120);
    send_command(0x01); //software reset
    DELAY_IN_MS(120);
    send_command(0x11); //sleep out
    DELAY_IN_MS(120);
    send_command(0x3A); //color mode
    unsigned char temp = 0x55;
    send_data(&temp, 1);
    send_command(0x36); //memory address control
    temp = 0x00;
    send_data(&temp, 1);
    send_command(0x21); //display inversion
    send_command(0x29); //display on
}
static void st7789_set_window(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye) {
    send_command(0x2A); //CASET
    uint8_t temp[4]={xs>>8, xs, xe>>8, xe};
    send_data(temp,4);
    send_command(0x2B); //RASET
    temp[0]=ys>>8, temp[1]=ys, temp[2]=ye>>8, temp[3]=ye;
    send_data(temp,4);
    send_command(0x2C); //RAMWR
}
void st7789_fill_area(uint16_t y, bool is_whole_screen) {
    uint16_t width=SCREEN_WIDTH, height;
    if (is_whole_screen) {
        height=SCREEN_HEIGHT;
        st7789_set_window(0,width-1, 0,height-1);
        return;
    }
    height=FONT_HEIGHT;
    if ((y+height) >SCREEN_HEIGHT) {
        height=SCREEN_HEIGHT, height -=y;
    }
    st7789_set_window(0,width-1, y, y + height-1);
}
