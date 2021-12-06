#include <LPC210X.H>
#include <inttypes.h>
#include "gestor_comandos.h"
#include "eventos.h"
#include "cola.h"
#include "constantes_comunes.h"

void rsi_uart0(void) __irq;

/* ****************************       */
/* Initialize Serial Interface       */
/* ****************************       */
void init_serial (void) /* Initialize Serial Interface       */
{               
  PINSEL0 |= 0x5;                  			 /* Enable RxD0 and TxD0              */
  U0LCR = 0x83;                          /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;                            /* 9600 Baud Rate @ 15MHz VPB Clock  */
  U0LCR = 0x03;                          /* DLAB = 0                          */

  U0IER |= 0x3;														// RBR and THRE interrupts enabled
  VICVectAddr4 = (unsigned long)rsi_uart0;          // set interrupt vector in 2
  
	// 0x20 bit 5 enables vectored IRQs. 
  // 6 is the number of the interrupt assigned. Number 6 is the UART0
  VICVectCntl4 = 0x20 | 6;                   
  VICIntEnable = VICIntEnable | 0x00000040;                  // Enable UART0 Interrupt
	
}

/* **************************************************************  */
/* Funcion que se ejecuta cuando se produce la interrupcion       */
/* **************************************************************  */
void rsi_uart0(void) __irq
{
	static volatile int dato;
	if (U0IIR & 0x4) // If the RDA interrupt is triggered
	{ 
		if(U0LSR & 0x1) //if 1 en el bit 1 de U0LSR, contains valid data
		{
			dato = U0RBR; 	//Guardamos el dato en una variable
			cola_guardar_eventos(SET_UART, dato);	//Generamos un evento en la cola de eventos
		}
	}
	VICVectAddr = 0;
}
