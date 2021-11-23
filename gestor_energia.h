#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

	// Valores posibles de la máquina de estados
	enum {
		DORMIDA = 0,
		DESPIERTA = 1
	};

	void PM_power_down (void);				// Pone al reloj en modo power down
	void PM_idle (void);							// Pone al reloj en modo idle
	void PM_wakeup (void);						// D
	extern void Switch_to_PLL(void); 	// Función dentro de Startup.s

#endif //GESTOR_ENERGIA_H
