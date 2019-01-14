
#include "s3c2440_soc.h"
#include "uart.h"
#include "init.h"

int main(void)
{
	unsigned char c;
	
	uart0_init();
	puts("Enter the Tacc val: \n\r");
	
	while(1)
	{
		c = getchar();
		putchar(c);

		// Access cycle
		// 000 = 1 clock 001 = 2 clocks
		// 010 = 3 clocks 011 = 4 clocks
		// 100 = 6 clocks 101 = 8 clocks
		// 110 = 10 clocks 111 = 14 clocks
		// Note: When nWAIT signal is used, Tacc ≥ 4 clocks.
		if (c >= '0' && c <= '7')
		{
			bank0_tacc_set(c - '0');
			led_test();
		}
		else
		{
			puts("Error, val should between 0~7\n\r");
			puts("Enter the Tacc val: \n\r");
		}
	}

	return 0;
}



