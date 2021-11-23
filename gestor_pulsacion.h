#ifndef GESTION_PULSACION_H
#define GESTION_PULSACION_H

	#include "eventos.h"

	// Maquinas de estados para las interrupciones externes 1 y 2
	void gestion_eint1(event_t evento);
	void gestion_eint2(event_t evento);

#endif //GESTION_PULSACION_H
