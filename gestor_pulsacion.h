#ifndef GESTION_PULSACION_H
#define GESTION_PULSACION_H

#include <stdlib.h>
#include <inttypes.h>
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "boton.h"
#include "eventos.h"

void gestion_eint1(event_t evento);
void gestion_eint2(event_t evento);

#endif //GESTION_PULSACION_H
