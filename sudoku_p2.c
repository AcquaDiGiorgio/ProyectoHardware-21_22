#include <stddef.h>
#include "sudoku_p2.h"
#include "tableros.h"
#include "cola.h"
#include "gestor_alarmas.h"
#include "temporizador.h"
#include "boton.h"
#include "gestor_IO.h"

#define cuadricula cuadricula_C_C

/* *****************************************************************************
 * propaga el valor de una determinada celda en C
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y regi?n */
/* Recibe como parametro la cuadricula, y la fila y columna de
 * la celda a propagar; no devuelve nada
 */
void candidatos_propagar(uint8_t fila, uint8_t columna)
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

    /* determinar fronteras regi�n */
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
 * y devuelve en celdas_vacias el n?mero de celdas vacias
 */
static int candidatos_actualizar_c()
{
  int celdas_vacias = 0;
	uint8_t i;
  uint8_t j;	
	
	for (i = 0; i < 9; ++i){
		for (j = 0; j < 9; ++j){
				elimnar_candidatos(cuadricula[i][j]);	
		}
	}
	
	for (i = 0; i < 9; ++i){
		for (j = 0; j < 9; ++j){
			if ( celda_leer_valor(cuadricula[i][j]) != 0x0000 ){
				candidatos_propagar(i,j);
			}else{
				celdas_vacias++;
			}
		}
	}
	return celdas_vacias;
}

static int
cuadricula_candidatos_verificar(CELDA solucion[NUM_FILAS][NUM_COLUMNAS])
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

void introducirValorCelda(uint8_t fila, uint8_t columna, int valor){
		cuadricula[fila][columna] = valor;
}
	

void eliminarValorCelda(uint8_t fila, uint8_t columna){
		cuadricula[fila][columna] = 0;
}	

uint8_t leer_celda(uint8_t fila, uint8_t columna){
	return celda_leer_valor(cuadricula[fila][columna]);
}

uint16_t leer_candidatos(uint8_t fila, uint8_t columna){
	return celda_leer_candidatos(cuadricula[fila][columna]);
}

uint8_t es_pista(uint8_t fila, uint8_t columna){
	return celda_es_pista(cuadricula[fila][columna]);
}
	
int main (void) {
	temporizador_iniciar();
	initIO();
	eint_init();
	inicializarAlarmasDefault();
	
	scheduler();
}