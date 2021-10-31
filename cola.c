#include "cola.h"
#include "eventos.h"
#include "gestor_alarmas.h"

static volatile int aLeer = 0;
static volatile int actual = 0;

void cola_guardar_eventos(uint8_t idEvento, uint32_t auxData)
{
	int iteracion = 0;
	int ret = -1;
	if(interruptionlist[actual].ready != 1)
	{
		interruptionlist[actual].id = idEvento;
		interruptionlist[actual].auxData = auxData;
		interruptionlist[actual].ready = 1;
		ret = 0;
		actual++;
		return;
	}
	//ILUMINAR EL LED DE OVERFLOW
}

void leer_evento()
{ 
	interruptionlist[aLeer].ready = 0;
	uint8_t id = interruptionlist[aLeer].id;
	uint32_t auxData = interruptionlist[aLeer].auxData;
	
	aLeer++;
	if(aLeer == MAX_INTERRUPTIONS){
		aLeer = 0;
	}
	
	switch (id)
	{
		case SET_ALARMA:
			break;
		
		case TIMER_0:
			gestionar_alarmas();
		  break;
		/*
		case :
			return 0;
		
		case :
			return 0;
		*/
	}
}

int hay_evento(){
	return interruptionlist[aLeer].ready;
}


int scheduler()
{	
	while(1)
	{
		if(hay_evento() == 1){
			leer_evento();
		}		
	}
	return 0;
}