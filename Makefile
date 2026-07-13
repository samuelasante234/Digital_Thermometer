CC_DIR = ~/Documents/msp430-gcc
TARGET = msp430g2553
CFLAGS = -I $(CC_DIR)/include -mmcu=$(TARGET)
LFLAGS = -L $(CC_DIR)/include -T $(TARGET).ld
CC = $(CC_DIR)/bin/msp430-gcc
VPATH = components
OBJECTS = main.o

firmware.elf: $(OBJECTS)
	$(CC) -o $@ $(LFLAGS) $(OBJECTS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@