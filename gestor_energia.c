#include "gestor_energia.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include <stdint.h>

static volatile int estado_energia;

//Inicializa los estados para gestinar la energia
void crear_gestor_energia(){
	estado_energia = MODO_NORMAL;
}

//Funcion para actualizar los estados
void actualizar_estado_energia(void){   
	if (estado_energia == MODO_NORMAL){
		estado_energia = MODO_IDLE;
		PM_idle();
	}
	else{
		PM_wakeup();
		estado_energia = MODO_NORMAL;
	}
}

// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void PM_power_down (void)  {
  EXTWAKE = 7; // EXTINT0,EXTINT1 and EXTINT2 will awake the processor
	PCON |= 0x02; 
	Switch_to_PLL(); //Configura PLL
}

void PM_idle (void)  {
  EXTWAKE = 7; // EXTINT0,EXTINT1 and EXTINT2 will awake the processor
	PCON |= 0x01; 
}

//Despierta al procesador
void PM_wakeup (void) {
	EXTWAKE = EXTWAKE & 0x0F8;
	PCON = PCON & 0xFC; //Despierta al procesador
}
