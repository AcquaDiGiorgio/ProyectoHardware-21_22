#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

	#include "eventos.h"
	// Valores posibles de la m�quina de estados
	enum {
		DORMIDO 	= 0,
		DESPIERTO = 1
	};

	int energia_estado_actual(void);								// Devuelve el valor da la m�quina de estados
	void energia_actualizar_estado(event_t evento);	// Maquina de estados
	void PM_power_down (void);											// Pone al reloj en modo power down
	void PM_idle (void);														// Pone al reloj en modo idle
	void PM_wakeup (void);													// Despierta al procesador de PowerDown
	
	extern void Switch_to_PLL(void); 								// Funci�n dentro de Startup.s
	void end_execution_error(void);
	
#endif //GESTOR_ENERGIA_H
