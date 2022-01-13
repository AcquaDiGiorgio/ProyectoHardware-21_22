#ifndef GESTOR_OUTPUT_H
#define GESTOR_OUTPUT_H

	#include <inttypes.h>

	void inicializar_tablero(void);

	void concat_fila(int ini, int *pos_ptr);
	void concat_cuadrante(int cuad, int *pos_ptr);
	void concat_tablero(void);

	void pintar(void);
	void enviar_info(void);
	void mostrar_final(uint8_t minutos, uint8_t segundos, uint32_t actualizar_coste, volatile char *razon, int len_razon);

#endif //GESTOR_OUTPUT_H
