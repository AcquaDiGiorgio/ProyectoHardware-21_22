#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <inttypes.h>
#include "eventos.h"

#define MAX_INTERRUPTIONS 32  /* the maximal number of processes in the system */

/* Process control block - 
 * holding all process relevant informations 
*/
struct interruption{
	int ready;						// Preparao para ser ejecutado
	event_t id;           // ID del evento
	uint32_t auxData;			// Datos auxiliares del Evento
	int marcaTemporal;		// Momento en el que se ha creado la Interrupción
};

static struct interruption interruptionlist[MAX_INTERRUPTIONS];
void cola_guardar_eventos(event_t idEvento, uint32_t auxData);
void leer_evento(void);
int hay_evento(void);
void scheduler(void);

#endif //COLA_H
