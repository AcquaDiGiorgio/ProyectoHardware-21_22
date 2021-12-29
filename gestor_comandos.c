#include <LPC210X.H>
#include "gestor_comandos.h"
#include <inttypes.h>
#include "constantes_comunes.h"
#include "uart0.h"
#include "cola.h"
#include "eventos.h"
#include "gestor_output.h"

#define MAX_COMMAND_SIZE 6
#define JUGADA_SIZE 4

volatile int comando[MAX_COMMAND_SIZE];
int posicion_actual = 0;
void __swi(0xFF) enable_isr (void);

//
//El vector de caracteres lo vacia
//
void comando_reiniciar(void)
{
	int i;
	
	for(i = 0; i < MAX_COMMAND_SIZE; i++)
	{
		comando[i] = '\0';
	}
	posicion_actual = 0;
}

//
// Introduce un carácter al comando y detecta posibles errores
// 
// Está función debe ser llamada por el scheduler
//
void comando_recibir_caracter(int chr)
{
	if (chr == '#')																	// Principio de un comando
	{
		comando_reiniciar();
	}
	else if (posicion_actual >= MAX_COMMAND_SIZE)		// Comando muy grande
	{
		comando_reiniciar();
		lanzar_error(OVERSIZED_COMMAND);
	}
	else
	{
		comando[posicion_actual] = chr;
		posicion_actual++;
	}
}

//
//Detecta si los caracteres en el vector representan a un comando valido
//
void comando_detectar(void)
{
		int int_val, i;
		int int_command[MAX_COMMAND_SIZE];
		boolean error = FALSE;

		if (comando[0] == 'R' && comando[1] == 'S' && comando[2] == 'T' && comando[3] == '!') 			// Comando RESET (RST)
		{
				reiniciar_partida();
				enable_isr();
		}
		else if (comando[0] == 'N' && comando[1] == 'E' && comando[2] == 'W' && comando[3] == '!')	// Comando NEW
		{
				nueva_partida();
				enable_isr();
		}
		else																																	// Comando Numérico
		{		
				for (i = 0; i < JUGADA_SIZE && error == FALSE; i++)
				{
						int_val = to_uint(comando[i]); 	// Cambio el valor char a int
						if (int_val == -1)							// Si no es un número entero positivo
						{
								error = TRUE;
						}
						else
						{	
								int_command[i] = int_val;
						}
				}

				if (error == FALSE && comando[4] == '!')
				{
						introducir_jugada(int_command);						
						enable_isr();
				}
				else
				{
						lanzar_error(NOT_A_COMMAND);
				}
		}
		comando_reiniciar();
}

void reiniciar_partida(void)
{
	cola_guardar_eventos(SET_RESET_COMMAND, NO_AUX_DATA, USER);
}

void nueva_partida(void)
{
	cola_guardar_eventos(SET_NEW_COMMAND, NO_AUX_DATA, USER);
}

void introducir_jugada(int info[])
{
		uint8_t fila, columna, valor, checksum, checksum_real;
		int auxData;

		fila 					= info[0];
		columna 			= info[1];
		valor 				= info[2];
		checksum 			= info[3];
	
		if ( fila > 0 && columna > 0)
		{	
				checksum_real = (fila + columna + valor) % 8;
	
				if (checksum == checksum_real)
				{
						fila--;
						columna--;
						auxData = (fila << 0x10) | (columna << 0x08) | valor;
						cola_guardar_eventos(SET_WRITE_COMMAND, auxData, USER);
				}
				else
				{
						lanzar_error(BAD_CHECKSUM);
				}
		}
		else
		{
				lanzar_error(NOT_A_COMMAND);
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
