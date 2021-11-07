#include <stddef.h>
#include "sudoku_2021.h"


extern int candidatos_actualizar_arm_c(CELDA cuadricula_ARM_C[NUM_FILAS][NUM_COLUMNAS]);
extern int candidatos_actualizar_arm(CELDA cuadricula_ARM_ARM[NUM_FILAS][NUM_COLUMNAS]);

/* *****************************************************************************
 * propaga el valor de una determinada celda en C
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y regi�n */
/* Recibe como parametro la cuadricula, y la fila y columna de
 * la celda a propagar; no devuelve nada
 */
void candidatos_propagar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
	uint8_t fila, uint8_t columna)
{
    uint8_t j, i , init_i, init_j, end_i, end_j;
    /* puede ayudar esta "look up table" a mejorar el rendimiento */
    const uint8_t init_region[NUM_FILAS] = {0, 0, 0, 3, 3, 3, 6, 6, 6};

    /* valor que se propaga */
    uint8_t valor = celda_leer_valor(cuadricula[fila][columna]);

    /* recorrer fila descartando valor de listas candidatos */
    for (j=0;j<NUM_FILAS;j++) 
			celda_eliminar_candidato(&cuadricula[fila][j],valor);

    /* recorrer columna descartando valor de listas candidatos */
    for (i=0;i<NUM_FILAS;i++)
			celda_eliminar_candidato(&cuadricula[i][columna],valor);

    /* determinar fronteras región */
    init_i = init_region[fila];
    init_j = init_region[columna];
    end_i = init_i + 3;
    end_j = init_j + 3;

    /* recorrer region descartando valor de listas candidatos */
    for (i=init_i; i<end_i; i++) {
      for(j=init_j; j<end_j; j++) {
	      celda_eliminar_candidato(&cuadricula[i][j],valor);
	    }
    }
}

/* *****************************************************************************
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */

/* Init del sudoku en codigo C invocando a propagar en C
 * Recibe la cuadricula como primer parametro
 * y devuelve en celdas_vacias el n�mero de celdas vacias
 */
static int candidatos_actualizar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS])
{
  int celdas_vacias = 0;
	uint8_t i;
  uint8_t j;	
	
	for (i = 0; i < 9; ++i){
		for (j = 0; j < 9; ++j){
				cuadricula[i][j] = cuadricula[i][j] & 0x007F;	
		}
	}
	
	for (i = 0; i < 9; ++i){
		for (j = 0; j < 9; ++j){
			if ( celda_leer_valor(cuadricula[i][j]) != 0x0000 ){
				candidatos_propagar_c(cuadricula,i,j);
			}else{
				celdas_vacias++;
			}
		}
	}
	return celdas_vacias;
}

/* Init del sudoku en codigo C invocando a propagar en arm
 * Recibe la cuadricula como primer parametro
 * y devuelve en celdas_vacias el n�mero de celdas vacias
 */
static int
candidatos_actualizar_c_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS])
{
	int celdas_vacias = 0;
	uint8_t i;
  uint8_t j;	
	
	for (i = 0; i < 9; ++i){
		for (j = 0; j < 9; ++j){
				cuadricula[i][j] = cuadricula[i][j] & 0x007F;	
		}
	}
	
	for (i = 0; i < 9; ++i){
		for (j = 0; j < 9; ++j){
			if ( celda_leer_valor(cuadricula[i][j]) != 0x0000 ){
				candidatos_propagar_arm(cuadricula,i,j);
			}else{
				celdas_vacias++;
			}
		}
	}
	return celdas_vacias;
}

static int
cuadricula_candidatos_verificar(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
	 CELDA solucion[NUM_FILAS][NUM_COLUMNAS])
{
    int correcto = 1;
    uint8_t i;
    uint8_t j;

    for(i=0; i < NUM_FILAS && 1 == correcto; ++i) {
       for(j=0; j < NUM_FILAS && 1 == correcto; ++j) {
					correcto = cuadricula[i][j] == solucion[i][j];
       }
    }
    return correcto;
}

/* ************************************************************************
 * programa principal del juego que recibe el tablero
 */


int
sudoku9x9(CELDA cuadricula_C_C[NUM_FILAS][NUM_COLUMNAS],
	CELDA cuadricula_C_ARM[NUM_FILAS][NUM_COLUMNAS],
	CELDA cuadricula_ARM_ARM[NUM_FILAS][NUM_COLUMNAS],
	CELDA cuadricula_ARM_C[NUM_FILAS][NUM_COLUMNAS],
	CELDA solucion[NUM_FILAS][NUM_COLUMNAS])
{
    int celdas_vacias[4];     //numero de celdas aun vacias
    int correcto = 0;
	  size_t i;
    /* calcula lista de candidatos, versi�n C */
    celdas_vacias[0] = candidatos_actualizar_c(cuadricula_C_C);

    /* Init C con propagar arm */
    celdas_vacias[1] = candidatos_actualizar_c_arm(cuadricula_C_ARM);

    /* Init arm con propagar arm */
    celdas_vacias[2] = candidatos_actualizar_arm(cuadricula_ARM_ARM);

    /* Init arm con propagar c */
    celdas_vacias[3] = candidatos_actualizar_arm_c(cuadricula_ARM_C);
	
	  for (i=1; i < 4; ++i) {
			if (celdas_vacias[i] != celdas_vacias[0]) {
				return -1;
			}
		}

    /* verificar que la lista de candidatos C_C calculada es correcta */
    correcto = cuadricula_candidatos_verificar(cuadricula_C_C,solucion);
    correcto += cuadricula_candidatos_verificar(cuadricula_ARM_ARM,solucion);
    correcto += cuadricula_candidatos_verificar(cuadricula_C_ARM,solucion);
    correcto += cuadricula_candidatos_verificar(cuadricula_ARM_C,solucion);
    return correcto;
}


// MAIN

//int main (void) {
//   //#include "tableros.h"
//   //int correcto = sudoku9x9(cuadricula_C_C, cuadricula_C_ARM, cuadricula_ARM_ARM, cuadricula_ARM_C, solucion);
//	
//}
	
