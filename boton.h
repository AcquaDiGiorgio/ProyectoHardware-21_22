#ifndef BOTON_H
#define BOTON_H

	#include "constantes_comunes.h"

	//Funcion para inicializar los botones
	void eint_init (void);

	//Devuelve el valor de la variable para detectar una nueva pulsacion
	int boton1_nueva_pulsacion(void);
	int boton2_nueva_pulsacion(void);

	//Resetea la variable correspondiente a 0
	void button_clear_nueva_pulsacion_1(void);
	void button_clear_nueva_pulsacion_2(void);

	//Devuelve si el boton correspondiente esta pulsado o no
	boolean boton1_pulsado(void);
	boolean boton2_pulsado(void);

	//Limpia la interrupcion del boton
	void boton1_clear(void);
	void boton2_clear(void);

	//Vuelve a activar el boton correspondiente para que vuelva a recibir interrupciones
	void boton1_reactivate(void);
	void boton2_reactivate(void);

#endif //BOTON_H
