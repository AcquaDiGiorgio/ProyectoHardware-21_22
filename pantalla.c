#include "pantalla.h"
#include "uart0.h"
#include "constantes_comunes.h"

#define BUFF_SIZE 5000

struct index{
		int aMostrar;			// Evento a ser ejecutado
		int aEscribir;  // Espacio libre para introducir un evento
};

static volatile struct index indice_buffer;
static volatile int buffer[BUFF_SIZE];
static volatile boolean buffer_vacio = TRUE;
boolean escribiendoComnado = FALSE;

void pantalla_write_buffer(void)
{	
		if(buffer_vacio == FALSE)
		{
				uart_print_chr(buffer[indice_buffer.aMostrar]);
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

void uart_add_to_buffer(char buff[], int size)
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
				pantalla_write_buffer();
		}
}