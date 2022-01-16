#include <LPC210X.H>
#include <stdint.h>
#include "idle_powerdown.h"
#include "constantes_comunes.h"

void IPD_power_down(void)  
{
		EXTWAKE = 6; 						// EXTINT1 y EXTINT2 despertarán al procesador
		PCON |= 0x02; 					// Se pone al procesador en modo PowerDown
		Switch_to_PLL(); 				// Se reconfigura el PLL
}

void IPD_idle (void)  
{
		PCON |= 0x01; 					// Se pone al procesador en modo Idle
}
