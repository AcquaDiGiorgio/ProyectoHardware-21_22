#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

enum {
	MODO_IDLE = 0,
	MODO_NORMAL = 1
};

void PM_power_down (void);
void PM_idle (void);
void PM_wakeup (void);
extern void Switch_to_PLL(void);

#endif //GESTOR_ENERGIA_H
