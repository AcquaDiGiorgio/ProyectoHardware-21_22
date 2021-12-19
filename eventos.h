#ifndef EVENTOS_H
#define EVENTOS_H
	
	#define NO_AUX_DATA 0x00000000 // El evento a introducir no tiene AuxData

	// Definici�n de los IDs de los Eventos Posibles
	typedef enum { // 1 byte size max
		// Evento nulo, �til para m�quinas de estados
		NULL_EVENT 					= 0x00,		
		
		// Evento de pruebas
		EV_TEST 						= 0xFF,
		
		// Origen del evento
		SET_ALARMA 					= 0x01, 	// El evento ha sido pedido por una alarma
		SET_TIMER_0 				= 0x02,		// El evento ha sido pedido por timer0
		SET_EXT_INT_1				= 0x03,		// El evento ha sido pedido por la interrupci�n externa 1
		SET_EXT_INT_2				= 0x04,		// El evento ha sido pedido por la interrupci�n externa 2
		SET_UART_SEND_CHR		= 0x05,		// Evento env�a un caracter desde la UART 
		SET_CHAR 						= 0x06,		// Evento que pide un car�cter para el gestor_ouput
		SET_UART_CHR_DISP 	= 0x07,		// Evento que dice que se puede volver a escribir en la UART
		SET_WATCHDOG 				= 0x08,
		
		// Actuaci�n del evento (Llamado por un evento de tipo SET)
		EV_GPIO_REF					= 0xA0,		// El evento requiere refrescar las salidas de la IO
		EV_POWER						= 0xA1,		// El evento requiere que se entre en modo PowerDown
		EV_CHECK_PULS				= 0xA2,		// El evento requiere comprobar si elg�n bot�n est� pulsado
		EV_LED_ERR					= 0xA3,		// El evento requiere apagar el led de error de la IO
		EV_LATIDO						= 0xA4		// El evento requiere cambiar el estado del led de latido de la IO
		
	}event_t;

#endif //EVENTOS_H
