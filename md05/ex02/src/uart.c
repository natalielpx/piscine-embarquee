#include "../embd.h"

// Sets baud rate to 115200
// Enables receiver and transmitter
// Sets character size to 8
// Disables parity
// Sets stop bit to 1
// ref: datasheet pg185
void uart_init ( void ) {

    // ---- Baud Rate Registers ----
    // ---- Set baud rate
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    // ref: datasheet 204
    
    // ---- Control and Status Register N B ----
    // ---- Enable receiver and transmitter
    // Enable receiver
    // RXEN0 = 1
    // Enable transmitter
    // TXEN0 = 1
    // Character size: 8
    // UCSZN2 = 0 (set in UCSR0B)
    // UCSZN1 = 1
    // UCSZN0 = 1
    // ref: datasheet pg201
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // ---- Control and Status Register N C ----
    // ---- 8N1: Set character size to 8, Disable parity, Set stop bit to 1
    // Mode: Asynchronous
    // UMSELN1 = 0
    // UMSELN0 = 0
    // Parity: disabled
    // UPMN1 = 0
    // UPMN0 = 0
    // Stop bit: 1
    // USBSN = 0
    // Character size: 8
    // UCSZN2 = 0 (set in UCSR0B)
    // UCSZN1 = 1
    // UCSZN0 = 1
    // Used only in synchronous mode; Written to zero during asynchronous mode
    // UCPOLN = 0
    // ref: datasheet pg202
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Waits for data to be present in receive buffer by checking RXCn Flag
// Read buffer and return value
// ref: datasheet pg189
uint8_t uart_rx( void ) {

    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));

    // Get and return received data from buffer
    return UDR0;
}

// Once Data Register Empty (UDREN) Flag in Control and Status Register A (UCSRNA) set
// Put data into transmit buffer and send
// ref: datasheet pg186
void uart_tx( uint8_t c ) {

    // Wait for empty transmit buffer
    // ref: datasheet pg200
    while (!( UCSR0A & (1 << UDRE0)));

    // Put data into transmit buffer and send
    // (A data transmission is initiated by loading the transmit buffer
    // with the data to be transmitted)
    UDR0 = c;
}

void uart_print_str( const uint8_t * str ) {

    for (int i = 0; str[i]; ++i)
        uart_tx(str[i]);
}

void uart_print_dec( uint16_t num ) {

    static const uint8_t dec[DEC] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    
    if (num >= DEC)
        uart_print_dec( num / DEC );

    uart_tx(dec[num % DEC]);
}

void uart_print_hex( uint8_t num ) {

    static const uint8_t hex[HEX] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                    'a', 'b', 'c', 'd', 'e', 'f'};
    
    uart_tx(hex[num / HEX]);
    uart_tx(hex[num % HEX]);
}
