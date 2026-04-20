#include "embd.h"

int main( void ) {

	// Set OC1A to output
	SET_PIN(DDRB, LED_D2, OUTPUT);

	// Waveform Generation
	// WGM13 = 0
	// WGM12 = 1
	// WGM11 = 0
	// WGM10 = 0
	// Clear Timer on Compare match (CTC) mode
	// TOP = OCR1A
	// Update of OCR1x at: immediate
	// TOV1 flag set on: MAX
	// reference: datasheet pg131, pg141 table16-4
	TCCR1B |= (1 << WGM12);

	// Output Behaviour
	// COM1x1 = 0
	// COM1x0 = 1
	// Toggle OC1x on Compare Match 
	// reference: datasheet pg140 table16-1
	TCCR1A |= (1 << COM1A0); 
	
	// Pulse Width Modulation
	OCR1A = 31249;	// F_CPU / (2 * prescaler)
	
	// Start timer with prescaler /256
	// Prescaler: Clock/256
	// CS12 = 1 
	// CS11 = 0
	// CS10 = 0
	// reference: https://nerd-corner.com/arduino-timer-interrupts-how-to-program-arduino-registers/
	TCCR1B |= (1 << CS12);

	// Empty loop
	while (42);
}
