#include "gestor_energia.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include <stdint.h>

static volatile int estado_energia = DESPIERTA;

void actualizar_estado_energia(void)
{	
	switch(estado_energia)
	{
		case DESPIERTA:								// Se viene de tener el procesador despierto
			estado_energia = DORMIDA;		// Se pone a dormida
			PM_power_down();						// Se duerme
			break;
		
		case DORMIDA:									// Se viene de tener el procesador dormido				
			estado_energia = DESPIERTA;	// Se pone a despierta
			PM_wakeup();								// Se despierta
			break;
		
		default:
			break;
	}
}

int obterner_estado_enegia(void)
{
	return estado_energia;
}

void PM_power_down (void)  
{
  actualizar_estado_energia();
	EXTWAKE = 6; 			// EXTINT1 y EXTINT2 despertarán al procesador
	PCON |= 0x02; 		// Se pone al procesador en modo PowerDown
	Switch_to_PLL(); 	// Se reconfigura el PLL
}

void PM_idle (void)  
{
	PCON |= 0x01; 		// Se pone al procesador en modo Idle
}

void PM_wakeup (void)
{
	EXTWAKE = EXTWAKE & 0x0F8;
	PCON = PCON & 0xFC; //Despierta al procesador
}
