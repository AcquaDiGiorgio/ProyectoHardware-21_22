#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

	#include "eventos.h"
	// Valores posibles de la máquina de estados
	enum {
		DORMIDO 	= 0,
		DESPIERTO = 1
	};

	int energia_estado_actual(void);								// Devuelve el valor da la máquina de estados
	void energia_actualizar_estado(event_t evento);	// Maquina de estados
	void energia_power_down(void);
	void energia_idle(void);
	
#endif //GESTOR_ENERGIA_H
