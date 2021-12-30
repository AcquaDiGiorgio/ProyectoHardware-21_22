/* guarda para evitar inclusiones m?ltiples ("include guard") */
#ifndef SUDOKU_P2_H
#define SUDOKU_P2_H

	#include <inttypes.h>
	#include "celda.h"
	#include "constantes_comunes.h"

	/* Tama�os de la cuadricula */
	/* Se utilizan 16 columnas para facilitar la visualizaci?n */
	enum {NUM_FILAS = 9,
				PADDING = 7,
				NUM_COLUMNAS = NUM_FILAS + PADDING};

	typedef uint16_t CELDA;
	/* La informaci�n de cada celda est� codificada en 16 bits
	 * con el siguiente formato, empezando en el bit m�s significativo (MSB):
	 * 1 bit  PISTA
	 * 1 bit  ERROR
	 * 1 bit  no usado
	 * 9 bits vector CANDIDATOS (0: es candidato, 1: no es candidato)
	 * 4 bits VALOR
	*/ 
	
	static CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS];

	// Propagaci�n y actualizaci�n de los candidatos
	void candidatos_propagar(uint8_t fila, uint8_t columna);
	int candidatos_actualizar(void);
	
	// Introducci�n y eliminaci�n del valor de una celda
	void sudoku_introducir_valor(uint8_t fila, uint8_t columna, int valor);
	void sudoku_eliminar_valor(uint8_t fila, uint8_t columna);
	
	// Lectura del valor y los candidatos de una celda
	uint8_t sudoku_leer_valor(uint8_t fila, uint8_t columna);
	uint16_t sudoku_leer_candidatos(uint8_t fila, uint8_t columna);
	
	// Obtenci�n de informaci�n de una celda
	uint8_t sudoku_es_pista(uint8_t fila, uint8_t columna);
	boolean sudoku_celda_correcta(uint8_t fila, uint8_t columna);
	
	// Reinicio del sudoku
	void sudoku_reiniciar(void);
	
	// Check de si la celda a acceder est� en el rango de celdas
	boolean sudoku_celdaAccesible(uint8_t fila, uint8_t columna);
	uint8_t sudoku_hay_error(uint8_t fila, uint8_t columna);

#endif /* SUDOKU_P2_H */
