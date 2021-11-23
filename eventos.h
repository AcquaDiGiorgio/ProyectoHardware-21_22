#ifndef EVENTOS_H
#define EVENTOS_H
	
	#define NO_AUX_DATA 0x00000000 // El evento a introducir no tiene AuxData

	// Definición de los IDs de los Eventos Posibles
	typedef enum { // 1 byte size max
		NULL_EVENT 		= 0x00,		// Evento nulo, útil para máquinas de estados
		// Origen del evento
		SET_ALARMA 		= 0x01, 	// El evento ha sido pedido por una alarma
		SET_TIMER_0 	= 0x02,		// El evento ha sido pedido por timer0
		SET_EXT_INT_1	= 0x03,		// El evento ha sido pedido por la interrupción externa 1
		SET_EXT_INT_2	= 0x04,		// El evento ha sido pedido por la interrupción externa 2
		// Actuación del evento
		EV_GPIO_REF		= 0x05,		// El evento requiere refrescar las salidas de la IO
		EV_POWER			= 0x06,		// El evento requiere que se entre en modo PowerDown
		EV_CHECK_PULS	= 0x07,		// El evento requiere comprobar si elgún botón está pulsado
		EV_LED_ERR		= 0x08		// El evento requiere apagar el led de error de la IO
	}event_t;

#endif //EVENTOS_H
