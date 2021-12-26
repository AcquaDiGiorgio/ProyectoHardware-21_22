#ifndef UART0_H
#define UART0_H

	#include <inttypes.h>

	typedef enum {
		INTER_THRE,
		INTER_RDA,
		INTER_UNKNOWN
	}interUart_t;

	void uart_init(void (*f)(char*,int), void (*writeStr)());
	void uart_print_chr(char chr);
	
#endif //UART0_H
