#include <LPC210X.H>
#include "gestor_comandos.h"
#include "eventos.h"
#include "cola.h"
#include "constantes_comunes.h"
#include "uart0.h"

#define BUFF_SIZE 20 

void rsi_uart0(void) __irq;

static volatile char buffer[BUFF_SIZE];
static volatile int sigChar = 0;

/* ****************************       */
/* Initialize Serial Interface       */
/* ****************************       */
void init_serial (void) /* Initialize Serial Interface       */
{               
  PINSEL0 |= 0x5;                  			 /* Enable RxD0 and TxD0              */
  U0LCR = 0x83;                          /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;                            /* 9600 Baud Rate @ 15MHz VPB Clock  */
  U0LCR = 0x03;                          /* DLAB = 0                          */
	
	// U0FCR = 0x41;

  U0IER |= 0x3;														// RBR and THRE interrupts enabled
  VICVectAddr4 = (unsigned long)rsi_uart0;          // set interrupt vector in 4
  
	// 0x20 bit 5 enables vectored IRQs. 
  // 6 is the number of the interrupt assigned. Number 6 is the UART0
  VICVectCntl4 = 0x20 | 6;                   
  VICIntEnable = VICIntEnable | 0x00000040;                  // Enable UART0 Interrupt
	
}

void char_to_uart(char chr)
{	
	U0THR = chr;
	// Gestion interrupciones
}

interUart_t detectar_interrupcion(uint8_t IIR)
{
		
	if ( (IIR & 0x8) == 0 && (IIR & 0x4) == 1 && (IIR & 0x2) == 0 )
	{
		return INTER_RDA;
	}
	else if ( (IIR & 0x8) == 0 && (IIR & 0x4) == 0 && (IIR & 0x2) == 1 )
	{
		return INTER_THRE;
	}
	
	return INTER_UNKNOWN;
}

/* **************************************************************  */
/* Funcion que se ejecuta cuando se produce la interrupcion       */
/* **************************************************************  */
void rsi_uart0(void) __irq
{
	int i;

	if ( U0IIR == 0x04 )
	{
		if( U0LSR & 0x01 ) 								//U0RBR contains valid data
		{			
			char_to_uart(U0RBR);			
			cola_guardar_eventos(SET_UART, U0RBR);
		}	
	}
	else if ( U0IIR == 0x02 )
	{
		
	}
	
	VICVectAddr = 0;
}
