#ifndef GESTOR_IO_H
#define GESTOR_IO_H

	#include <inttypes.h>

	// Inicia la IO de la GPIO
	void IO_init(void);

	// Refresca las salidas del sistema
	void IO_refrescarSalidas(void);
	
	// Escribe/Borra un valor de la celda leída de la GPIO
	void IO_escribirValor(void);
	void IO_eliminarValor(void);
	
	// Encer led de error
	void IO_ponerLedErr(void);
	
	// Quita el led de error
	void IO_quitarLedErr(void);
	
	// Comprueba que se quiere finalizar la partida
	void IO_checkFinPartida(uint8_t fila, uint8_t columna, uint8_t valor);
	
	// Ha habido overflow en alguna parte del sistema (encer led)
	void IO_overflowLed(void);
	
	void IO_reiniciarEstadoAnterior(void);
	void IO_latidoLed(void);

#endif //GESTOR_IO_H
