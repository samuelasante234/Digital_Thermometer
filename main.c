#include "msp430g2553.h"
#include "font_array/font_array.h"
#include "reg_bit_mapping/reg_bit_mapping.h"
#include "st7789_hal/st7789_hal.h"
#include "temp_sensor_driver/temp_sensor_driver.h"
#include <stdint.h>
#include <stdbool.h>

volatile bool sampling_done_flag=false;

static void __attribute__((naked, used, section(".crt_0042"))) disable_watchdog();
void enable_global_interrupt();
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR();

int main() {
    enable_global_interrupt();
    temp_sensor_init();
    initialise_spi_bus();
    st7789_wakeup();
    clear_screen();
    uint32_t temp_readings_numeric;
    char temp_in_string[26];
    while (1) {
        enable_enc();
        while (!sampling_done_flag);
        calculate_temp_readings(&temp_readings_numeric);
        convert_temp_reading_to_string(temp_readings_numeric,temp_in_string);
        draw_characters(temp_in_string,0,87);
        sampling_done_flag=false;
        DELAY_IN_MS(2000);
    }
    return 0;
}
static void __attribute__((naked, used, section(".crt_0042"))) disable_watchdog() {
    WDTCTL = WDTPW | WDTHOLD;
}
void enable_global_interrupt() {
    __enable_interrupt();
}
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR() {
    if (ADC10CTL0 & ADC10IFG) {
        disable_enc();
        sampling_done_flag=true;
    }
}