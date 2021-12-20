#ifndef GESTOR_OUTPUT_H
#define GESTOR_OUTPUT_H

	void inicializar_tablero(void);

	void concat_fila(int ini, int *pos_ptr);
	void concat_cuadrante(int cuad, int *pos_ptr);
	void concat_tablero(void);

	void pintar(void);
	int to_string(int val);
	void enviar_info(void);

#endif //GESTOR_OUTPUT_H
