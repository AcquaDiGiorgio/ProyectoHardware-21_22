#include "gestor.h"

static volatile int primero = 0;

void cola_guardar_eventos(uint8_t idEvento, uint32_t auxData)
{
	int iteracion = 0;
	int i = primero+1;
	int ret = -1;
	while(iteracion < MAX_INTERRUPTIONS)
	{
		if(i == MAX_INTERRUPTIONS){i=0;}
		if(interruptionlist[i].ready != 1)
		{
			interruptionlist[i].id = idEvento;
			interruptionlist[i].auxData = auxData;
			interruptionlist[i].ready = 1;
			ret = 0;
			return;
		}
		iteracion++;
		i++;
	}
	//ILUMINAR EL LED DE OVERFLOW
}

int leer_evento()
{ 
	interruptionlist[primero].ready = 0;
	uint8_t id = interruptionlist[primero].id;
	uint32_t auxData = interruptionlist[primero].auxData;
	
	primero++;
	if(primero == MAX_INTERRUPTIONS){
		primero = 0;
	}
	
	// EJECUTAR EL EVENTO
	return 0;
}

int hay_evento(){
	return interruptionlist[primero].ready;
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