#ifndef COLA_H
#define COLA_H

	#include <inttypes.h>
	#include "eventos.h"
	#include "constantes_comunes.h"

	#define MAX_EVENTS 32  // M�ximo n�mero de interrupciones en la cola

	// Tipo interrupci�n
	struct event_info{
		boolean ready;				// Preparado para ser ejecutado
		event_t id;           // ID del evento
		uint32_t auxData;			// Datos auxiliares del Evento
		int marcaTemporal;		// Momento en el que se ha creado la Interrupci�n
	};

	// Vector de interrupciones
	static struct event_info eventList[MAX_EVENTS];
	
	// Guardado, lectura y compraci�n de existencia de eventos de la cola
	void cola_guardar_eventos(event_t idEvento, uint32_t auxData);
	void leer_evento(void);
	boolean hay_evento(void);
	
	// Scheduler del sistema de eventos
	void scheduler(void);

#endif //COLA_H
