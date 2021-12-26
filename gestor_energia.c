#include <LPC210X.H>
#include <stdint.h>
#include "gestor_energia.h"
#include "constantes_comunes.h"

static volatile int estado_energia = DESPIERTO;

//Devuelve el estado de energia actual del sistema
int energia_estado_actual(void)
{
	return estado_energia;
}

void energia_actualizar_estado(event_t evento)
{	 
		int value = estado_energia;
		switch(evento)
		{
			case EV_POWER:				// Le llama powerdown
				value = DORMIDO;		// Se pone a dormido
				break;
			
			default:							// Le llama otra parte del sistema
				value = DESPIERTO;	// Se pone a despierto
				break;
		}
		
		estado_energia = value;
}

void PM_power_down (void)  
{
		energia_actualizar_estado(EV_POWER);
		EXTWAKE = 6; 						// EXTINT1 y EXTINT2 despertarán al procesador
		PCON |= 0x02; 					// Se pone al procesador en modo PowerDown
		Switch_to_PLL(); 				// Se reconfigura el PLL
}

void PM_idle (void)  
{
		PCON |= 0x01; 					// Se pone al procesador en modo Idle
}

void end_execution_error(void)
{
		while (1)								// While 1 por si acaso
		{
				EXTWAKE = 0; 					// Ni EXTINT0 ni EXTINT1 ni EXTINT2 despertarán al procesador
				PCON |= 0x02; 				// Se pone al procesador en modo PowerDown
		}
}
