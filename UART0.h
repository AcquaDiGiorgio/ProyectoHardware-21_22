#ifndef UART0_H
#define UART0_H

	#include <inttypes.h>

	typedef enum {
		INTER_THRE,
		INTER_RDA,
		INTER_UNKNOWN
	}interUart_t;

	void init_serial (void); /* Initialize Serial Interface       */
	interUart_t detectar_interrupcion(uint8_t IIR);
	void string_to_uart(char string[], int lenght);
#endif //UART0_H
