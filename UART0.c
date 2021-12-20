#include <LPC210X.H>
#include "gestor_comandos.h"
#include "eventos.h"
#include "cola.h"
#include "constantes_comunes.h"
#include "uart0.h"

#define BUFF_SIZE 5000

void rsi_uart0(void) __irq;
boolean escribiendoComnado = FALSE;

static volatile int buffer[BUFF_SIZE];
static volatile boolean buffer_vacio = TRUE;

struct index{
		int aMostrar;			// Evento a ser ejecutado
		int aEscribir;  // Espacio libre para introducir un evento
};

static volatile struct index indice_buffer;

/* **************************** */
/* Initialize Serial Interface  */
/* **************************** */
void init_serial (void) /* Initialize Serial Interface       */
{               
  PINSEL0 |= 0x5;                  			 /* Enable RxD0 and TxD0              */
  U0LCR = 0x83;                          /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;                            /* 9600 Baud Rate @ 15MHz VPB Clock  */
  U0LCR = 0x03;                          /* DLAB = 0                          */
	U0FCR = U0FCR | 0x1;
	U0FCR = U0FCR & 00111111;

  U0IER |= 0x3;															// RBR and THRE interrupts enabled
  VICVectAddr4 = (unsigned long)rsi_uart0;  // set interrupt vector in 4
  
	// 0x20 bit 5 enables vectored IRQs. 
  // 6 is the number of the interrupt assigned. Number 6 is the UART0
  VICVectCntl4 = 0x20 | 6;                   
  VICIntEnable = VICIntEnable | 0x00000040; // Enable UART0 Interrupt
	
}

void write_buffer(void)
{	
	if(buffer_vacio == FALSE)
	{
		U0THR = buffer[indice_buffer.aMostrar];
		indice_buffer.aMostrar++;
		
		if (indice_buffer.aMostrar == BUFF_SIZE)
		{
			indice_buffer.aMostrar = 0;
		}
		if (indice_buffer.aMostrar == indice_buffer.aEscribir){
			buffer_vacio = TRUE;
		}
	}	
}

void recibir_buffer(char buff[], int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		buffer[indice_buffer.aEscribir] = buff[i];
		indice_buffer.aEscribir++;
		
		if (indice_buffer.aEscribir == BUFF_SIZE)
		{
			indice_buffer.aEscribir = 0;
		}
		if (indice_buffer.aEscribir == indice_buffer.aMostrar){
			// Overflow
		}
	}
	if (buffer_vacio == TRUE)
	{
		buffer_vacio = FALSE;
		write_buffer();
	}
}

/* **************************************************************  */
/* Funcion que se ejecuta cuando se produce la interrupcion       */
/* **************************************************************  */
void rsi_uart0(void) __irq
{
	int interrupt;
		char chr;
	interrupt = U0IIR >> 0x1;
	interrupt = interrupt & 0x03;
	
	if ( interrupt == 0x02 )				// BRB intrruption
	{
		if( U0LSR & 0x01 ) 								// U0RBR contains valid data
		{			
			chr = U0RBR;
			recibir_buffer(&chr,1);
			cola_guardar_eventos(SET_UART_SEND_CHR, chr);
		}	
	}
	else if ( interrupt == 0x01 )	// THRE Interruption
	{	
		write_buffer();
	}
	
	VICVectAddr = 0;
}
