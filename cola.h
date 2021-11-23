#ifndef COLA_H
#define COLA_H

	#include <stdlib.h>
	#include <inttypes.h>
	#include "eventos.h"

	#define MAX_INTERRUPTIONS 32  // Máximo número de interrupciones en la cola

	// Tipo interrupción
	struct interruption{
		int ready;						// Preparado para ser ejecutado
		event_t id;           // ID del evento
		uint32_t auxData;			// Datos auxiliares del Evento
		int marcaTemporal;		// Momento en el que se ha creado la Interrupción
	};

	// Vector de interrupciones
	static struct interruption interruptionlist[MAX_INTERRUPTIONS];
	
	// Guardado, lectura y compración de existencia de eventos de la cola
	void cola_guardar_eventos(event_t idEvento, uint32_t auxData);
	void leer_evento(void);
	int hay_evento(void);
	
	// Scheduler del sistema de eventos
	void scheduler(void);

#endif //COLA_H
