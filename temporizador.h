#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

	#include <inttypes.h>

	// Inicializaci�n de los timers
	void temporizador_iniciar(void);					// Inicia Timer1
	void temporizador_periodo(int periodo);		// Inicia Timer0 con periodo ms
	
	// Iniciaci�n, lectura y parada del timer1
	void temporizador_empezar(void);
	uint64_t clock_gettime(void);
	void temporizador_parar(void);

#endif //TEMPORIZADOR_H
