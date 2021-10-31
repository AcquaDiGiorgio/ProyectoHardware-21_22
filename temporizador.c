// ...ID... ............auxData...............
// -------- -------- - -----------------------
// ID				Evento   P Retardo
#include <LPC210X.H>
#include "eventos.h"
#include "cola.h"

void timer0_ISR (void) __irq;
void timer1_ISR(void) __irq;

static volatile unsigned long timer1_count = 0;

void temporizador_iniciar(){
	T1MR0 = 0xFFFFFFFF;                        		// Interrumpe cada ?ms =  counts
	T1MCR = 3;                     								// Timer1 interrumpe y reinicia al llegar a T1MR0
	T1TCR = 1;                             				// Timer1 Enable
	int val = T1TC;
	VICVectAddr0 = (unsigned long)timer0_ISR;
	VICVectAddr1 = (unsigned long)timer1_ISR;
}


void temporizador_empezar(){
	VICVectCntl1 = 0x20 | 5;
}


unsigned long temporizador_leer(){
	 return timer1_count * 0xFFFFFFFF + T1TC;
}

void temporizador_parar(){
	VICVectCntl1 = VICVectCntl1 & 0x1F;
	timer1_count = 0;
}

unsigned int temporizador_periodo(int periodo){
	
}

void timer0_ISR(void) __irq {
	cola_guardar_eventos(TIMER_0, 0);
}

void timer1_ISR(void) __irq{
	timer1_count++;
}