#include "embd.h"

int main( void ) {

	// Set OC1A to output
	SET_PIN(DDRB, LED_D2, OUTPUT);

	// Waveform Generation
	// WGM13 = 1
	// WGM12 = 0
	// WGM11 = 1
	// WGM10 = 0
	// Phase Correct PWM Mode
	// TOP = ICR1
	// Update of OCR1x at: TOP
	// TOV1 flag set on: BOTTOM
	// reference: datasheet pg134, pg141 table16-4
	TCCR1B |= (1 << WGM13);
	TCCR1A |= (1 << WGM11);

	// Output Behaviour
	// COM1x1 = 1
	// COM1x0 = 0
	// Clear OC1A/OC1B on Compare Match when upcounting
	// Set OC1A/OC1B on Compare Match when downcounting
	// reference: datasheet pg141 table16-3
	TCCR1A |= (1 << COM1A1); 

	// Pulse Width Modulation
	ICR1 = 7812;		// F_CPU / (2 * prescaler * TOP)
	OCR1A = ICR1 / 10;	// duty cycle: 10%
	
	// Start timer with prescaler /1024
	// Prescaler: Clock/1024
	// CS12 = 1 
	// CS11 = 0
	// CS10 = 1
	// reference: https://nerd-corner.com/arduino-timer-interrupts-how-to-program-arduino-registers/
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// Empty loop
	while (42);
}
