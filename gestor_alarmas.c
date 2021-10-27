#include <inttypes.h>
#include "eventos.h"

const int TOT_ALARMAS = 8;
static volatile int ocupada[TOT_ALARMAS] = {0,0,0,0,0,0,0,0};

void crear_alarma_unica(uint8_t evento){
	for (int i = 0; i < TOT_ALARMAS; i++){
		if(ocupada[i] == 0){
			int id = SET_ALARMA | i;
			

			break;
		}
	}
}

void crear_alarma_periodica(uint8_t evento){
	for (int i = 0; i < TOT_ALARMAS; i++){
		if(ocupada[i] == 0){
			int id = SET_ALARMA | i;
			

			break;
		}
	}
}

int esPeriodica(uint32_t alarma){
	return alarma & 0x0000800000 >> 5;
}

void gestionar_alarma(uint8_t idEvento, uint32_t auxData){
	// Ejecutar evento
	if (esPeriodica() ){
	}

}
