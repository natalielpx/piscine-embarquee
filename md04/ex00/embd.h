#ifndef EMBD_H
# define EMBD_H

# include <avr/io.h>
# include <util/delay.h>

# define LED_D1 PB0
# define LED_D2 PB1
# define LED_D3 PB2
# define LED_D4 PB4

# define SWITCH_1 PD2
# define SWITCH_2 PD4

# define INPUT 0
# define OUTPUT 1

# define LOW 0
# define HIGH 1

# define FLOAT 0
# define PULL_UP 1

# define DISABLE 0
# define ENABLE 1

# define SET_FLAG 0
# define CLR_FLAG 1

# define READ_PIN(reg, bit) !!(reg & (1 << bit))
# define SET_PIN(reg, bit, val) (val ? (reg |= (1 << bit)) : (reg &= ~(1 << bit)))
# define TOGGLE_PIN(reg, bit) (reg ^= (1 << bit))

#endif
