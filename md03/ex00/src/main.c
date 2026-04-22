#include "../embd.h"

void setup( void ) {

	// Components
	SET_PIN(DDRD, LED_RGB_B, OUTPUT);		// Set PD3 to Output
	SET_PIN(DDRD, LED_RGB_R, OUTPUT);		// Set PD5 to Output
	SET_PIN(DDRD, LED_RGB_G, OUTPUT);		// Set PD6 to Output
}

void set_RGB( uint8_t clr ) {
 
	// Turn off all three RGB LEDs
	SET_PIN(PORTD, LED_RGB_R, LOW);
	SET_PIN(PORTD, LED_RGB_G, LOW);
	SET_PIN(PORTD, LED_RGB_B, LOW);

	// Light up desired colour
	SET_PIN(PORTD, clr, HIGH);
}

int main( void ) {

	setup();

	uint8_t rgb[3] = {LED_RGB_R, LED_RGB_G, LED_RGB_B};

	while (42) {

		for (int i = 0; i < 3; ++i) {
			set_RGB(rgb[i]);
			_delay_ms(1000);
		}
	}
}
