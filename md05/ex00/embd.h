#ifndef EMBD_H
# define EMBD_H

# include <avr/io.h>
# include <util/delay.h>

# ifndef F_CPU
#  define F_CPU = 16000000UL
# endif

# define LED_D1 PB0
# define LED_D2 PB1
# define LED_D3 PB2
# define LED_D4 PB4

# define ADC_POT PC0
# define ADC_LDR PC1
# define ADC_NTC PC2

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

#define ADC_CH0 0x0000
#define ADC_CH1 0x0001
#define ADC_CH2 0x0010
#define ADC_CH3 0x0011
#define ADC_CH4 0x0100
#define ADC_CH5 0x0101
#define ADC_CH6 0x0110
#define ADC_CH7 0x0111
#define ADC_CH8 0x1000

# define READ_PIN(reg, bit) !!(reg & (1 << bit))
# define SET_PIN(reg, bit, val) (val ? (reg |= (1 << bit)) : (reg &= ~(1 << bit)))
# define TOGGLE_PIN(reg, bit) (reg ^= (1 << bit))

// ----- UART -----

void uart_init( void );
void uart_tx( char );
void uart_printstr( const char * );
char uart_rx( void );

// ----- TIMER -----
void timers_init( void );

#endif
