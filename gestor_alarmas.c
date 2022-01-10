#include <inttypes.h>
#include "gestor_alarmas.h"
#include "cola.h"

#define ALRM_UNICA 0			// Alarma que solo se debe ejcutar 1 vez
#define ALRM_PERIODICA 1	// Alarma que se debe reiniciar tras su ejecución


void __swi(0xFF) enable_isr (void);

void alarma_crear_alarma_unica(int id, event_t evento, int retardo)
{
	if (id == 0)
	{
		int i;
		for (i = DYNAMIC_ID; i < TOT_ALARMAS; i++)
		{
			if(alarmas[i].active == FALSE)
			{
				uint32_t auxData = alarma_returnAuxData(evento,ALRM_UNICA,retardo);
				alarmas[i].elapsedTime = 0;
				alarmas[i].auxData = auxData;
				alarmas[i].active = TRUE;
				return;
			}
		}
	}
	else
	{
		uint32_t auxData = alarma_returnAuxData(evento,ALRM_UNICA,retardo);
		alarmas[id-1].auxData = auxData;
		alarmas[id-1].elapsedTime = 0;
		alarmas[id-1].active = TRUE;
		return;
	}
	// Todas las alarmas ocupadas
}

void alarma_crear_alarma_periodica(int id, event_t evento, int retardo)
{
		if (id == 0)
		{
				int i;
				for (i = DYNAMIC_ID; i < TOT_ALARMAS; i++)
				{
						if(alarmas[i].active == FALSE)
						{
								uint32_t auxData = alarma_returnAuxData(evento,ALRM_PERIODICA,retardo);
								alarmas[i].auxData = auxData;
								alarmas[i].active = TRUE;
								return;
						}
				}
		}
		else
		{
				uint32_t auxData = alarma_returnAuxData(evento,ALRM_PERIODICA,retardo);
				alarmas[id-1].auxData = auxData;
				alarmas[id-1].active = TRUE;
				return;
		}
		// Todas las alarmas ocupadas
}

void alarma_parar(int id)
{
		alarmas[id].active = FALSE;
}

// PRE:  id del evento a gestionar, la periodicidad de la alama y su retardo
// POST: Crea los Datos auxilires de una alama
uint32_t alarma_returnAuxData(event_t evento, int perioica, int retardo)
{
		uint32_t retVal = evento;
		retVal = ( retVal << 1 ) | perioica;
		retVal = ( retVal << 23 ) | retardo;
		return retVal;
}

int alarma_getRetardo(uint32_t auxData)
{
		return auxData & 0x7FFFFF;
}

uint8_t alarma_getEvento(uint32_t auxData)
{
		return (auxData >> 24) & 0xFF;
}

int alarma_esPeriodica(uint32_t auxData)
{
		return (auxData >> 23) & 0x1;
}

// PRE:  True
// POST: suma 1 en el contador de la alarmas activas y las gestiona si es
// 			 necesario
void alarma_gestionar_alarmas()
{
		int i;
		for(i = 0; i < TOT_ALARMAS; i++)
		{
				if(alarmas[i].active == TRUE)
				{
						alarmas[i].elapsedTime++;
						if(alarmas[i].elapsedTime >= alarma_getRetardo(alarmas[i].auxData))
						{
								alarma_gestionar_alarma(i);
						}
				}
		}
}

// PRE:  id de un Evento a gestionar y los datos de la alarma ejecutada
// POST: gestiona el evento anclado a la alrma ejecutada y si es periódica
// 			 la reinicia
void alarma_gestionar_alarma(int idAlarma)
{	
	// Guardamos el evento que se tiene que gestionar
		uint32_t auxData = alarmas[idAlarma].auxData;
		
		cola_guardar_eventos(SET_ALARMA, alarma_getEvento(auxData), USER);
		
		// Si la alarma es periódica, reiniciamos su tiempo
		if (alarma_esPeriodica(auxData) == 1)
		{
				alarmas[idAlarma].elapsedTime = 0;
		}		
		else	// Sino, la desactivamos
		{
				alarma_parar(idAlarma);
		}
}

void alarma_add_alarma_PD(void)
{
		alarma_crear_alarma_unica(POW_DOWN,EV_POWER,15 * SEGUNDO);
}

void alarma_inicializarAlarmasDefault(int wd_sec)
{
	alarma_crear_alarma_periodica(0,EV_CHECK_PULS,100);
	alarma_crear_alarma_periodica(0,EV_LATIDO,200);
	alarma_crear_alarma_periodica(0,EV_FEED_WATCHDOG,wd_sec);
}
