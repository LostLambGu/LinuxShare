#include "uart.h"

void printException(unsigned int cpsr, char *str)
{
	puts("Excption! cpsr = ");
	printHex(cpsr);
	puts(" ");
	puts(str);
	puts("\r\n");
}
