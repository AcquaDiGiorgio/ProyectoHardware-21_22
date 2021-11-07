
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer0.h"
// variable para contabilizar el número de interrupciones
static volatile unsigned int timer0_int_count = 0;

void timer0_ISR (void) __irq;    // Generate Interrupt 


/* Setup the Timer Counter 0 Interrupt */
void timer0_init (void) {
		
	timer0_int_count = 0;	
	
	// configuration of Timer 0
	T0MR0 = 149999;                        				// Interrumpe cada 0,05ms = 150.000-1 counts
	T0MCR = 3;                     								// Generates an interrupt and resets the count when the value of MR0 is reached
	T0TCR = 1;                             				// Timer0 Enable
	
	// configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
	VICVectAddr0 = (unsigned long)timer0_ISR;     // set interrupt vector in 0
	
	// 0x20 bit 5 enables vectored IRQs. 
	// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
	VICVectCntl0 = 0x20 | 4;                   
	VICIntEnable = VICIntEnable | 0x00000010;     // Enable Timer0 Interrupt
}


/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
	timer0_int_count++;
	T0IR = 1;                              				// Clear interrupt flag
	VICVectAddr = 0;                       				// Acknowledge Interrupt
}

unsigned int timer0_read_int_count(void){
	return timer0_int_count;
};

