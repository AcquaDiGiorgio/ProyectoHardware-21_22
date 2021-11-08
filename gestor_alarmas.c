#include <inttypes.h>
#include "gestor_alarmas.h"

#define ON 1
#define OFF 0

void crear_alarma_unica(event_t evento, int retardo){
	int i;
	for (i = 0; i < TOT_ALARMAS; i++){
		if(alarmas[i].active == OFF){
			uint32_t auxData = returnAuxData(evento,ALRM_UNICA,retardo);
			alarmas[i].auxData = auxData;
			alarmas[i].active = ON;
			return;
		}
	}
	// Todas las alarmas ocupadas
}

void crear_alarma_periodica(event_t evento, int retardo){
	int i;
	for (i = 0; i < TOT_ALARMAS; i++){
		if(alarmas[i].active == OFF){		
			uint32_t auxData = returnAuxData(evento,ALRM_PERIODICA,retardo);
			alarmas[i].auxData = auxData;
			alarmas[i].active = ON;
			return;
		}
	}
}

// PRE: id del evento a gestionar, la periodicidad de la alama y su retardo
// POST: Crea los Datos auxilires de una alama
uint32_t returnAuxData(event_t evento, int perioica, int retardo){
	uint32_t retVal = evento;
	retVal = ( retVal << 1 ) | perioica;
	retVal = ( retVal << 23 ) | retardo;
	return retVal;
}

int getRetardo(uint32_t auxData){
	return auxData & 0x7FFFFF;
}

uint8_t getEvento(uint32_t auxData){
	return (auxData >> 24) & 0xFF;
}

int esPeriodica(uint32_t auxData){
	return (auxData >> 23) & 0x1; // No creo que est� bien
}

// PRE: True
// POST: suma 1 en el contador de la alarmas activas y las gestiona si es
// 			 necesario
void gestionar_alarmas(){
	int i;
	for(i = 0; i < TOT_ALARMAS; i++){
		if(alarmas[i].active == ON){
			alarmas[i].elapsedTime++;
			if(alarmas[i].elapsedTime >= getRetardo(alarmas[i].auxData)){
				gestionar_alarma(i);
			}
		}
	}
}

// PRE: id de un Evento a gestionar y los datos de la alarma ejecutada
// POST: gestiona el evento anclado a la alrma ejecutada y si es peri�dica
// 			 la reinicia
void gestionar_alarma(int idAlarma){
	// GUARDAMOS EL EVENTO QUE SE TIENE QUE GESTIONAR
	uint32_t auxData = alarmas[idAlarma].auxData;
	
	cola_guardar_eventos(SET_ALARMA, getEvento(auxData));
	
	// SI ES PERI�DICA, LA REINICIAMOS
	if (esPeriodica(auxData) == 1){
		alarmas[idAlarma].elapsedTime = 0;
	// SINO, LA DESACTIVAMOS
	}else{
		alarmas[idAlarma].active = 0;
	}
}
