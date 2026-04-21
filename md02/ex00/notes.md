# MD02 EX00

## UART

reference: datasheet pg179

## `uart_init`

reference: datasheet pg184 

Initializes the on-chip UART to 8 data bits, 1 stop bit, and no parity (8250 compatible).

On entry:
EAX
    Baud rate. 
EBX
    Input clock in Hz (normally 1843200). 
ECX
    UART internal divisor (normally 16). 
DX
    UART base port.
ref: https://qnx.com/developers/docs/7.1////com.qnx.doc.neutrino.building/topic/ipl_lib/uart_init.html

https://forum.arduino.cc/t/help-with-custom-uart-initialization/538372

## `uart_tx`
