#include <inttypes.h>
#include "gestor_alarmas.h"

void crear_alarma_unica(uint8_t evento, int retardo){
	for (int i = 0; i < TOT_ALARMAS; i++){
		if(ocupada[i] == 0){
			uint8_t id = SET_ALARMA;
			uint32_t auxData = returnAuxData(evento,0,retardo);
			dataAlarma[i] = auxData; // CALCULAR RETADO EN CICLOS, NO SEGUNDOS
			
			cola_guardar_eventos(id,auxData);

			return;
		}
	}
}

void crear_alarma_periodica(uint8_t evento, int retardo){
	for (int i = 0; i < TOT_ALARMAS; i++){
		if(ocupada[i] == 0){
			int id = SET_ALARMA;
			
			uint32_t auxData = returnAuxData(evento,1,retardo);

			return;
		}
	}
}

// PRE: id del evento a gestionar, la periodicidad de la alama y su retardo
// POST: Crea los Datos auxilires de una alama
int returnAuxData(int evento, int perioica, int retardo){
	uint32_t retVal = 0;
	// TODO
	return retVal;
}

int getRetardo(uint32_t auxData){
	// TODO
}

int getEvento(uint32_t auxData){
	// TODO	
}

int esPeriodica(uint32_t auxData){
	return auxData & 0x0000800000 >> 5; // No creo que esté bien
}

// PRE: True
// POST: suma 1 en el contador de la alarmas activas y las gestiona si es
// 			 necesario
void gestionar_alarmas(){
	for(int i = 0; i < TOT_ALARMAS; i++){
		if(ocupada[i] == 1){
			valAlarma[i]++;
			if(valAlarma[i] == getRetardo(dataAlarma[i])){
				gestionar_alarma(i, dataAlarma[i]);
			}
		}
	}
}

// PRE: id de un Evento a gestionar y los datos de la alarma ejecutada
// POST: gestiona el evento anclado a la alrma ejecutada y si es periódica
// 			 la reinicia
void gestionar_alarma(int idAlarma, uint32_t auxData){
	// GUARDAMOS EL EVENTO QUE SE TIENE QUE GESTIONAR
	cola_guardar_eventos(getEvento(auxData),0);
	
	// SI ES PERIÓDICA, LA VOLVEMOS A INTRODUCIR
	if (esPeriodica(auxData) == 1){
		cola_guardar_eventos(idAlarma, auxData);
	}else{
		ocupada[idAlarma] = 0;
	}
}
