#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Definición de los IDs de los Eventos Posibles
typedef enum { // 1 byte size max
	SET_ALARMA 	= 0x00, 
	TIMER_0 		= 0x01,
	EXT_INT_1		= 0x02,
	EXT_INT_2		= 0x03
	// GPIO?
	// WAKE?
}event_t;

#define NO_AUX_DATA 0x00000000
#define ALRM_UNICA 0
#define ALRM_PERIODICA 1

#endif //EVENTOS_H
