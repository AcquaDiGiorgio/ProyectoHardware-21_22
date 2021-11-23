#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

enum {
	MODO_POWER_DOWN = 0,
	MODO_AFTER_POWER_DOWN = 1,
	MODO_NORMAL = 2
};

int estado_energia_actual(void);
void actualizar_estado_energia(void);
void PM_power_down (void);
void PM_idle (void);
void PM_wakeup (void);
extern void Switch_to_PLL(void);

#endif //GESTOR_ENERGIA_H
