#include <LPC210X.H>
#include <stdint.h>
#include "gestor_energia.h"
#include "constantes_comunes.h"

static volatile int estado_energia = DESPIERTO;

//Devuelve el estado de energia actual del sistema
int estado_energia_actual(void)
{
	return estado_energia;
}

void actualizar_estado_energia(event_t evento)
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
  actualizar_estado_energia(EV_POWER);
	EXTWAKE = 6; 						// EXTINT1 y EXTINT2 despertarán al procesador
	PCON |= 0x02; 					// Se pone al procesador en modo PowerDown
	Switch_to_PLL(); 				// Se reconfigura el PLL
}

void PM_idle (void)  
{
	PCON |= 0x01; 					// Se pone al procesador en modo Idle
}

void PM_wakeup (void)
{
	EXTWAKE = EXTWAKE & 0x0F8;
	PCON = PCON & 0xFC; 		//Despierta al procesador
}
