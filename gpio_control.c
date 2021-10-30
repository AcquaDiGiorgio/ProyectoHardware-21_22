//#include "gpio_control.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include <stdint.h>

// variable para comprobar que se hacen las interrupciones que deberían hacerse
static volatile unsigned int eint0_count = 0;
// variable que se activa al detectar una nueva pulsación
static volatile int eint0_nueva_pulsacion = 0;

void GPIO_iniciar () {
	IOSET = IOSET | 0x1;
	IOCLR = IOCLR | 0x1;

}

int32_t GPIO_leer (int bit_inicial, int num_bits) {
	int32_t dato = IOPIN;
	int32_t mascara = 0xFFFFFFFF; //Creamos una mascara llena de 1´s
	
	mascara = mascara << bit_inicial; // Seleccionamos todos los bits a partir de la posicion inicial, 11111000 == 11111111 << 3 
	dato = dato && mascara;
	
	dato = dato >> bit_inicial;	// Desplazamos a la posicion inicial el dato, 00010101 == 10101010 >> 3
	
	mascara = 0xFFFFFFFF;
	mascara = mascara >> (32 - (num_bits + bit_inicial)); // 00010101 == 10101010 >> 3
	dato = dato && mascara;
	return dato;
}

void GPIO_escribir (int bit_inicial, int num_bits, int32_t valor) {
	valor = valor << bit_inicial; // Seleccionamos todos los bits a partir de la posicion inicial, 11111000 == 11111111 << 3 
	int32_t mascara = 0xFFFFFFFF; //Creamos una mascara llena de 1´s
	mascara = mascara >> (32 - (num_bits + bit_inicial));
	valor = valor && mascara; //Se crea la mascara para escribir
	IOPIN = IOPIN | valor; //Se midifica el IOPIN
}

void GPIO_marcar_entrada (int bit_inicial, int num_bits) {
	

}

void GPIO_marcar_salida (int bit_inicial, int num_bits)  {
	int32_t mascara1 = 0x7FFFFFFF; //Creamos una mascara llena de 1´s
	int32_t mascara2 = 0xFFFFFFFF; //Creamos una mascara llena de 1´s
	mascara1 = mascara1 >> (31 - (num_bits + bit_inicial));
	mascara2 = mascara2 << bit_inicial; //11111000 == 11111111 << 3 
	int32_t mascara3 = mascara1 & mascara2;
	IODIR = IODIR | mascara3;
}
//-----------------------------------------------------------------