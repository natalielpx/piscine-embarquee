# MD00 EX01

## TIMER1

a counter (TCNT1)
plus settings (TCCR1x)
plus compare values (OCR1x)
plus interrupt controls (TIMSK1 / TIFR1)

## Vocabulary

### Waveform Generation: Timer Behaviour

Waveform generation is about how the timer behaves over time and what kind of signal it produces.

The timer is like a counter: 0 -> 1 -> 2 -> 3 -> ... -> TOP -> repeat

The waveform mode decides:
- When the counter resets
- Whether it counts up or up/down
- When outputs change (pins toggle, go HIGH/LOW)

### Prescaling: Timer Speed

The timer runs on the CPU clock very quickly (in our case, 16 MHz), and may cause overflows too quickly. Prescaling slows it down by dividing the clock by a factor, enabling us to measure longer time intervals.

| Prescaler | CS12 | CS11 | CS10 | Timer Frequency (16 MHz clock) |
|------|-|-|-|---------|
| None | 0 | 0 | 0 | Timer/Counter stopeed|
| 1 | 0 | 0 | 1 | 16 MHz  |
| 8 | 0 | 1 | 0 | 2 MHz  |
| 64 | 0 | 1 | 1 | 250 kHz  |
| 256 | 1 | 0 | 0 | 62.5 kHz  |
| 1024 | 1 | 0 | 1 | 15.625 kHz  |

### Interrupts: Automatic Reactions

An interrupt is like the microcontroller saying: “Stop what you're doing — something important just happened.”

1. Overflow Interrupt  
Triggered when: TCNT1 -> 65535 -> 0

2. Compare Match Interrupt  
Triggered when: TCNT1 == OCR1A (or OCR1B)

3. Input Capture Interrupt  
Triggered when: External signal arrives on ICP pin

## Registers

### Control Registers

#### TCCR1A – Timer/Counter1 Control Register A

Controls waveform generation (partly) and output behavior.

Key bits:

COM1A1:0 / COM1B1:0 -> Control what happens on output pins (OC1A / OC1B)  
Toggle, clear, set on compare match  
WGM11:10 -> Lower bits of waveform mode  

👉 Used for:

PWM mode selection (together with TCCR1B)  
Output pin behavior  

#### TCCR1B – Timer/Counter1 Control Register B  

Controls clock source, prescaler, and completes waveform setup.

Key bits:

CS12:10 -> Clock select (prescaler)  
001 = no prescaling  
011 = /64  
101 = /1024  
WGM13:12 -> Upper waveform bits  
ICES1 -> Input capture edge select
ICNC1 -> Input capture noise canceler

👉 Used for:

Starting/stopping the timer
Setting speed (prescaler)

#### TCCR1C – Timer/Counter1 Control Register C

Less commonly used.

Key bits:

FOC1A / FOC1B -> Force Output Compare (non-PWM mode only)

👉 Used for:

Manually triggering compare events

### Counter Register
TCNT1H / TCNT1L – Timer/Counter1

**This is the actual 16-bit counter value.**

Counts up (or up/down depending on mode)
Range: 0 -> 65535

👉 Use cases:

Measure time
Create delays

Example:

TCNT1 = 0;   // reset timer

### Output Compare Registers
OCR1AH / OCR1AL – Output Compare A
OCR1BH / OCR1BL – Output Compare B

These store values to compare against TCNT1.

When:

TCNT1 == OCR1A (or OCR1B)

-> A compare match occurs.

👉 Used for:

Generating PWM
Creating precise time events

Example:

OCR1A = 15624;  // match value

### Input Capture Register
ICR1H / ICR1L – Input Capture Register

Stores timer value when an external event occurs on ICP1 pin.

👉 Used for:

Measuring pulse width
Measuring frequency

Example:

uint16_t captured = ICR1;

### Interrupt Registers
TIMSK1 – Interrupt Mask Register

Enables/disables Timer1 interrupts.

Key bits:

TOIE1 -> Overflow interrupt enable
OCIE1A / OCIE1B -> Compare match interrupts
ICIE1 -> Input capture interrupt

Example:

TIMSK1 |= (1 << OCIE1A);  // enable compare A interrupt
TIFR1 – Interrupt Flag Register

Shows which events occurred.

Key bits:

TOV1 -> Overflow happened
OCF1A / OCF1B -> Compare match occurred
ICF1 -> Input capture occurred

👉 Writing 1 clears the flag.

Example:

if (TIFR1 & (1 << OCF1A)) {
    // compare match occurred
}

## TOP and BOTTOM
BOTTOM = 0
TOP = max value (or a custom value depending on mode)

When it reaches these:

It may reset
It may change direction
It may trigger events

## Modes (WGM bits)

The Waveform Generation Mode (WGM) bits decide how the counter behaves.

Examples:

Normal mode -> just counts up to max, then rolls over
CTC mode -> resets when it hits a specific value (TOP)
PWM modes -> counts up/down to generate signals (like dimming LEDs or controlling motors)

👉 This is where timers become really powerful: they’re not just counters—they generate precise signals.

## Start / Stop

Controlled by CS12:0 bits:

Select clock source + speed (prescaler)
If all zero -> timer stops


## Overflow (TOV1 flag)

When the counter passes its limit:

It sets a flag called TOV1

You can use this to:

Trigger an interrupt -> pause your program and run a function

👉 Example: “Every 1 ms, do something.”


## Important caveat

If you write to the counter while it’s running:

You might get unpredictable results (timing conflicts)

## Atomic Operations


## On the datasheet:
- 16.2.1 Registers
- 16.2.2 Definitions of BOTTOM, MAX, TOP
- 16.9 Modes of operation
- Table 16-4 @pg141

godbolt.org