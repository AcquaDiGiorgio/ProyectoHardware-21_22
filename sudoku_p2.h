/* guarda para evitar inclusiones m?ltiples ("include guard") */
#ifndef SUDOKU_P2_H
#define SUDOKU_P2_H

	#include <inttypes.h>
	#include "celda.h"

	/* Tama?os de la cuadricula */
	/* Se utilizan 16 columnas para facilitar la visualizaci?n */
	enum {NUM_FILAS = 9,
				PADDING = 7,
				NUM_COLUMNAS = NUM_FILAS + PADDING};

	typedef uint16_t CELDA;
	/* La informaci?n de cada celda est? codificada en 16 bits
	 * con el siguiente formato, empezando en el bit m?s significativo (MSB):
	 * 1 bit  PISTA
	 * 1 bit  ERROR
	 * 1 bit  no usado
	 * 9 bits vector CANDIDATOS (0: es candidato, 1: no es candidato)
	 * 4 bits VALOR
	*/ 

	static CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS];

	void candidatos_propagar(uint8_t fila, uint8_t columna);
	int candidatos_actualizar(void);
	
	void introducirValorCelda(uint8_t fila, uint8_t columna, int valor);
	void eliminarValorCelda(uint8_t fila, uint8_t columna);
	
	uint8_t leer_celda(uint8_t fila, uint8_t columna);
	uint16_t leer_candidatos(uint8_t fila, uint8_t columna);
	
	uint8_t es_pista(uint8_t fila, uint8_t columna);
	int celda_correcta(uint8_t fila, uint8_t columna);
	
	void sudokuReiniciar(void);

#endif /* SUDOKU_P2_H */
