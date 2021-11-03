// ...ID... ............auxData...............
// -------- -------- - -----------------------
// ID				Evento   P Retardo
#include <LPC210X.H>
#include "eventos.h"
#include "cola.h"

#define DEFAULT 0x20

void timer_ISR(void) __irq;
void timer1_ISR(void) __irq;

static volatile unsigned int timer1_count = 0;
static volatile unsigned int timer0_count = 0;

void temporizador_iniciar(void){
	T1MR0 = 0xFFFFFFFF - 0x1;                     // Timer1 interrumpe cada 286331 ms = 4294967295 - 1 counts
	T1MCR = 3;                     								// Timer1 interrumpe y reinicia al llegar a T1MR0
	T1TCR = 1;                             				// Timer1 Enable
	
	T0MR0 = 150000 - 1;														// Timer0 interrumpe cada 1 ms = 15000 - 1 counts
	T0MCR = 3;																		// Timer0 interrumpe y reinicia al llegar a T0MR0
	T0TCR = 1;																		// Timer0 Enable
	
	VICVectAddr0 = (unsigned long)timer_ISR;
	VICVectAddr1 = (unsigned long)timer1_ISR;
	
	VICVectCntl0 = DEFAULT | 4;
	VICVectCntl1 = DEFAULT | 5;
	
	VICIntEnable = VICIntEnable | 0x00000010;
}


void temporizador_empezar(){
	VICIntEnable = VICIntEnable | 0x00000020;
}


unsigned long temporizador_leer(){
	 // Veces interrumpido * maxCount (ms) + Count actual (ms)
	 return timer1_count * 0xFFFFFFFF + T1TC ;
}

void temporizador_parar(){
	VICIntEnable = VICIntEnable & 0xFFFFFFDF;
	timer1_count = 0;
}

unsigned int temporizador_periodo(int periodo){
	return 0;
}

void timer_ISR(void) __irq {
	timer0_count++;
	cola_guardar_eventos(TIMER_0, 0);
}

void timer1_ISR(void) __irq{
	timer1_count++;
}
