#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

	#include <inttypes.h>
	#include "eventos.h"

	#define TOT_ALARMAS 8 		// M�ximo n�mero de alarmas
	#define ON 1 							// Alarma activa
	#define OFF 0 						// Alarma apagada
	#define ALRM_UNICA 0			// Alarma que solo se debe ejcutar 1 vez
	#define ALRM_PERIODICA 1	// Alarma que se debe reiniciar tras su ejecuci�n
	#define DYNAMIC_ID 4			// M�nimo �ndice de una alarma no predifinida por el sistema

	// Tipo de una alma
	struct alarma_t{
		int active; 			// Se recomienda usar ON/OFF
		int elapsedTime;  // Marca cuanto tiempo ha pasado en ms
		uint32_t auxData; // Informaci�n auxiliar de la alrma
	};
	
	// Id de alarmas predefinidas por el sistema
	enum id_alarma{
		GPIO_REFRESH 	= 1,	// Alarma que refrescar� las salidas de la GPIO
		POW_DOWN 			= 2,	// Alarma que pondr� el sistema a modo Power Down
		PULSACION 		= 3,	// Alarma que comprobar� si ha habido una nueva pulsaci�n
		LED_ERROR			= 4,	// Alarma que bajar� el led de error
	};

	// Vector de alarmas
	static volatile struct alarma_t 
		alarmas[TOT_ALARMAS] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

	// Creaci�n de Alarmas
	void crear_alarma_unica(int id, event_t evento, int retardo);
	void crear_alarma_periodica(int id, event_t evento, int retardo);

	// Getters informaci�n de una alarma
	uint32_t returnAuxData(event_t evento, int perioica, int retardo);
	int getRetardo(uint32_t auxData);
	uint8_t getEvento(uint32_t auxData);
	int esPeriodica(uint32_t auxData);

	// Gesti�n de alarmas y sus eventos
	void gestionar_alarmas(void); 				// Esta funci�n ser� llamada por el scheduler
	void gestionar_alarma(int idAlarma);	// Esta funci�n se llamar� cuando se haya llegado al tiempo l�mite

	// Inicializaci�n de ciertas alarmas predefinidas por el sistema
	void inicializarAlarmasDefault(void);
#endif
