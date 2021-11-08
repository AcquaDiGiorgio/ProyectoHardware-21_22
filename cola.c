#include "cola.h"
#include "eventos.h"
#include "gestor_alarmas.h"

static volatile int aLeer = 0;
static volatile int actual = 0;

void cola_guardar_eventos(event_t idEvento, uint32_t auxData)
{
	if(interruptionlist[actual].ready != 1)
	{
		interruptionlist[actual].id = idEvento;
		interruptionlist[actual].auxData = auxData;
		interruptionlist[actual].ready = 1;
		actual++;
		if (actual == MAX_INTERRUPTIONS){
			actual = 0;
		}
		return;
	}
	//ILUMINAR EL LED DE OVERFLOW
	while(1){}
}

void leer_evento()
{ 
	
	event_t id = interruptionlist[aLeer].id;
	uint32_t auxData = interruptionlist[aLeer].auxData;
	interruptionlist[aLeer].ready = 0;
	
	aLeer++;
	if(aLeer == MAX_INTERRUPTIONS){
		aLeer = 0;
	}
	
	switch (id)
	{
		case SET_ALARMA:
			// Gestionar evento de la alarma
			while(1){}
			break;
		
		case TIMER_0:
			gestionar_alarmas();
		  break;
		
		case EXT_INT:
			// NPI	
			break;
		
		default:
			break;
		
	}
}

int hay_evento(){
	return interruptionlist[aLeer].ready;
}

void scheduler()
{	
	while(1)
	{
		if(hay_evento() == 1){
			leer_evento();
		}		
	}
}
