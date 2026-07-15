#include <stdint.h>
#include <stdio.h>

typedef union {
        struct __attribute__((packed)) {
            unsigned char bit0_UCSYNC :1;
            unsigned char bit1_2_UCMODE :2;
            unsigned char bit3_UCMST :1;
            unsigned char bit4_UC7BIT :1;
            unsigned char bit5_UCMSB :1;
            unsigned char bit6_UCCKPL :1;
            unsigned char bit7_UCCKPH :1;
        }UCA0CTL0_bits;
        unsigned char UCA0CTL0_reg;
    } UCA0CTL0_register;

typedef union {
        struct __attribute__((packed))  {
            unsigned char bit0_UCSWRST:1;
            unsigned char bit1_5_RESERVED:5;
            unsigned char bit6_7_UCSSELx:2;
        } UCA0CTL1_bits;
        unsigned char UCA0CTL1_reg;
    } UCA0CTL1_register;

typedef union {
        struct __attribute__((packed)) UCA0BR0_bits {
            unsigned char bit0_7_UCBRx:8;
        };
        unsigned char UCA0BR0_reg;
    } UCA0BR0_register;

typedef union {
        struct __attribute__((packed)) UCA0BR1_bits {
            unsigned char bit0_7_UCBRx:8;
        };
        unsigned char UCA0BR1_reg;
    } UCA0BR1_register;