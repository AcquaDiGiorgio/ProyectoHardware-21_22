#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

	#include "eventos.h"
	// Valores posibles de la máquina de estados
	enum {
		DORMIDO 	= 0,
		DESPIERTO = 1
	};

	int estado_energia_actual(void);								// Devuelve el valor da la máquina de estados
	void actualizar_estado_energia(event_t evento);	// Maquina de estados
	void PM_power_down (void);											// Pone al reloj en modo power down
	void PM_idle (void);														// Pone al reloj en modo idle
	void PM_wakeup (void);													// Despierta al procesador de PowerDown
	extern void Switch_to_PLL(void); 								// Función dentro de Startup.s

#endif //GESTOR_ENERGIA_H
