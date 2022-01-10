#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

	#include <inttypes.h>
	#include "eventos.h"
	#include "constantes_comunes.h"

	#define TOT_ALARMAS 10 		// M�ximo n�mero de alarmas
	#define DYNAMIC_ID 	2			// M�nimo �ndice de una alarma no predifinida por el sistema
	
	// Tipo de una alma
	struct alarma_t{
		boolean active; 				// Alarma activa o no
		uint32_t elapsedTime; 	// Marca cuanto tiempo ha pasado en ms
		uint32_t auxData; 			// Informaci�n auxiliar de la alrma
	};
	
	// Id de alarmas predefinidas por el sistema
	enum {
		POW_DOWN 			= 1,			// Alarma que pondr� el sistema a modo Power Down
		LED_CANCELAR  = 2,			// Alarma que cambia el estado del led de cancelar operacion
	};

	// Vector de alarmas
	static volatile struct alarma_t 
		alarmas[TOT_ALARMAS] = {{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},
														{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},{FALSE,0,0},
														{FALSE,0,0},{FALSE,0,0}};

	// Creaci�n de Alarmas
	void alarma_crear_alarma_unica(int id, event_t evento, int retardo);
	void alarma_crear_alarma_periodica(int id, event_t evento, int retardo);

	// Getters informaci�n de una alarma
	uint32_t alarma_returnAuxData(event_t evento, int perioica, int retardo);
	int alarma_getRetardo(uint32_t auxData);
	uint8_t alarma_getEvento(uint32_t auxData);
	int alarma_esPeriodica(uint32_t auxData);

	// Gesti�n de alarmas y sus eventos
	void alarma_gestionar_alarmas(void); 				// Esta funci�n ser� llamada por el scheduler
	void alarma_gestionar_alarma(int idAlarma);	// Esta funci�n se llamar� cuando se haya llegado al tiempo l�mite
	void alarma_parar(int id);
														
	// Inicializaci�n de ciertas alarmas predefinidas por el sistema
	void alarma_inicializarAlarmasDefault(int wd_sec);
	void alarma_add_alarma_PD(void);

#endif
