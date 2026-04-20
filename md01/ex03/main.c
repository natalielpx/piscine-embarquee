#include "embd.h"

#define TOP 15624

void setup() {

	SET_PIN(DDRB, LED_D2, OUTPUT);		// Set OC1A to output

	SET_PIN(DDRD, SWITCH_1, INPUT);		// Set PD2 to input
	SET_PIN(PORTD, SWITCH_1, PULL_UP);	// Enable pull-up on PD2
	SET_PIN(DDRD, SWITCH_2, INPUT);		// Set PD4 to input
	SET_PIN(PORTD, SWITCH_2, PULL_UP);	// Enable pull-up on PD4

}

int main( void ) {

	setup();

	// Waveform Generation
	// WGM13 = 1
	// WGM12 = 1
	// WGM11 = 1
	// WGM10 = 0
	// FAST PWM Mode
	// TOP = ICR1
	// Update of OCR1x at: BOTTOM
	// TOV1 flag set on: TOP
	// reference: datasheet pg132, pg142 table16-4
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << WGM11);

	// Output Behaviour
	// COM1x1 = 1
	// COM1x0 = 0
	// Clear OC1A on Compare Match
	// Set OC1A at BOTTOM
	// reference: datasheet pg140 table16-2
	TCCR1A |= (1 << COM1A1); 

	// Pulse Width Modulation
	ICR1 = TOP;			// F_CPU / (prescaler * TOP)
	OCR1A = TOP / 2;	// duty cycle: 50%
	
	// Start timer with prescaler /1024
	// Prescaler: Clock/1024
	// CS12 = 1 
	// CS11 = 0
	// CS10 = 1
	// reference: https://nerd-corner.com/arduino-timer-interrupts-how-to-program-arduino-registers/
	TCCR1B |= (1 << CS12) | (1 << CS10);

	uint8_t index = 5;
	uint8_t sw1 = 0, sw2 = 0;

	// Duty Cycle Controls
	while (42) {

		// Switch 1
		// If Switch 1 not pressed, record sw1 as not pressed
		if (READ_PIN(PIND, SWITCH_1))
			sw1 = 0;
		// If Switch 1 pressed, increment and record sw1 as pressed
		else if (!sw1) {
			sw1 = 1;
			// Update duty cycle
			if (index < 10) {
				++index;
				OCR1A = ((uint32_t)TOP * index) / 10;
			}
		}
		
		// Switch 2
		// If Switch 2 not pressed, record sw2 as not pressed
		if (READ_PIN(PIND, SWITCH_2))
			sw2 = 0;
		// If Switch 2 pressed, decrement and record sw2 as pressed
		else if(!sw2) {
			sw2 = 1;
			// Update duty cycle
			if (index > 0) {
				--index;
				OCR1A = ((uint32_t)TOP * index) / 10;
			}
		}

		// Debounce
		_delay_ms(30);
	}
}
