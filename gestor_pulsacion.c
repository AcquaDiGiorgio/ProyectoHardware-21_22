#include "gestor_pulsacion.h"
#include "boton.h"
#include "constantes_comunes.h"

//Diferentes estados que puede estar un boton
typedef enum {
	NO_PULSADO 	= 0,
	PULSADO 		= 1
} estado;

// Estado actual del botón
estado procesando_eint1 = NO_PULSADO;
estado procesando_eint2 = NO_PULSADO;

void gestion_eint1(event_t evento)
{
	
	if (evento == SET_EXT_INT_1) 		// Le llama el boton
	{		
		procesando_eint1 = PULSADO;		// Cambio estado 
	}
	else // Le llama la alarma
	{
		if( boton1_pulsado() == FALSE && boton1_nueva_pulsacion() == TRUE ) // Botón bajado
		{	
			button_clear_nueva_pulsacion_1(); 													// Indicamos que no hay pulsacion
			boton1_reactivate();																				// Reactivamos las interrupciones
			procesando_eint1 = NO_PULSADO;															// Cambio estado	
		}
		else
		{
			boton1_clear(); // Se intenta bajar
		}
	}
}

void gestion_eint2(event_t evento)
{
	if (evento == SET_EXT_INT_2) // Le llama el boton
	{
		procesando_eint2 = PULSADO;		// Cambio estado 
	
	}
	else // Le llama la alarma
	{	
		if( boton2_pulsado() == FALSE && boton2_nueva_pulsacion() == TRUE ) // Botón bajado
		{	
				button_clear_nueva_pulsacion_2(); 													//Indicamos que no hay pulsacion
				boton2_reactivate();																				// Reactivamos las interrupciones
				procesando_eint2 = NO_PULSADO;															// Cambio estado
		}
		else
		{
			boton2_clear(); // Se intenta bajar
		}
	}
}
