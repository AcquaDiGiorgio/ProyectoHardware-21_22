#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Definición de los IDs de los Eventos Posibles
typedef enum { // 1 byte size max
	SET_ALARMA 		= 0x00, 
	EV_TIMER_0 		= 0x01,
	EV_EXT_INT_1	= 0x02,
	EV_EXT_INT_2	= 0x03,
	EV_GPIO				= 0x04,
	EV_POWER			= 0x05,
	EV_CHECK_PULS	= 0x06,
	EV_LED_ERR		= 0x07
}event_t;

#define NO_AUX_DATA 0x00000000

#endif //EVENTOS_H
