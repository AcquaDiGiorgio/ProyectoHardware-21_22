#include "cola.h"
#include "temporizador.h"
#include "gestor_pulsacion.h"
#include "gestor_alarmas.h"
#include "gestor_IO.h"
#include "gestor_energia.h"

// �ndices del vector de eventos
static volatile int aLeer = 0;		// Evento que toca procesar
static volatile int actual = 0;		// �ndice al cual se le va a introducir un nuevo evento

void cola_guardar_eventos(event_t idEvento, uint32_t auxData)
{
	// Comprobamos que donde queremos introducir el evento, no hay otro esperando a ser le�do
	if(eventList[actual].ready != TRUE)
	{
		// Preparamos el evento
		eventList[actual].id = idEvento;
		eventList[actual].auxData = auxData;
		eventList[actual].marcaTemporal = temporizador_leer(); 
		eventList[actual].ready = TRUE;
		
		// aumentamos el �ndice
		actual++;
		
		// Si hemos llegado al final de la cola, ponemos el �ndice al 0
		if (actual == MAX_EVENTS)
		{
			actual = 0;
		}
		return; // Salimos de la funci�n
	}
	
	// Si hay un evento en ese espacio, saltamos un error
	overflow();
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
			switch(auxData){ 			// La acci�n a realizar depende de sus datos auxiliares
				case EV_GPIO_REF:			// Refrescar las salidas de la GPIO
					refrescarSalidas();
					break;
				
				case EV_POWER:				// Poner el procesador en modo Power Down
					PM_power_down();
					break;
				
				case EV_CHECK_PULS:		// Comprobar la pulsaci�n de los botones
					gestion_eint1(EV_CHECK_PULS);
					gestion_eint2(EV_CHECK_PULS);
					break;
				
				case EV_LED_ERR:			// Apagar el led de error de la GPIO
					quitarLedErr();
					break;
				
				default:
					break;
			}
			break;
		
		// Gestionar el evento de timer0
		case SET_TIMER_0:
			gestionar_alarmas();
		  break;
		
		// Gestionar interrupci�n externa 1
		case SET_EXT_INT_1:
			escribirValor();
			break;
	
		// Gestionar interrupci�n externa 2
		case SET_EXT_INT_2:
			eliminarValor();
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
