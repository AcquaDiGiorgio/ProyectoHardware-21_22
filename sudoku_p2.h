/* guarda para evitar inclusiones m?ltiples ("include guard") */
#ifndef SUDOKU_P2_H
#define SUDOKU_P2_H

	#include <inttypes.h>
	#include "celda.h"
	#include "constantes_comunes.h"

	/* Tamaños de la cuadricula */
	/* Se utilizan 16 columnas para facilitar la visualizaci?n */
	enum {NUM_FILAS = 9,
				PADDING = 7,
				NUM_COLUMNAS = NUM_FILAS + PADDING};

	typedef uint16_t CELDA;
	/* La información de cada celda está codificada en 16 bits
	 * con el siguiente formato, empezando en el bit más significativo (MSB):
	 * 1 bit  PISTA
	 * 1 bit  ERROR
	 * 1 bit  no usado
	 * 9 bits vector CANDIDATOS (0: es candidato, 1: no es candidato)
	 * 4 bits VALOR
	*/ 
	
	static CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS];

	// Propagación y actualización de los candidatos
	void candidatos_propagar(uint8_t fila, uint8_t columna);
	int candidatos_actualizar(void);
	
	// Introducción y eliminación del valor de una celda
	void introducirValorCelda(uint8_t fila, uint8_t columna, int valor);
	void eliminarValorCelda(uint8_t fila, uint8_t columna);
	
	// Lectura del valor y los candidatos de una celda
	uint8_t leer_celda(uint8_t fila, uint8_t columna);
	uint16_t leer_candidatos(uint8_t fila, uint8_t columna);
	
	// Obtención de información de una celda
	uint8_t es_pista(uint8_t fila, uint8_t columna);
	boolean celda_correcta(uint8_t fila, uint8_t columna);
	
	// Reinicio del sudoku
	void sudokuReiniciar(void);
	
	// Check de si la celda a acceder está en el rango de celdas
	boolean celdaAccesible(uint8_t fila, uint8_t columna);
	uint8_t hay_error(uint8_t fila, uint8_t columna);

#endif /* SUDOKU_P2_H */
