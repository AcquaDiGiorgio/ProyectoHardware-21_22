#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Definición de los IDs de los Eventos Posibles
typedef enum { // 1 byte size max
	SET_ALARMA 	= 0x00, 
	TIMER_0 		= 0x01,
	EXT_INT_1		= 0x02,
	EXT_INT_2		= 0x03,
	EV_GPIO			= 0x04,
	EV_POWER		= 0x05,
	CHECK_PULS	= 0x06
}event_t;

#define NO_AUX_DATA 0x00000000

#endif //EVENTOS_H
