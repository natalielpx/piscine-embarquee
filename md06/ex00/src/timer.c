#include "../embd.h"

static inline uint8_t start_timer0( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:    return 0b001;
        case 8:    return 0b010;
        case 64:   return 0b011;
        case 256:  return 0b100;
        case 1024: return 0b101;
        default:   return 0;
    }
}

static inline uint8_t start_timer1( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:    return 0b001;
        case 8:    return 0b010;
        case 64:   return 0b011;
        case 256:  return 0b100;
        case 1024: return 0b101;
        default:   return 0;
    }
}

static inline uint8_t start_timer2( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:    return 0b001;
        case 8:    return 0b010;
        case 32:   return 0b011;
        case 64:   return 0b100;
        case 128:  return 0b101;
        case 256:  return 0b110;
        case 1024: return 0b111;
        default:   return 0;
    }
}

#define PRESCALER 256
#define TIMES_PER_SECOND 2
#define COMP_MATCH (F_CPU / PRESCALER / TIMES_PER_SECOND - 1)

void timers_init( void ) {

    // ==== Timer1 ====
    // CTC Mode;
    // TOP = OCR1A;
    // Updates OCR1x immediately;
    // TOV1 set on MAX
    TCCR1B |= (1 << WGM12);
	// Compare match
	OCR1A = COMP_MATCH;
    // Enable TIMER0_COMPA interrupt
	TIMSK1 |= (1 << OCIE1A);
    // Start timer
	TCCR1B |= start_timer1(PRESCALER);
}
