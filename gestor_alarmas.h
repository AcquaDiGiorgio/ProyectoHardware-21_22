#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

	#include <inttypes.h>
	#include "eventos.h"
	#include "constantes_comunes.h"

	#define TOT_ALARMAS 8 		// Máximo número de alarmas

	// Tipo de una alma
	struct alarma_t{
		boolean active; 				// Alarma activa o no
		uint32_t elapsedTime; 			 	// Marca cuanto tiempo ha pasado en ms
		uint32_t auxData; 			// Información auxiliar de la alrma
	};
	
	// Id de alarmas predefinidas por el sistema
	enum {
		GPIO_REFRESH 	= 1,			// Alarma que refrescará las salidas de la GPIO
		POW_DOWN 			= 2,			// Alarma que pondrá el sistema a modo Power Down
		PULSACION 		= 3,			// Alarma que comprobará si ha habido una nueva pulsación
		LED_ERROR			= 4,			// Alarma que bajará el led de error
		LATIDO				= 5,
		WATCHDOG			= 6
	};

	// Vector de alarmas
	static volatile struct alarma_t 
		alarmas[TOT_ALARMAS] = {{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},{FALSE,0,0}};

	// Creación de Alarmas
	void alarma_crear_alarma_unica(int id, event_t evento, int retardo);
	void alarma_crear_alarma_periodica(int id, event_t evento, int retardo);

	// Getters información de una alarma
	uint32_t alarma_returnAuxData(event_t evento, int perioica, int retardo);
	int alarma_getRetardo(uint32_t auxData);
	uint8_t alarma_getEvento(uint32_t auxData);
	int alarma_esPeriodica(uint32_t auxData);

	// Gestión de alarmas y sus eventos
	void alarma_gestionar_alarmas(void); 				// Esta función será llamada por el scheduler
	void alarma_gestionar_alarma(int idAlarma);	// Esta función se llamará cuando se haya llegado al tiempo límite

	// Inicialización de ciertas alarmas predefinidas por el sistema
	void alarma_inicializarAlarmasDefault(int wd_sec);
#endif
