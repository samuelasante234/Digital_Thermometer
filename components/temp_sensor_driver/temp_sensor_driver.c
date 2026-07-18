#include "temp_sensor_driver.h"
#include "msp430g2553.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void temp_sensor_init();
void enable_enc();
void disable_enc();
void calculate_temp_readings(uint32_t* final_temp);

volatile uint16_t temp_readings[5];

volatile bool must_enable_enc=true;

void temp_sensor_init() {
    ADC10CTL0 |= 
    SREF0 | ADC10SHT1 | ADC10SR |     //SREF=011B, 16 ADC CLOCK CYCLES, 50ksps
    REFON | ADC10ON | ADC10IE; //reference on(1.5v), enable ADC10 and interrupts
    ADC10CTL1 |= 
    INCH_5 | SHS_0 | ADC10DIV_7 |     //Channel A5, ADC10SC as trigger, divisor of 8
    ADC10SSEL1 | CONSEQ1;             // use MCLK, repeat single-channel mode
    ADC10AE0 |= BIT5;                 //enable analog input A5
    ADC10DTC0 &= 0;                   //one-block transfer, non-continuous
    ADC10DTC1 = BIT2 | BIT0;          //5 DTC transfers per temperature reading
    ADC10SA = (uint16_t )temp_readings;         //address of temperature reading buffer
}
void enable_enc() {
    ADC10CTL0 |= ENC | ADC10SC;
}
void disable_enc() {
    ADC10CTL0 &= ~(ENC| ADC10SC);
}
void calculate_temp_readings(uint32_t* final_temp) {
    uint16_t average=0;
    for (unsigned char x=0; x<5;x++) {
        average += temp_readings[x];
    }
    average = (uint16_t)(average/5);
    *final_temp = ((uint32_t)average * 15000) /(1023);        //two decimal digits precision
}