#include <inttypes.h>
#include "gestor_alarmas.h"

#define ON 1
#define OFF 0

void crear_alarma_unica(uint8_t evento, int retardo){
	for (int i = 0; i < TOT_ALARMAS; i++){
		if(alarmas[i].active == OFF){
			alarmas[i].active = ON;
			
			uint32_t auxData = returnAuxData(evento,0,retardo);
			alarmas[i].auxData = auxData;
			return;
		}
	}
}

void crear_alarma_periodica(uint8_t evento, int retardo){
	for (int i = 0; i < TOT_ALARMAS; i++){
		if(alarmas[i].active == OFF){		
			alarmas[i].active = ON;
			
			uint32_t auxData = returnAuxData(evento,1,retardo);
			alarmas[i].auxData = auxData;
			return;
		}
	}
}

// PRE: id del evento a gestionar, la periodicidad de la alama y su retardo
// POST: Crea los Datos auxilires de una alama
uint32_t returnAuxData(int evento, int perioica, int retardo){
	uint32_t retVal = evento;
	retVal = ( retVal << 1 ) | perioica;
	retVal = ( retVal << 23 ) | retardo;
	return retVal;
}

inline int getRetardo(uint32_t auxData){
	return auxData & 0x7FFFFF;
}

inline int getEvento(uint32_t auxData){
	return (auxData >> 24) & 0xFF;
}

inline int esPeriodica(uint32_t auxData){
	return (auxData >> 23) & 0x1; // No creo que esté bien
}

// PRE: True
// POST: suma 1 en el contador de la alarmas activas y las gestiona si es
// 			 necesario
void gestionar_alarmas(){
	for(int i = 0; i < TOT_ALARMAS; i++){
		if(alarmas[i].active == ON){
			alarmas[i].elapsedTime++;
			if(alarmas[i].elapsedTime == getRetardo(alarmas[i].auxData)){
				gestionar_alarma(i, alarmas[i].auxData);
			}
		}
	}
}

// PRE: id de un Evento a gestionar y los datos de la alarma ejecutada
// POST: gestiona el evento anclado a la alrma ejecutada y si es periódica
// 			 la reinicia
void gestionar_alarma(int idAlarma, uint32_t auxData){
	// GUARDAMOS EL EVENTO QUE SE TIENE QUE GESTIONAR
	cola_guardar_eventos(SET_ALARMA, getEvento(auxData));
	
	// SI ES PERIÓDICA, LA REINICIAMOS
	if (esPeriodica(auxData) == 1){
		alarmas[idAlarma].elapsedTime = 0;
	}else{
		alarmas[idAlarma].active = 0;
	}
}
