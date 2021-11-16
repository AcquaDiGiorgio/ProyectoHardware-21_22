#ifndef GESTION_PULSACION_H
#define GESTION_PULSACION_H

#include <stdlib.h>
#include <inttypes.h>
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "boton.h"
#include "eventos.h"

int gestionando_eint1(void);
int gestion_eint1(event_t evento);

#endif //GESTION_PULSACION_H