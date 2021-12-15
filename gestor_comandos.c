#include <LPC210X.H>
#include "gestor_comandos.h"
#include <inttypes.h>
#include "constantes_comunes.h"
#include "uart0.h"

#define MAX_COMMAND_SIZE 10
#define JUGADA_SIZE 4

volatile char comando[MAX_COMMAND_SIZE];
int posicion_actual = 0;

void reiniciar_comando(void)
{
	int i;
	
	for(i = 0; i < MAX_COMMAND_SIZE; i++)
	{
		comando[i] = '\0';
	}
	posicion_actual = 0;
}

//
// Introduce un car�cter al comando y detecta posibles errores
// 
// Est� funci�n debe ser llamada por el scheduler
//
void recibir_caracter(char chr)
{
	if (chr == '#')																	// Principio de un comando
	{
		reiniciar_comando();
	}
	else if (posicion_actual >= MAX_COMMAND_SIZE)		// Comando muy grande
	{
		reiniciar_comando();
		lanzar_error(OVERSIZED_COMMAND);
	}
	else if (chr == '!')														// Fin del comando
	{
		detectar_comando();
		reiniciar_comando();
	}
	else
	{
		comando[posicion_actual] = chr;
		posicion_actual++;
	}
}

void detectar_comando(void)
{
	int int_val, i;
	int int_command[MAX_COMMAND_SIZE];
	boolean error = FALSE;

	if (comando[0] == 'R' && comando[1] == 'S' && comando[2] == 'T') 			// Comando RESET (RST)
	{
		reiniciar_partida();
	}
	else if (comando[0] == 'N' && comando[1] == 'E' && comando[2] == 'W')	// Comando NEW
	{
		nueva_partida();
	}
	else																																	// Comando Num�rico
	{		
		for (i = 1; i < JUGADA_SIZE + 1 && error == FALSE; i++)
		{
			int_val = to_uint(comando[i]); 	// Cambio el valor char a int
			if (int_val == -1)							// Si no es un n�mero entero positivo
			{
				error = TRUE;
			}
			else
			{	
				int_command[i] = int_val;
			}
		}

		if (error == FALSE)
		{
			introducir_jugada(int_command);
		}
		else
		{
			lanzar_error(NOT_A_COMMAND);
		}
	}
}

//
// Funci�n extremadamente fea que cambia el valor de tipo car�cter
// a entero sin signo
//
// Si no es un n�mero, devuelve -1 (error)
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

void reiniciar_partida(void)
{
	char string[] = "RP\n";
	string_to_uart(string,3);
}

void nueva_partida(void)
{
	char string[] = "NW\n";
	string_to_uart(string,3);
}

void introducir_jugada(int info[])
{
	uint8_t fila, columna, valor, checksum, checksum_real;

	fila 					= info[0];
	columna 			= info[1];
	valor 				= info[2];
	checksum 			= info[3];
	checksum_real = (fila + columna + valor) % 8;

	if (checksum == checksum_real)
	{
		// Meter el valor
	}
	else
	{
		lanzar_error(BAD_CHECKSUM);
	}
}

void lanzar_error(command_err err)
{
	switch(err)
	{
		case OVERSIZED_COMMAND:
			break;

		case NOT_A_COMMAND:
			break;

		case BAD_CHECKSUM:
			break;
	}
}
