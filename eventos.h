#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Definición de los IDs de los Eventos Posibles

typedef enum { // 1 byte size max
	SET_ALARMA 	= 0x00, 
	TIMER_0 		= 0x01,
	EXT_INT 		= 0x02
}event_t;

#define NO_AUX_DATA 0x0
#define UNICA 0
#define PERIODICA 1

#endif //EVENTOS_H
