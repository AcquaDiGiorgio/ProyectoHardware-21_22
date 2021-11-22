#include <inttypes.h>
#include "gestor_alarmas.h"

#define ON 1
#define OFF 0
#define ALRM_UNICA 0
#define ALRM_PERIODICA 1
#define DYNAMIC_ID 4

void crear_alarma_unica(int id, event_t evento, int retardo){
	if (id == 0){
		int i;
		for (i = DYNAMIC_ID; i < TOT_ALARMAS; i++){
			if(alarmas[i].active == OFF){
				uint32_t auxData = returnAuxData(evento,ALRM_UNICA,retardo);
				alarmas[i].elapsedTime = 0;
				alarmas[i].auxData = auxData;
				alarmas[i].active = ON;
				return;
			}
		}
	}else{
		uint32_t auxData = returnAuxData(evento,ALRM_UNICA,retardo);
		alarmas[id-1].auxData = auxData;
		alarmas[id-1].elapsedTime = 0;
		alarmas[id-1].active = ON;
		return;
	}
	// Todas las alarmas ocupadas
}

void crear_alarma_periodica(int id, event_t evento, int retardo){
	if (id == 0){
		int i;
		for (i = DYNAMIC_ID; i < TOT_ALARMAS; i++){
			if(alarmas[i].active == OFF){
				uint32_t auxData = returnAuxData(evento,ALRM_PERIODICA,retardo);
				alarmas[i].auxData = auxData;
				alarmas[i].active = ON;
				return;
			}
		}
	}else{
		uint32_t auxData = returnAuxData(evento,ALRM_PERIODICA,retardo);
		alarmas[id-1].auxData = auxData;
		alarmas[id-1].active = ON;
		return;
	}
	// Todas las alarmas ocupadas
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
	return (auxData >> 23) & 0x1; // No creo que esté bien
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
// POST: gestiona el evento anclado a la alrma ejecutada y si es periódica
// 			 la reinicia
void gestionar_alarma(int idAlarma){
	// GUARDAMOS EL EVENTO QUE SE TIENE QUE GESTIONAR
	uint32_t auxData = alarmas[idAlarma].auxData;
	
	cola_guardar_eventos(SET_ALARMA, getEvento(auxData));
	
	// SI ES PERIÓDICA, LA REINICIAMOS
	if (esPeriodica(auxData) == 1){
		alarmas[idAlarma].elapsedTime = 0;
	// SINO, LA DESACTIVAMOS
	}else{
		alarmas[idAlarma].active = OFF;
	}
}

void inicializarAlarmasDefault(void){
	crear_alarma_unica(POW_DOWN,EV_POWER,15000);
	crear_alarma_periodica(PULSACION,EV_CHECK_PULS,100);
}
