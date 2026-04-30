######################################

# 	Piscine Embarquee

######################################

NAME	= embd.hex

MMCU	= atmega328p
F_CPU	= 16000000UL
FORMAT	= ihex
PRGMMR	= arduino
CHIP	= m328p
PORT	= /dev/ttyUSB*
BAUD	= 115200
MMOP	= flash:w:$(NAME):i

######################################
# 	TOOLS
######################################

CC		= avr-gcc
OBJCPY	= avr-objcopy
DUDE	= avrdude

MKDIR	= mkdir -p
RM      = rm -f
RMDIR	= rm -rf

######################################
# 	FLAGS
######################################

CFLAGS	= -Os -Wall -Wextra -Werror

######################################
# 	SOURCES/OBJECTS
######################################

SRCS_DIR	:= src
SRCS		:= \
				main.c \
				uart.c \
				timer.c \
				i2c.c \
				aht20.c
OBJS_DIR	:= obj
OBJS 		:= $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
ELF			= embd.elf

######################################
# 	RECIPES
######################################

all: hex flash

hex: $(ELF)
	$(OBJCPY) -O $(FORMAT) -R .eeprom $< $(NAME)

$(ELF): ${OBJS}
	$(CC) -mmcu=$(MMCU) -DF_CPU=$(F_CPU) ${CFLAGS} $^ -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) -mmcu=$(MMCU) -DF_CPU=$(F_CPU) ${CFLAGS} -c $< -o $@

$(OBJS_DIR):
	$(MKDIR) $(OBJS_DIR)

flash:
	$(DUDE) -c $(PRGMMR) -p $(CHIP) -P $(PORT) -b $(BAUD) -U $(MMOP)

clean:
	$(RMDIR) $(OBJS_DIR)
	$(RM) $(ELF)
	$(RM) $(NAME)

re: clean all

.PHONY: all hex flash clean re
