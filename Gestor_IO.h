#ifndef GESTOR_IO_H
#define GESTOR_IO_H

	#include <inttypes.h>

	// Inicia la IO de la GPIO
	void initIO(void);

	// Refresca las salidas del sistema
	void refrescarSalidas(void);
	
	// Escribe/Borra un valor de la celda leída de la GPIO
	void escribirValor(void);
	void eliminarValor(void);
	
	// Encer led de error
	void ponerLedErr(void);
	
	// Quita el led de error
	void quitarLedErr(void);
	
	// Comprueba que se quiere finalizar la partida
	void checkFinPartida(uint8_t fila, uint8_t columna, uint8_t valor);
	
	// Ha habido overflow en alguna parte del sistema (encer led)
	void overflow(void);

#endif //GESTOR_IO_H
