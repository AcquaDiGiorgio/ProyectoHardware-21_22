#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

#include <inttypes.h>
#include "eventos.h"
#include "cola.h"
#include "eventos.h"

#define TOT_ALARMAS 8

	struct alarma_t{
		int active;
		int elapsedTime;
		uint32_t auxData;
	};
	
	enum id_alarma{
		GPIO_REFRESH 	= 1,
		POW_DOWN 			= 2,
		PULSACION 		= 3,
		LED_ERROR			= 4,
	};

	static volatile struct alarma_t alarmas[TOT_ALARMAS] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

	//Creación de Alarmas
	void crear_alarma_unica(int id, event_t evento, int retardo);
	void crear_alarma_periodica(int id, event_t evento, int retardo);

	//Get información de una alarma
	uint32_t returnAuxData(event_t evento, int perioica, int retardo);
	int getRetardo(uint32_t auxData);
	uint8_t getEvento(uint32_t auxData);
	int esPeriodica(uint32_t auxData);

	//Gestión de alarmas y sus eventos
	void gestionar_alarmas(void);
	void gestionar_alarma(int idAlarma);

	void inicializarAlarmasDefault(void);
#endif
