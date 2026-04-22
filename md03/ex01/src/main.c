#include "../embd.h"

#define R 0
#define G 1
#define B 2

#define RED {1, 0, 0}
#define GREEN {0, 1, 0}
#define BLUE {0, 0, 1}
#define YELLOW {1, 1, 0}
#define CYAN {0, 1, 1}
#define MAGENTA {1, 0, 1}
#define WHITE {1, 1, 1}

void setup( void ) {

	// Components
	SET_PIN(DDRD, LED_RGB_B, OUTPUT);		// Set PD3 to Output
	SET_PIN(DDRD, LED_RGB_R, OUTPUT);		// Set PD5 to Output
	SET_PIN(DDRD, LED_RGB_G, OUTPUT);		// Set PD6 to Output
}

void set_RGB( uint8_t * clr ) {

	// Light up desired colour(s)
	SET_PIN(PORTD, LED_RGB_R, clr[R]);
	SET_PIN(PORTD, LED_RGB_G, clr[G]);
	SET_PIN(PORTD, LED_RGB_B, clr[B]);
}

int main( void ) {

	setup();

	uint8_t rgb[7][3] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};

	while (42) {

		for (int i = 0; i < 7; ++i) {
			set_RGB(rgb[i]);
			_delay_ms(1000);
		}
	}
}
