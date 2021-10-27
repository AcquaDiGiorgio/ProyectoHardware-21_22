#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <inttypes.h>

#define MAX_INTERRUPTIONS 32  /* the maximal number of processes in the system */

/* Process control block - 
 * holding all process relevant informations 
*/
struct interruption{
	uint8_t id;                    /* ID of the proces */
	uint32_t auxData;
	int ready;
};

static struct interruption interruptionlist[MAX_INTERRUPTIONS];
void cola_guardar_eventos(uint8_t idEvento, uint32_t auxData);
int leer_evento();
int hay_evento();
int scheduler();

#endif //COLA_H
