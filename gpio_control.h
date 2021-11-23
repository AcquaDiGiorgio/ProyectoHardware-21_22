#ifndef GPIO_CONTROL_H
#define GPIO_CONTROL_H

	#include <inttypes.h>

	// Inicialización de la GPIO (Limpieza)
	void GPIO_iniciar (void);
	
	// Lectura y escritura en la GPIO
	int GPIO_leer (int bit_inicial, int num_bits);
	void GPIO_escribir (int bit_inicial, int num_bits, int32_t valor);
	
	// Marcado de un pin como entrada/salida
	void GPIO_marcar_entrada (int bit_inicial, int num_bits);
	void GPIO_marcar_salida (int bit_inicial, int num_bits);
	
	// Limpieza de un pin de tipo salida
	void GPIO_clear_salida(int bit_inicial, int num_bits);

#endif //GPIO_CONTROL_H
