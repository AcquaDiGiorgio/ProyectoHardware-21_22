#include "cola.h"
#include "temporizador.h"
#include "gestor_pulsacion.h"
#include "gestor_alarmas.h"
#include "gestor_IO.h"
#include "gestor_energia.h"
#include "gestor_comandos.h"
#include "uart0.h"
#include "gestor_output.h"
#include "sudoku_p2.h"
#include "partida.h"

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);

void __swi(0xFD) enable_isr_fiq (void);
void __swi(0xFC) disable_isr_fiq (void);

struct index{
		int aLeer;			// Evento a ser ejecutado
		int aEscribir;  // Espacio libre para introducir un evento
};

static volatile struct index indice;

void cola_guardar_eventos(event_t idEvento, uint32_t auxData)
{
	int indiceAux;
		
	disable_isr();											// Deshabilitamos interrupciones
	
	indiceAux = indice.aEscribir;				// Creamos un �ndice auxiliar
	indice.aEscribir++;									// aumentamos el �ndice
	if (indice.aEscribir == MAX_EVENTS)	// Si hemos llegado al final de la cola, ponemos el �ndice al 0
		indice.aEscribir = 0;								
	
	// Comprobamos que donde queremos introducir el evento, no hay otro esperando a ser le�do
	if(eventList[indiceAux].ready != TRUE)
	{
		// Preparamos el evento
		eventList[indiceAux].id = idEvento;
		eventList[indiceAux].auxData = auxData;
		eventList[indiceAux].marcaTemporal = clock_gettime(); 
		eventList[indiceAux].ready = TRUE;
		
		return; // Salimos de la funci�n
	}
	
	// Si hay un evento en ese espacio, saltamos un error
	overflowLed();
	end_execution_error();
}

void tratar_alarma(uint32_t auxData){
	switch(auxData){ 				// La acci�n a realizar depende de sus datos auxiliares
		case EV_GPIO_REF:			// Refrescar las salidas de la GPIO
			refrescarSalidas();
			break;
		
		case EV_POWER:				// Poner el procesador en modo Power Down
			PM_power_down();
			reiniciarEstadoAnterior();
			break;
		
		case EV_CHECK_PULS:		// Comprobar la pulsaci�n de los botones
			gestion_eint1(EV_CHECK_PULS);
			gestion_eint2(EV_CHECK_PULS);
			break;
		
		case EV_LED_ERR:			// Apagar el led de error de la GPIO
			quitarLedErr();
			break;
		
		case EV_LATIDO:				// Apagar el led del latido de la GPIO
			latidoLed();
			break;
		
		case EV_FIN_PARTIDA:
			terminar_partida("Partida ha sido muy larga", 25);
			break;
		
		default:
			break;
	}
}

void leer_evento()
{ 	
	event_t id;
	estado_juego_t estado_partida;
	uint32_t auxData;
	int indiceAux, fila, columna, valor;
	
	//disable_isr();
	
	// Sacamos la informaci�n del evento y liberamos su espacio
	indiceAux = indice.aLeer;
	
	id = eventList[indiceAux].id;
	auxData = eventList[indiceAux].auxData;
	eventList[indiceAux].ready = FALSE;
	
	// Aumentamos en 1 el �ndice
	indice.aLeer++;
	
	// Estamos al final de la cola de procesos
	if(indice.aLeer == MAX_EVENTS)
		indice.aLeer = 0;
	
	// enable_isr();		// Rehabilitamos interrupciones
	
	// Acci�n dependendiendo del identificador del evento
	switch (id)
	{
		case SET_ALARMA:
			// Gestionar evento de la alarma
			tratar_alarma(auxData);
			break;
		
		// Gestionar el evento de timer0
		case SET_TIMER_0:
			gestionar_alarmas();
		  break;
		
		// Gestionar interrupci�n externa 1
		case SET_EXT_INT_1:
			switch(estado_energia_actual())
			{
				case DESPIERTO:
					escribirValor();
					break;
					
				case DORMIDO:
					actualizar_estado_energia(NULL_EVENT);
					break;
				
				default:
					break;
			}			
			break;
	
		// Gestionar interrupci�n externa 2
		case SET_EXT_INT_2:
			switch(estado_energia_actual())
			{
				case DESPIERTO:
					eliminarValor();
					break;
					
				case DORMIDO:
					actualizar_estado_energia(NULL_EVENT);
					break;
				
				default:
					break;
			}			
			break;
		
		// Gestionar entrada de la UART
		case SET_UART_SEND_CHR:
			estado_partida = obtener_estado_juego();
			if(estado_partida == principio)
			{
					empezar_partida();
			}
			else
			{
					recibir_caracter(auxData);
			}			
			break;
		
		case SET_WATCHDOG:	
			disable_isr_fiq();
			WD_feed();
			enable_isr_fiq();
			break;
		
		case SET_WRITE_COMMAND:
			valor = auxData & 0xFF;
			columna = (auxData >> 0x8) & 0xFF;
			fila = (auxData >> 0x10) & 0xFF;
			introducirValorCelda(fila, columna, valor);
			candidatos_actualizar();
			dibujar();
			break;
		
		case SET_RESET_COMMAND:
			sudokuReiniciar();
			dibujar();
			break;
		
		default:
			break;
	}
	
	
}

boolean hay_evento()
{
	return eventList[indice.aLeer].ready;
}

void scheduler()
{	
	indice.aLeer = 0;
	indice.aEscribir = 0;
	
	while(1)
	{
		if(hay_evento() == TRUE){
			leer_evento();
		}else{
			PM_idle();
		}		
	}
}
