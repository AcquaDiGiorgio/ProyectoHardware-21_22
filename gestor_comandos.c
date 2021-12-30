#include <LPC210X.H>
#include <inttypes.h>
#include "gestor_comandos.h"
#include "constantes_comunes.h"
#include "cola.h"
#include "eventos.h"
#include "sudoku_p2.h"
#include "pantalla.h"
#include "gestor_alarmas.h"

#define MAX_COMMAND_SIZE 6
#define JUGADA_SIZE 4

static volatile char comando[MAX_COMMAND_SIZE];
static volatile char comandoAnterior[MAX_COMMAND_SIZE];
static volatile int posicion_actual;

static volatile int oldValue = 0;
static volatile boolean comando_cancelado = FALSE;


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
// Esta función debe ser llamada por el scheduler
//
void comando_recibir_caracter(char chr) 
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
		else if(chr == '!')
		{
				comando_detectar();
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

		comando_cancelado = FALSE;
	
		if (comando[0] == 'R' && comando[1] == 'S' && comando[2] == 'T') 			// Comando RESET (RST)
		{
				reiniciar_partida();
		}
		else if (comando[0] == 'N' && comando[1] == 'E' && comando[2] == 'W')	// Comando NEW
		{
				nueva_partida();
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
		uint8_t fila, columna, valor, checksum, checksum_real, i;
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
					
						oldValue = sudoku_leer_valor(fila, columna);
						for(i=0; i<JUGADA_SIZE; i++)
						{
								comandoAnterior[i] = comando[i];
						}	
								
						cola_guardar_eventos(SET_WRITE_COMMAND, auxData, USER);
						alarma_crear_alarma_unica(0,EV_COMMAND_CONFIRM, 3 * SEGUNDO);
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

void comando_cancelar(void)
{
		comando_cancelado = TRUE;
}

void comando_comprobar(void)
{
		uint8_t fila, columna;
		int auxData, info[MAX_COMMAND_SIZE], i;
	
		for (i = 0; i < JUGADA_SIZE; i++)
		{
				info[i] = to_uint(comandoAnterior[i]); 	// Cambio el valor char a int
		}
	
		if (comando_cancelado == TRUE)
		{
				fila 					= info[0];
				columna 			= info[1];
				auxData = (fila << 0x10) | (columna << 0x08) | oldValue;
				pantalla_add_to_buffer("\n\nComando cancelado\n", 20);
				cola_guardar_eventos(SET_WRITE_COMMAND, auxData, USER);
		}
		
		comando_reiniciar();
}

void lanzar_error(command_err err)
{
	switch(err)
	{
		case OVERSIZED_COMMAND:
			pantalla_add_to_buffer("\nTamanyo del comando excedido, reiniciando comando\n", 51);			
			break;

		case NOT_A_COMMAND:
			pantalla_add_to_buffer("\nComando no detectado\n", 22);
			break;

		case BAD_CHECKSUM:
			pantalla_add_to_buffer("\nEl checksum no coincide\n", 25);
			break;
	}
	pantalla_add_to_buffer("\nComando: ",10);
}
