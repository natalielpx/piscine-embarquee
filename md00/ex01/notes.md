# MD00 EX01

## `avr/io.h`
With `#include <avr/io.h>`, the preprocessor looks at the `-mmcu=` flag passed to `avr-gcc` (in our case: `-mmcu=atmega328p`) and resolves to the correct device header, such as `iom328p.h`. Otherwise, every register address for the specific chip will have to be `#define`d manually, and the source code will have to be changed each time the type of chip is changed.

In this project, it will help define:
- Register names: symbolic names mapped to their memory addresses (e.g. `PORTB`, `DDRB`, `PINB`)
- Bit names: individual bit positions within registers (e.g. `PB0`, `PB5`, `RXEN0`)

## AVR I/O Registers
Every physical I/O port on an AVR (Port B, Port C, Port D, etc.) is controlled by three registers — `DDRx`, `PORTx`, and `PINx`, where x is just a placeholder for the port letter. For example, the registers of Port B will be named `DDRB`, `PORTB`, and `PINB`. Not all ports exist on all chips: smaller chips may only have `PORTB`.
Each of these registers comprise of 8 bits. Each bit corresponds to one physical pin on that port. For example, the register `DDRD` corresponds to the pins `PD0~7`.

### `DDRx`: Data Direction Register (Input/Output)
Bits in the data direction register tells the hardware which direction current can flow. It doesn not directly affect the electrical state of the pin.
- Output: 0  
0 is the default state during reset.
- Input: 1
### Data Register `PORTx` (Task)
If pin is configured as:
- Output (`DDRx` is set to 0):
	- 0: leaves pin floating (tri-state / high impedance)  
	- 1: enables internal pull-up resistor  
	Connects an internal resistor between the pin and VCC. That resistor "pulls" the pin voltage up toward VCC, so when the switch is open and nothing else is driving the pin, it reads a stable HIGH instead of floating.
- Input (`DDRx` is set to 1):
	- 0: drives pin LOW (GND)  
	LED does not light up
	- 1: drives pin HIGH (~Vcc)  
	LED lights up
### Input Pins Register `PINx` (Current State)
When read, gives actual physical voltage level of pin.
## Non-destructive Bit Setting
Unless intentionally configuring the entire port at once, registers should only be manipulated via bit operations. `DDRx = 0` will set all 8-bits on the register to 0, which is alright if the intention is to clear all 8 pins on the port. To set or clear a specific pin, care should be taken to manipulate only the bit that controls the target pin.
### set a bit to 1 with `|=`
Say we want to set the 3rd bit (from the right) of a given register to 1. Only the target bit should be set to 1, regardless of it's initial value. The rest of the 7 bits of the register must remain as they were. Essentially, we want to do the following:
```
register = register OR 00000100
register = register | 00000100
register |= 00000100
register |= (1 << 3 - 1)
register |= (1 << 2)
```
### clear a bit to 0 with `&= ~`
On the other hand, to set the 3rd bit of the register to 0, we can use a similar logic and the bitwise negation operation `~`:
```
register = register AND 11111011
register = register AND NEG(00000100)
register = register & ~(00000100)
register &= ~(00000100)
register &= ~(1 << 3 - 1)
register &= ~(1 << 2)
```