#include "../embd.h"

int main( void ) {

    uart_init();

    while (42) {

        uart_tx('Z');
        _delay_ms(1000);
    }
}
