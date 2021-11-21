#include "boton.h"
#include "eventos.h"
#include "gestor_pulsacion.h"

//Diferentes estados que puede estar un boton
typedef enum {
	NO_PULSADO = 0,
	PULSADO = 1
} estado;

estado procesando_eint1 = NO_PULSADO;
estado procesando_eint2 = NO_PULSADO;

void gestion_eint1(event_t evento)	{
	
	if (evento == EV_EXT_INT_1){	// Le llama el boton
		procesando_eint1 = PULSADO;		// Cambio estado 
	
	}else{	// Le llama la alarma
		if( boton1_pulsado() == 0 && button_nueva_pulsacion_1() == 1 ){	 // Botón bajado
				button_clear_nueva_pulsacion_1(); //Indicamos que no hay pulsacion
				boton1_reactivate();
				procesando_eint1 = NO_PULSADO;	// Cambio estado
		
		}else{ // Botón mantenido
				boton1_clear();
		}
	}
}

void gestion_eint2(event_t evento)	{
	
	if (evento == EV_EXT_INT_2){	// Le llama el boton
		procesando_eint2 = PULSADO;		// Cambio estado 
	
	}else{	// Le llama la alarma
		if( boton2_pulsado() == 0 && button_nueva_pulsacion_2() == 1 ){	 // Botón bajado
				button_clear_nueva_pulsacion_2(); //Indicamos que no hay pulsacion
				boton2_reactivate();
				procesando_eint2 = NO_PULSADO;	// Cambio estado
		
		}else{ // Botón mantenido
				boton2_clear();
		}
	}
}
