#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

#include <inttypes.h>
#include "eventos.h"
#include "cola.h"
#include "eventos.h"

#define TOT_ALARMAS 8

static volatile int ocupada[TOT_ALARMAS] = {0,0,0,0,0,0,0,0};
static volatile int valAlarma[TOT_ALARMAS] = {0,0,0,0,0,0,0,0};
static volatile uint32_t dataAlarma[TOT_ALARMAS] = {0,0,0,0,0,0,0};

void crear_alarma_unica(uint8_t evento, int retardo);
void crear_alarma_periodica(uint8_t evento, int retardo);
int returnAuxData(int evento, int perioica, int retardo);
int esPeriodica(uint32_t auxData);
void gestionar_alarmas();
void gestionar_alarma(int idAlarma, uint32_t auxData);

#endif