#include "gpio_control.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include <stdint.h>

/*Permite emplear el GPIO y debe ser invocada antes de
poder llamar al resto de funciones de la biblioteca */
void GPIO_iniciar (void) {
	IOCLR = IOCLR | 0xFFFFFFFF;
}

/* La función devuelve un
entero con el valor de los bits indicados */
int32_t GPIO_leer (int bit_inicial, int num_bits) {
	if((num_bits+bit_inicial) <= 32){
		int32_t mascara = 0x0;
		int32_t dato = IOSET; //Recoge los datos de IOSET
		int i;
		for(i = 0; i<num_bits; i++){ //Bucle que genera la mascara para seleccionar el dato
			mascara = mascara << 1;
			mascara = mascara | 0x1;
		}
		dato = dato >> bit_inicial; //Desplazamos la cadena de bits hata la posicion inicial
		dato = dato & mascara; //Aplicamos la mascara
		return dato;
	}
	else{ return -1; } //ERROR
}

/* Similar al anterior, pero en lugar
de leer escribe en los bits indicados el valor*/
void GPIO_escribir (int bit_inicial, int num_bits, int32_t valor) {
	if((num_bits+bit_inicial) <= 32){
		int32_t mascara = 0x0;
		int i;
		for(i=0; i<num_bits; i++){ //Bucle que genera la mascara para seleccionar el valor
			mascara = mascara << 1;
			mascara = mascara | 0x1;
		}
		valor = valor & mascara; //Solecionamos los bits del dato que se pueden
		valor = valor << bit_inicial; //Desplazamos hasta la posicion indicada
		IOSET = IOSET | valor; //Actualizamos el IOSET
	}
}

/* Los bits indicados se utilizarán
como pines de entrada*/
void GPIO_marcar_entrada (int bit_inicial, int num_bits) {
		if((num_bits+bit_inicial) <= 32){
		int32_t mascara = 0x0;
		int i;
		for(i=0; i<num_bits; i++){ //Bucle que genera la mascara
			mascara = mascara << 1;
			mascara = mascara | 0x1;
		}
		mascara = mascara << bit_inicial; //Desplazamos la mascara hasta su posicion
		mascara =  ~ mascara;
		IODIR = IODIR & mascara; //Activamos los pines para salida
	}
}

/* Los bits indicados se utilizarán
como pines de salida*/
void GPIO_marcar_salida (int bit_inicial, int num_bits)  {
	if((num_bits+bit_inicial) <= 32){
		int32_t mascara = 0x0;
		int i;
		for(i = 0; i<num_bits; i++){ //Bucle que genera la mascara
			mascara = mascara << 1;
			mascara = mascara | 0x1;
		}
		mascara = mascara << bit_inicial; //Desplazamos la mascara hasta su posicion
		IODIR = IODIR | mascara; //Activamos los pines para salida
	}
}
//-----------------------------------------------------------------
