#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

	#include <inttypes.h>

	// Inicializaci�n de los timers
	void temporizador_iniciar(void);					// Inicia Timer1
	void temporizador_periodo(int periodo);		// Inicia Timer0 con periodo ms
	
	// Iniciaci�n, lectura y parada del timer1
	void temporizador_empezar(void);
	uint64_t __swi(0x00) clock_gettime (void);
	void temporizador_parar(void);

	//RTC y Watchdog
	void RTC_init(void);
	uint8_t RTC_leer_minutos(void);
	uint8_t RTC_leer_segundos(void);
	
	void WD_init(int sec);
	void WD_feed(void);
	
#endif //TEMPORIZADOR_H
