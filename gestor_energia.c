//#include "gestor_energia.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include <stdint.h>

static volatile int estado = 0;

// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void PM_power_down (void)  {
  EXTWAKE = 7; // EXTINT0,EXTINT1 and EXTINT2 will awake the processor
	PCON |= 0x02; 
}

void PM_idle (void)  {
  EXTWAKE = 7; // EXTINT0,EXTINT1 and EXTINT2 will awake the processor
	PCON |= 0x01; 
}

void PM_wakeup (void) {
	EXTWAKE = 0;
	PCON = PCON & 0xFC; //Despierta al procesador
}