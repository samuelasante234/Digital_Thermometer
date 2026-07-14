CC_DIR = ~/Documents/msp430-gcc
TARGET = msp430g2553
HEADER_PATHS = -I components/st7789_hal
OBJECT_FILES = st7789_hal.o
CFLAGS = -I $(CC_DIR)/include $(HEADER_PATHS) -mmcu=$(TARGET)
LFLAGS = -L $(CC_DIR)/include -T $(TARGET).ld
CC = $(CC_DIR)/bin/msp430-gcc
VPATH = components/st7789_hal
OBJECTS = main.o $(OBJECT_FILES)

firmware.elf: $(OBJECTS)
	$(CC) -o $@ $(LFLAGS) $(OBJECTS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@