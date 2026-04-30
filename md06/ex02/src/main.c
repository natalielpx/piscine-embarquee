#include "../embd.h"
#include <stdlib.h>

#define RETRY 5

int ard_init( void ) {

	i2c_init();
	int status = 0;
	for (int i = 0; i < RETRY; ++i) {
		aht20_init();
		status = aht20_read_status(3);
		if (status == 1)
			break;
	}
	if (status != 1)
		return 1;

	uart_init();
	
	SREG |= (1 << 7);	// Global Interrupt
	timers_init();

	return 0;
}


double calc_humidity( uint8_t b1, uint8_t b2, uint8_t b3) {

	double raw = (((uint32_t)b1 << 12) | ((uint32_t)b2 << 4) | ((uint32_t)b3 >> 4));

	return ((raw / 1048576) * 100);
}

double calc_temperature( uint8_t b3, uint8_t b4, uint8_t b5 ) {

	uint32_t raw = (((uint32_t)(b3 & 0x0F) << 16) | ((uint32_t)b4 << 8) | (uint32_t)b5);

	return (((double)raw / 1048576) * 200 - 50);
}


double calc_average( volatile double * data ) {

	double sum = data[0] + data[1] + data[2];
	
	static int count = 0;
	// First measurement
	if (count < 2)
		return (++count, sum);
	// Second measurement
	if (count < 4)
		return (++count, sum / 2);
	// All subsequent measurements
	return (sum / 3);
}

volatile enum States aht20 = IDLE;
volatile uint8_t data[7] = {0};
volatile double temperature[3] = {0};
volatile double humidity[3] = {0};

int main( void ) {

	ard_init();

	char str[7] = {0};
	int tries = 0;
	int count = 0;

	// Super loop
	// ref: https://en.wikibooks.org/wiki/Embedded_Systems/Super_Loop_Architecture
	while (42) {

		if (aht20 == IDLE) continue;

		// Trigger measurement
		if (aht20 == TRIG) {
			tries = 0;
			if (aht20_trig() != 0) continue;
			aht20 = WAIT;
		}

		// Wait for 80ms
		if (aht20 == WAIT) {
			SET_PIN(TIFR0, OCF0A, CLR_FLAG);	// Clear flag in Timer0
			TCNT0 = 0;							// Ensure Timer0 starts from 0
			start_timer0(1024);
			aht20 = WAITING;
		}

		// Check if measurement complete
		if (aht20 == CHECK) {
			if (aht20_read_status(7) == 0)
				aht20 = RECV;
			else {
				++tries;
				if (tries < RETRY)
					aht20 = WAIT;
				else
					aht20 = IDLE;
			}
		}

		// Receive results
		if (aht20 == RECV) {
			
			if (i2c_recv(AHT20_ADDR) != 0) continue;

			// Calculate new values
			temperature[count] = calc_temperature(data[3], data[4], data[5]);
			humidity[count] = calc_humidity(data[1], data[2], data[3]);
			// Obtain average
			temperature[count] = calc_average(temperature);
			humidity[count] = calc_average(humidity);
			// Print results
			uart_print_str("Temperature: ");
			uart_print_str(dtostrf(temperature[count], 4, 1, str));
			uart_print_str("C, Humidity: ");
			uart_print_str(dtostrf(humidity[count], 3, 0, str));
			uart_print_str("%\r\n");

			count = (count + 1) % 3;
			aht20 = IDLE;
		}
	}
}

// ref: https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf
