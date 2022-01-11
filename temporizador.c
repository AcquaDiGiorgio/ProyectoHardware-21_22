#include <LPC210X.H>
#include "eventos.h"
#include "cola.h"

#define DEFAULT 0x20

void timer0_ISR(void) __irq;
void timer1_ISR(void) __irq;

void __swi(0xFD) enable_isr_fiq (void);
void __swi(0xFC) disable_isr_fiq (void);

//uint64_t __swi(0x00) clock_gettime (void);

static volatile unsigned int timer1_count = 0;

void temporizador_iniciar(void)
{

	T1MR0 = 0xD693A400 - 0x1;                     // Timer1 interrumpe cada 4 minutos (3.600.000.000 cuentas)
	T1MCR = 3;                     								// Timer1 interrumpe y reinicia al llegar a T1MR0
	T1TCR = 1;                             				// Timer1 Enable
	
	
	VICVectAddr1 = (unsigned long)timer1_ISR;			// Función que se llamará cuando llegue la interrupción timer0
	VICVectCntl1 = DEFAULT | 5;
}


void temporizador_empezar()
{
	VICIntEnable = VICIntEnable | 0x00000020;			// Encendemos timer1
}

// Devuelve ms
uint64_t temporizador_leer(void)
{
	 // Veces interrumpido * Tiempo por interrupcción (ms) + Ciclos actuales / ciclos por ms  con  reloj de 60 MHz
	 return timer1_count * 240000000 + T1TC / 15;
}

uint64_t __SWI_0 (void){
	return temporizador_leer();
}

void temporizador_parar()
{
	VICIntEnable = VICIntEnable & 0xFFFFFFDF;			// Quitamos timer1
	timer1_count = 0;
}

void temporizador_periodo(int periodo)
{
	int cuentas;
	cuentas = periodo * 15000;										
	T0MR0 = cuentas - 1;													// Timer0 interrumpe cada 'periodo' ms = 'cuentas' - 1 counts
	T0MCR = 3;																		// Timer0 interrumpe y reinicia al llegar a T0MR0
	T0TCR = 1;																		// Timer0 Enable
	
	VICVectAddr0 = (unsigned long)timer0_ISR;			// Función que se llamará cuando llegue la interrupción timer0
	
	VICVectCntl0 = DEFAULT | 4;

	VICIntEnable = VICIntEnable | 0x00000010;			// Encendemos sólo timer0
}

/*
  Real-time clock (RTC) 
*/
//Inicializa el RTC, reseteando la cuenta, ajustando el reloj y activando el enable.
void RTC_init(void)
{
	PREINT	= 456; 															// (15.000.000 / 32768) - 1
	PREFRAC = 25024;														//  15.000.000 - ((456+1)*32768)
	CCR 		= 0x01;
}

// Devuelve los segundos transcurridos en el minuto actual.
uint8_t RTC_leer_minutos(void)
{
	return (CTIME0 & 0x3F00) >> 8;
}

// Devuelve los segundos de juego (entre 0 y 59).
uint8_t RTC_leer_segundos(void)
{
	return (CTIME0 & 0x3F);
}

/*
  Watchdog (WD) 
*/

// Alimenta al watchdog timer
void WD_feed(void)
{
	disable_isr_fiq();
	WDFEED = 0xAA;						   
	WDFEED = 0x55;
	enable_isr_fiq();
}

// Inicializa el watchdog timer para que resetee el procesador
// dentro de sec segundos su no se le "alimenta"
void WD_init(int sec)
{
	// Asigna el periodo
	WDTC = sec * 14999 * 333;
	WDMOD = 0x3;
	// Se debe alimentar una primera vez para ponerlo en marcha
	WD_feed();
}

void timer0_ISR(void) __irq
{
	cola_guardar_eventos(SET_TIMER_0, NO_AUX_DATA, FIQ);	// Metemos en la cola el evento sin AuxData
	T0IR = 1;                              								// Clear interrupt flag
	VICVectAddr = 0;                       								// Acknowledge Interrupt
}

void timer1_ISR(void) __irq
{
	timer1_count++;																				// Sumamos 1 en el número de cuentas
	T1IR = 1;                              								// Clear interrupt flag
	VICVectAddr = 0;                       								// Acknowledge Interrupt
}
