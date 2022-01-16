#ifndef IDLE_POWERDOWN_H
#define IDLE_POWERDOWN_H

	void IPD_power_down(void);											// Pone al reloj en modo power down
	void IPD_idle (void);														// Pone al reloj en modo idle
	void IPD_wakeup (void);													// Despierta al procesador de PowerDown
	
	extern void Switch_to_PLL(void); 								// Función dentro de Startup.s
	void IDP_end_execution_error(void);
	
#endif //IDLE-POWERDOWN_H
