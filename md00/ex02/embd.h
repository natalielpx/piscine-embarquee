#ifndef EMBD_H
# define EMBD_H

# include <avr/io.h>
# include <util/delay.h>

# define LED_D1 PB0
# define SWITCH_1 PD2

# define INPUT 0
# define OUTPUT 1

# define LOW 0
# define HIGH 1

# define FLOAT 0
# define PULL_UP 1

# define READ_PIN(reg, bit) !!(reg & (1 << bit))
# define SET_PIN(reg, bit, val) (val ? (reg |= (1 << bit)) : (reg &= ~(1 << bit)))

#endif
