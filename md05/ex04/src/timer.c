#include "../embd.h"

static inline uint8_t start_timer0( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:     return (1 << CS00);
        case 8:     return (1 << CS01);
        case 64:    return (1 << CS01) | (1 << CS00);
        case 256:   return (1 << CS02);
        case 1024:  return (1 << CS02) | (1 << CS00);
        default:    return 0;
    }
}

static inline uint8_t start_timer1( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:     return (1 << CS10);
        case 8:     return (1 << CS11);
        case 64:    return (1 << CS11) | (1 << CS10);
        case 256:   return (1 << CS12);
        case 1024:  return (1 << CS12) | (1 << CS10);
        default:    return 0;
    }
}

static inline uint8_t start_timer2( uint16_t prescaler ) {

    switch (prescaler) {
        case 1:     return (1 << CS20);
        case 8:     return (1 << CS21);
        case 32:    return (1 << CS21) | (1 << CS20);
        case 64:    return (1 << CS22);
        case 128:   return (1 << CS22) | (1 << CS20);
        case 256:   return (1 << CS22) | (1 << CS21);
        case 1024:  return (1 << CS22) | (1 << CS21) | (1 << CS20);
        default:    return 0;
    }
}

#define PRESCALER 8
#define TIMES_PER_SECOND 50
#define COMP_MATCH (F_CPU / PRESCALER / TIMES_PER_SECOND - 1)

void timers_init( void ) {

    // ==== LED RGB ====
    // -- Wave Generation Mode  --
    // ---------------------------
    // Fast PWM mode
    // TOP = 0xFF
    // Update of OCRx at: BOTTOM
    // TOV flag set on: TOP
	// ref: datasheet pg115 & pg164
	// ---------------------------
	// Timer0 (LED_R & LED_G)
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// Timer2 (LED_B)
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
	// -- Output Behaviour --
	// ----------------------
	// Clear OCnx on Compare Match
	// Set OCnx at BOTTOM
	// ref: datasheet pg113 table15-2, pg114 table15-6
	// ----------------------
	// Timer0 (LED_R & LED_G)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	// Timer2 (LED_B)
	TCCR2A |= (1 << COM2B1);
	// -- Start without prescaler --
    // Timer0 (LED_R & LED_G)
	TCCR0B |= start_timer0(1);
    // Timer2 (LED_B)
    TCCR2B |= start_timer2(1);
	// -----------------------------

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