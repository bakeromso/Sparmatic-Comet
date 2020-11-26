TARGET=main
MCU=atmega169
SOURCES = main.c
SOURCES += include/lcd.c
SOURCES += include/motor.c

PROGRAMMER=arduino
PORT=-Pnet:192.168.0.127:328

#Ab hier nichts verändern
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-c -Os
LDFLAGS=

all: hex eeprom

hex: $(TARGET).hex

eeprom: $(TARGET)_eeprom.hex

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -j .data -j .text $(TARGET).elf $(TARGET).hex

$(TARGET)_eeprom.hex: $(TARGET).elf
	avr-objcopy -O ihex -j .eeprom --change-section-lma .eeprom=1 $(TARGET).elf $(TARGET)_eeprom.hex

$(TARGET).elf: $(OBJECTS)
	avr-gcc $(LDFLAGS) -mmcu=$(MCU) $(OBJECTS) -o $(TARGET).elf

.c.o:
	avr-gcc $(CFLAGS) -mmcu=$(MCU) $< -o $@

size:
	avr-size --mcu=$(MCU) -C $(TARGET).elf

program:
	avrdude -p$(MCU) $(PORT) -c$(PROGRAMMER) -Uflash:w:$(TARGET).hex:a

clean_tmp:
	rm -rf *.o
	rm -rf *.elf
	rm -rf include/*.o
	rm -rf include/*.elf

clean:
	rm -rf *.o
	rm -rf *.elf
	rm -rf *.hex
	rm -rf include/*.o
	rm -rf include/*.elf
	rm -rf include/*.hex
	
