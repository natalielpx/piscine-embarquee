#include "../embd.h"

#define TIMER1_PWM_TOP 31999
#define TIMES_PER_SECOND (F_CPU / (TIMER1_PWM_TOP + 1))
#define STEP ((TIMER1_PWM_TOP + 1) / (TIMES_PER_SECOND / 2))
// TIMES_PER_SECOND divided by 2 to account for both brightening and dimming

void setup() {

	// ==== Components ====
	// ---- LED D2 (Timer1 - OC1A) ----
	SET_PIN(DDRB, LED_D2, OUTPUT);

	// ==== Global Interrupt ====
	SREG |= (1 << 7);

	// ==== LED D2 (Timer1 - OC1A) ====
	// Fast PWM Mode; TOP = ICR1; Updates OCR1x at BOTTOM; TOV1 set on TOP
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << WGM11);
	ICR1 = TIMER1_PWM_TOP;
	// Compare match (to be changed by Timer0 interrupts)
	OCR1A = 0;
	// Clear OC1A on Compare Match
	// Set OC1A at BOTTOM
	TCCR1A |= (1 << COM1A1);
	// Start timer with prescaler /1
	TCCR1B |= TIM1_PRESCALER_1;

	// ==== Duty Cycle (Timer0) ====
	// Enable TIMER0_COMPA interrupt
	TIMSK0 |= (1 << OCIE0A);
	// CTC Mode; Top = OCRA; Updates OCRx immediately; TOV set on MAX
    TCCR0A |= (1 << WGM01);
	// Compare match
	OCR0A = F_CPU / 256 / TIMES_PER_SECOND - 1;
	// Start timer with prescaler /
	TCCR0B |= TIM0_PRESCALER_256;
}

void __attribute__(( signal, used, externally_visible )) TIMER0_OVF_vect( void ) {

	static int direction = 1;

	// Duty cycle reached 100% or 0%; Switch direction
	if (((direction == 1) && (OCR1A >= TIMER1_PWM_TOP)) ||
		((direction == -1) && (OCR1A < STEP)))
		direction = -direction;

	OCR1A += direction * STEP;
}


void __attribute__(( signal, used, externally_visible )) TIMER0_COMPA_vect( void ) {

	static volatile uint8_t count = 255;
	++count;
	if (count < 255)
		return;

	static int direction = 1;

	// Duty cycle reached 100% or 0%; Switch direction
	if (((direction == 1) && (OCR1A >= TIMER1_PWM_TOP)) ||
		((direction == -1) && (OCR1A < STEP)))
		direction = -direction;

	OCR1A += direction * STEP;
}

int main( void ) {

	setup();

	// Empty loop
	while (42);
}
