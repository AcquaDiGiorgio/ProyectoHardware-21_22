#include "cola.h"
#include "temporizador.h"
#include "gestor_pulsacion.h"
#include "eventos.h"
#include "gestor_alarmas.h"
#include "gestor_IO.h"
#include "gestor_energia.h"

static volatile int aLeer = 0;
static volatile int actual = 0;

void cola_guardar_eventos(event_t idEvento, uint32_t auxData)
{
	if(interruptionlist[actual].ready != 1)
	{
		interruptionlist[actual].id = idEvento;
		interruptionlist[actual].auxData = auxData;
		interruptionlist[actual].marcaTemporal = temporizador_leer(); 
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
			switch(auxData){
				case EV_GPIO:
					refrescarSalidas();
					break;
				
				case EV_POWER:
					PM_power_down();
					break;
				
				case EV_CHECK_PULS:					
					gestion_eint1(EV_CHECK_PULS);
					gestion_eint2(EV_CHECK_PULS);
					break;
				
				case EV_LED_ERR:
					quitarLedErr();
					break;
				
				default:
					break;
			}
			break;
		
		case EV_TIMER_0:
			gestionar_alarmas();
		  break;
		
		case EV_EXT_INT_1:
			escribirValor();
			break;
		
		case EV_EXT_INT_2:
			eliminarValor();
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
		}else{
			//actualizar_estado_energia();
		}		
	}
}
