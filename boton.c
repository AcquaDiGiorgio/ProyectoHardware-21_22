#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "boton.h"
#include "eventos.h"
#include "cola.h"

// variable que se activa al detectar una nueva pulsación
static volatile int nueva_pulsacion_eint2 = 0;
static volatile int nueva_pulsacion_eint1 = 0;
/**/

void eint1_ISR(void) __irq;
void eint2_ISR(void) __irq;

//Funcion para inicializar los botones
void eint_init (void) {
	EXTINT = EXTINT | 6;        								// clear interrupt flag    
 	
	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint1_ISR;    // set interrupt vector in 0
	VICVectAddr3 = (unsigned long)eint2_ISR;    // set interrupt vector in 0
  
	PINSEL0 		 = PINSEL0 & 0x0FFFFFFF;				//Sets bits 0 and 1 to 0
	PINSEL0 		 = PINSEL0 | 0xA0000000;				//Enable the EXTINT1 and EXTINT2 interrupt
	
	VICVectCntl2 = VICVectCntl2 & 0xFFFFFFC0;
	VICVectCntl2 = VICVectCntl2 | 0x20 | 15;
	
	VICVectCntl3 = VICVectCntl3 & 0xFFFFFFC0;
	VICVectCntl3 = VICVectCntl3 | 0x20 | 16;
	
  VICIntEnable = VICIntEnable | 0x00018000;    // Enable EXTINT1 and EXINT2 Interrupt
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
}

void button_clear_nueva_pulsacion_2(){
	nueva_pulsacion_eint2 = 0;
}

//Funcion que se ejecuta cuando se produce la interrupcion
void eint1_ISR (void) __irq {
	nueva_pulsacion_eint1 = 1;
	EXTINT =  EXTINT | 2;
	VICIntEnClr = VICIntEnClr | 0x00008000; //deshabilitamos eint1
	VICVectAddr = 0;
	cola_guardar_eventos(EXT_INT_1,0);
}

void eint2_ISR (void) __irq {
	nueva_pulsacion_eint2 = 1;
	EXTINT =  EXTINT | 4;
	VICIntEnClr = VICIntEnClr | 0x00010000; //deshabilitamos eint2
	VICVectAddr = 0;
	cola_guardar_eventos(EXT_INT_2,0);
}

/******************************************************************************************/
//Devuelve si el boton correspondiente esta pulsado o no
uint8_t boton1_pulsado()	{
	if( (EXTINT & 0x2) != 0 ) return 1;
	else return 0;
}

uint8_t boton2_pulsado()	{
	if( (EXTINT & 0x4) != 0 ) return 1;
	else return 0;
}

//Limpia la interrupcion del boton
void boton1_clear()	{
	EXTINT =  EXTINT | 2;
}

void boton2_clear()	{
	EXTINT =  EXTINT | 4;
}

//Vuelve a activar el boton correspondiente para que vuelva a recibir interrupciones
void boton1_reactivate()	{
	VICIntEnable = VICIntEnable | 0x00008000;
}

void boton2_reactivate()	{
	VICIntEnable = VICIntEnable | 0x00010000;
}
/***************************************************************************************/
