// ...ID... ............auxData...............
// -------- -------- - -----------------------
// ID				Evento   P Retardo
#include <LPC210X.H>
#include "cola.h"

void timer0_ISR (void) __irq;
void timer1_ISR(void) __irq;

static volatile unsigned long timer0_count = 0;
static volatile unsigned long timer1_count = 0;

void temporizador_iniciar(){
	T1MR0 = 149999;                        				// Interrumpe cada 0,05ms = 150.000-1 counts
	T1MCR = 0;                     								// Timer1 ni interrumpe ni se reinicia ni se para al llegar a MR0
	T1TCR = 1;                             				// Timer1 Enable
	int val = T1TC;
	VICVectAddr0 = (unsigned long)timer0_ISR;
	VICVectAddr1 = (unsigned long)timer1_ISR;
}


void temporizador_empezar(){
	VICVectCntl0 = 0x20 | 4;
}


unsigned long temporizador_leer(){
	 return timer0_count;
}

void temporizador_parar(){
	VICVectCntl1 = VICVectCntl1 & 0x1F;
	timer1_count = 0;
}

unsigned int temporizador_periodo(int periodo){
	
}

void timer0_ISR(void) __irq {
	timer0_count++;
	T0IR = 1;
}

void timer1_ISR(void) __irq{
	cola_guardar_eventos(1,1);
}