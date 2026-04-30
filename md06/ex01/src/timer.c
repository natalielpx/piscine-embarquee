#include "../embd.h"

static inline uint16_t timer0_clock_select_bits( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:    return 0b001;
        case 8:    return 0b010;
        case 64:   return 0b011;
        case 256:  return 0b100;
        case 1024: return 0b101;
        default:   return 0;
    }
}

inline void start_timer0( uint16_t prescaler ) {

	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	TCCR0B |= timer0_clock_select_bits(prescaler);
}


static inline uint16_t timer1_clock_select_bits( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:    return 0b001;
        case 8:    return 0b010;
        case 64:   return 0b011;
        case 256:  return 0b100;
        case 1024: return 0b101;
        default:   return 0;
    }
}

inline void start_timer1( uint16_t prescaler ) {
	
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    TCCR1B |= timer1_clock_select_bits(prescaler);
}

static inline uint16_t timer2_clock_select_bits( uint16_t prescaler ) {

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

inline void start_timer2( uint16_t prescaler ) {

	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    TCCR2B |= timer2_clock_select_bits(prescaler);
}

// Timer0: Every 8ms
#define PRESCALER_0 1024
#define TIMES_PER_SECOND_0 125
#define COMP_MATCH_0 (F_CPU / PRESCALER_0 / TIMES_PER_SECOND_0 - 1)

// Timer1: Every 500ms
#define PRESCALER_1 256
#define TIMES_PER_SECOND_1 2
#define COMP_MATCH_1 (F_CPU / PRESCALER_1 / TIMES_PER_SECOND_1 - 1)

void timers_init( void ) {

    // ==== Timer0 ====
    // CTC Mode;
    // TOP = OCR0A;
    // Updates OCR0x immediately;
    // TOV1 set on MAX
    TCCR0A |= (1 << WGM01);
	// Compare match
	OCR0A = COMP_MATCH_0;
    // Enable TIMER0_COMPA interrupt
	TIMSK0 |= (1 << OCIE0A);
    // Don't start timer

    // ==== Timer1 ====
    // CTC Mode;
    // TOP = OCR1A;
    // Updates OCR1x immediately;
    // TOV1 set on MAX
    TCCR1B |= (1 << WGM12);
	// Compare match
	OCR1A = COMP_MATCH_1;
    // Enable TIMER1_COMPA interrupt
	TIMSK1 |= (1 << OCIE1A);
    // Start timer
	start_timer1(PRESCALER_1);
}
