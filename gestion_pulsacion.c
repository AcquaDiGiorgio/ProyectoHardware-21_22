#include "boton.h"
#include "eventos.h"
#include "gestion_pulsacion.h"
#include "gestor_alarmas.h"

//Diferentes estados que puede estar un boton
typedef enum {
	NO_PULSADO = 0,
	PULSADO = 1
} estado;

estado procesando_eint1 = NO_PULSADO;
estado procesando_eint2 = NO_PULSADO;

//uint32_t periodo_espera = 50;

int gestionando_eint1()	{
	return procesando_eint1;
}

int gestion_eint1(event_t evento)	{
	if(procesando_eint1 == PULSADO && evento == CHECK_PULS)	{
		if(boton1_pulsado() == 1)	{
			crear_alarma_periodica(1,CHECK_PULS,100); //Preparo la alarma cada 100ms
			boton1_clear();
		}
		else{
			button_clear_nueva_pulsacion_1(); //Indicamos que no hay pulsacion
			boton1_reactivate();
			procesando_eint1 = NO_PULSADO;	// Cambio estado
		}
	}
	else if(evento == EXT_INT_1)	{
		procesando_eint1 = PULSADO;		// Cambio estado 
		crear_alarma_periodica(1,CHECK_PULS,100); //Preparo la alarma cada 100ms
	}
	return gestionando_eint1();
}
