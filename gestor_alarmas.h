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

	static volatile struct alarma_t alarmas[TOT_ALARMAS] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

	//Creaci�n de Alarmas
	void crear_alarma_unica(uint8_t evento, int retardo);
	void crear_alarma_periodica(uint8_t evento, int retardo);

	//Get informaci�n de una alarma
	uint32_t returnAuxData(int evento, int perioica, int retardo);
	int getRetardo(uint32_t auxData);
	int getEvento(uint32_t auxData);
	int esPeriodica(uint32_t auxData);

	//Gesti�n de alarmas y sus eventos
	void gestionar_alarmas(void);
	void gestionar_alarma(int idAlarma, uint32_t auxData);

#endif
