#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <inttypes.h>

#define MAX_INTERRUPTIONS 32  /* the maximal number of processes in the system */

/* Process control block - 
 * holding all process relevant informations 
*/
struct interruption{
	uint8_t id;                    /* ID of the process */
	uint32_t auxData;
	int ready;
};

static struct interruption interruptionlist[MAX_INTERRUPTIONS];
void cola_guardar_eventos(uint8_t idEvento, uint32_t auxData);
void leer_evento(void);
int hay_evento(void);
void scheduler(void);

#endif //COLA_H
