#include "../embd.h"

void set_rgb( uint8_t r, uint8_t g, uint8_t b ) {

	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel( uint8_t pos ) {

	pos = 255 - pos;
	
	if (pos < 85)
		set_rgb(255 - pos * 3, 0, pos * 3);

	else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}

	else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}