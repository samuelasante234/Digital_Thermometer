#ifndef TEMP_SENSOR_DRIVER_H
#define TEMP_SENSOR_DRIVER_H

#define ANALOG_INPUT BIT5

void temp_sensor_init();
void enable_enc();
void disable_enc();
void calculate_temp_readings(uint32_t* final_temp);
void convert_temp_reading_to_string(uint32_t final_temp, char* temp_string);

#endif