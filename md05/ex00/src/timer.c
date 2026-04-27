#include "../embd.h"

static inline uint8_t start_timer1( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:    return (1 << CS10);
        case 8:    return (1 << CS11);
        case 64:   return (1 << CS11) | (1 << CS10);
        case 256:  return (1 << CS12);
        case 1024: return (1 << CS12) | (1 << CS10);
        default:   return 0;
    }
}

#define PRESCALER 8
#define TIMES_PER_SECOND 50
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
