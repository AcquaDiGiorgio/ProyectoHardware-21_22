#include <LPC210X.H>
#include <stdint.h>
#include "gestor_energia.h"
#include "idle_powerdown.h"
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

void energia_power_down(void)  
{
		energia_actualizar_estado(EV_POWER);
		IPD_power_down();
}

void energia_idle(void)
{
		IPD_idle();
}	
