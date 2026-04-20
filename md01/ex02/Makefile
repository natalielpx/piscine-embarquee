######################################

# 	Piscine Embarquee: MD00 -- EX00

######################################

NAME	= main.hex

MMCU	= atmega328p
F_CPU	= 16000000UL
FORMAT	= ihex
PRGMMR	= arduino
CHIP	= m328p
PORT	= /dev/ttyUSB0
BAUD	= 115200
MMOP	= flash:w:$(NAME):i

######################################
# 	TOOLS
######################################

CC		= avr-gcc
OBJCPY	= avr-objcopy
DUDE	= avrdude

RM      = rm -f

######################################
# 	FLAGS
######################################

CFLAGS	= -Os -Wall -Wextra -Werror

######################################
# 	SOURCES/OBJECTS
######################################

SRC	= main.c
ELF	= main.elf

######################################
# 	RECIPES
######################################

all: hex flash

hex: $(ELF)
	$(OBJCPY) -O $(FORMAT) -R .eeprom $< $(NAME)

%.elf: %.c
	$(CC) -mmcu=$(MMCU) -DF_CPU=$(F_CPU) ${CFLAGS} $< -o $@

flash:
	$(DUDE) -c $(PRGMMR) -p $(CHIP) -P $(PORT) -b $(BAUD) -U $(MMOP)

clean:
	$(RM) $(ELF)
	$(RM) $(NAME)

re: clean all

.phony: all hex flash clean re
