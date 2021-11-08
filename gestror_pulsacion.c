//#include "botones.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

// variable que se activa al detectar una nueva pulsación
static volatile int nueva_pulsacion_eint2 = 0;
static volatile int nueva_pulsacion_eint1 = 0;
/**/

void eint1_ISR(void) __irq;
void eint2_ISR(void) __irq;

//Funcion para inicializar los botones
void eint_init (void) {
	nueva_pulsacion_eint1 = 0; 									//Indicamos que no ha habido interupcion
	nueva_pulsacion_eint2 = 0;
	EXTINT = EXTINT | 6;        								// clear interrupt flag    
 	
	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint1_ISR;    // set interrupt vector in 0
	VICVectAddr3 = (unsigned long)eint2_ISR;    // set interrupt vector in 0
  
	// 0x20 bit 5 enables vectored IRQs. 
	// 14 is the number of the interrupt assigned. Number 14 is the EINT0 (see table 40 of the LPC2105 user manual 
	PINSEL1 		 = PINSEL1 & 0x0FFFFFFF;				//Sets bits 0 and 1 to 0
	PINSEL1 		 = PINSEL1 | 0xA0000000;				//Enable the EXTINT1 and EXTINT2 interrupt
	
	VICVectCntl2 = 0x20 | 15;
	VICVectCntl3 = 0x20 | 16;
	
  VICIntEnable = VICIntEnable | 0x0000C000;    // Enable EXTINT1 and EXINT2 Interrupt
}

//Devuelve el valor de la variable para detectar una nueva pulsacion
int button_nueva_pulsacion_1(){
	return nueva_pulsacion_eint1;
}

int button_nueva_pulsacion_2(){
	return nueva_pulsacion_eint2;
}

//Resetea la variable correspondiente a 0
void button_clear_nueva_pulsacion_1(){
	nueva_pulsacion_eint1 = 0;
	// Activar Interrupts
}

void button_clear_nueva_pulsacion_2(){
	nueva_pulsacion_eint2 = 0;
}

//Funcion que se ejecuta cuando se produce la interrupcion
void eint1_ISR (void) __irq {
	nueva_pulsacion_eint1 = 1;
	VICIntEnClr = 0x00004000; //deshabilitamos eint1
}

void eint2_ISR (void) __irq {
	nueva_pulsacion_eint2 = 1;
	VICIntEnClr = 0x00008000; //deshabilitamos eint2
}
