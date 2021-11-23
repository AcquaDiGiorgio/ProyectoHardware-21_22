#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

	#include <inttypes.h>

	// Inicialización de los timers
	void temporizador_iniciar(void);
	
	// Iniciación, lectura y parada del timer1
	void temporizador_empezar(void);
	uint64_t temporizador_leer(void);
	void temporizador_parar(void);
	
	// No clue
	unsigned int temporizador_periodo(int periodo);

#endif //TEMPORIZADOR_H
