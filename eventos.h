#ifndef EVENTOS_H
#define EVENTOS_H
	#define NO_AUX_DATA 0x00000000 // El evento a introducir no tiene AuxData

	// Definici�n de los IDs de los Eventos Posibles
	typedef enum { // 1 byte size max
		NULL_EVENT 		= 0x00,		// Evento nulo, �til para m�quinas de estados
		// Origen del evento
		SET_ALARMA 		= 0x01, 	// El evento ha sido pedido por una alarma
		SET_TIMER_0 	= 0x02,		// El evento ha sido pedido por timer0
		SET_EXT_INT_1	= 0x03,		// El evento ha sido pedido por la interrupci�n externa 1
		SET_EXT_INT_2	= 0x04,		// El evento ha sido pedido por la interrupci�n externa 2
		// Actuaci�n del evento
		EV_GPIO_REF		= 0x05,		// El evento requiere refrescar las salidas de la IO
		EV_POWER			= 0x06,		// El evento requiere que se entre en modo PowerDown
		EV_CHECK_PULS	= 0x07,		// El evento requiere comprobar si elg�n bot�n est� pulsado
		EV_LED_ERR		= 0x08		// El evento requiere apagar el led de error de la IO
	}event_t;

#endif //EVENTOS_H
