#include "gpio_control.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include <stdint.h>

//Esportamos las funciones auxiliares correspondientes
extern int32_t GPIO_marcar_arm(int bit_inicial, int num_bits);
extern int GPIO_leer_arm(int bit_inicial, int num_bits, int32_t valor);
extern int GPIO_escribir_arm(int bit_inicial, int num_bits, int32_t valor);

/*Permite emplear el GPIO y debe ser invocada antes de
poder llamar al resto de funciones de la biblioteca */
void GPIO_iniciar () {
	IOSET = IOSET | 0xFFFFFFFF;
}

/* La función devuelve un
entero con el valor de los bits indicados */
int32_t GPIO_leer (int bit_inicial, int num_bits) {
	return GPIO_leer_arm(bit_inicial,num_bits,IOSET);
}

/* Similar al anterior, pero en lugar
de leer escribe en los bits indicados el valor*/
void GPIO_escribir (int bit_inicial, int num_bits, int32_t valor) {
	int32_t mascara = GPIO_escribir_arm(bit_inicial,num_bits,valor);
	IOSET = IOSET | mascara;
}

/* Los bits indicados se utilizarán
como pines de entrada*/
void GPIO_marcar_entrada (int bit_inicial, int num_bits) {
	int32_t mascara = GPIO_marcar_arm(bit_inicial,num_bits);
	mascara = ~ mascara;
	IODIR = IODIR & mascara;
}

/* Los bits indicados se utilizarán
como pines de salida*/
void GPIO_marcar_salida (int bit_inicial, int num_bits)  {
	int32_t mascara = GPIO_marcar_arm(bit_inicial,num_bits);
	IODIR = IODIR | mascara;
}
//-----------------------------------------------------------------