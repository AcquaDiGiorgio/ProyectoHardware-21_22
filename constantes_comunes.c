#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "constantes_comunes.h"

//
	// Función extremadamente fea que cambia el valor de tipo carácter
	// a entero sin signo
	//
	// Si no es un número, devuelve -1 (error)
	//
	int to_uint(char value)
	{
		switch(value)
		{
			case '0':
				return 0;

			case '1':
				return 1;

			case '2':
				return 2;

			case '3':
				return 3;

			case '4':
				return 4;

			case '5':
				return 5;

			case '6':
				return 6;

			case '7':
				return 7;

			case '8':
				return 8;

			case '9':
				return 9;

			default:
				return -1;
		}
	}
