#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

#include <stdlib.h>
#include <inttypes.h>

void PM_power_down (void);
void PM_idle (void);
void PM_wakeup (void);

#endif //GESTOR_ENERGIA_H