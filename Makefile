CC_DIR = ~/Documents/msp430-gcc
MCU = msp430g2553
HEADER_PATHS = -Icomponents/st7789_hal -Icomponents/font_array -Icomponents/reg_bit_mapping -Icomponents/temp_sensor_driver
OBJECT_FILES = st7789_hal.o temp_sensor_driver.o
CFLAGS = -I $(CC_DIR)/include $(HEADER_PATHS) -mmcu=$(MCU)
LFLAGS = -L $(CC_DIR)/include -T $(MCU).ld
CC = $(CC_DIR)/bin/msp430-gcc
VPATH = components/st7789_hal components/temp_sensor_driver components/reg_bit_mapping components/font_array
OBJECTS = main.o $(OBJECT_FILES)
IMAGE = firmware

$(IMAGE).elf: $(OBJECTS)
	$(CC) -o $@ $(LFLAGS) $(OBJECTS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

flash: $(IMAGE).elf
	mspdebug rf2500 "prog $(IMAGE).elf"