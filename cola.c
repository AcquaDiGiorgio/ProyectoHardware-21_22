#include "cola.h"
#include "temporizador.h"
#include "gestor_pulsacion.h"
#include "gestor_alarmas.h"
#include "gestor_IO.h"
#include "gestor_energia.h"
#include "gestor_comandos.h"

// �ndices del vector de eventos
static volatile int aLeer = 0;		// Evento que toca procesar
static volatile int actual = 0;		// �ndice al cual se le va a introducir un nuevo evento

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);

void cola_guardar_eventos(event_t idEvento, uint32_t auxData)
{
	disable_isr();						// Deshabilitamos interrupciones
	
	int indiceAux = actual;		// Creamos un �ndice auxiliar
	actual++;									// aumentamos el �ndice
	if (actual == MAX_EVENTS)	// Si hemos llegado al final de la cola, ponemos el �ndice al 0
		actual = 0;
	
	enable_isr();							// Rehabilitamos interrupciones
	
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
	overflow();
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
		
		default:
			break;
	}
}

void leer_evento()
{ 	
	// Sacamos la informaci�n del evento y liberamos su espacio
	event_t id = eventList[aLeer].id;
	uint32_t auxData = eventList[aLeer].auxData;
	eventList[aLeer].ready = FALSE;
	
	// Aumentamos en 1 el �ndice
	aLeer++;
	
	// Estamos al final de la cola de procesos
	if(aLeer == MAX_EVENTS){
		aLeer = FALSE;
	}
	
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
		case SET_UART:
			recibir_caracter(auxData);
			break;

		default:
			break;
		
	}
}

boolean hay_evento()
{
	return eventList[aLeer].ready;
}

void scheduler()
{	
	while(1)
	{
		if(hay_evento() == TRUE){
			leer_evento();
		}else{
			PM_idle();
		}		
	}
}
