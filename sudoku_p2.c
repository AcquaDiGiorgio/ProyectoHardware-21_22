#include "sudoku_p2.h"
#include "tableros.h"
#include "cola.h"
#include "temporizador.h"
#include "gestor_alarmas.h"
#include "gestor_IO.h"
#include "boton.h"
#include "uart0.h"
#include "gestor_output.h"
#include "partida.h"
#include "pantalla.h"

// Cuadrícula a utilizar
// Para cambiarla, modificar cuadricula_C_C por el tablero deseado
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
 * y devuelve en celdas_vacias el n?mero de celdas vacias
 */
int candidatos_actualizar(void)
{
  int celdas_vacias = 0;
	uint8_t i;
  uint8_t j;	
	
	for (i = 0; i < 9; ++i){
			for (j = 0; j < 9; ++j){
					eliminar_candidatos(&cuadricula[i][j]);				
					sudoku_celda_correcta(i,j);
			}
	}
	
	for (i = 0; i < 9; ++i){
			for (j = 0; j < 9; ++j){
					if ( celda_leer_valor(cuadricula[i][j]) != 0 ){
							candidatos_propagar(i,j);
					}else{
							celdas_vacias++;
					}
			}
	}
	return celdas_vacias;
}

void comprobar_errores(uint8_t fila, uint8_t columna)
{
    uint8_t i, j, init_i, init_j, end_i, end_j;
	
    /* puede ayudar esta "look up table" a mejorar el rendimiento */
    const uint8_t init_region[NUM_FILAS] = {0, 0, 0, 3, 3, 3, 6, 6, 6};

    /* recorrer fila descartando valor de listas candidatos */
    for (j=0;j<NUM_FILAS;j++)
				sudoku_celda_correcta(fila, j);

    /* recorrer columna descartando valor de listas candidatos */
    for (i=0;i<NUM_FILAS;i++)
				sudoku_celda_correcta(i, columna);

    /* determinar fronteras región */
    init_i = init_region[fila];
    init_j = init_region[columna];
    end_i = init_i + 3;
    end_j = init_j + 3;

    /* recorrer region descartando valor de listas candidatos */
    for (i=init_i; i<end_i; i++) {
				for(j=init_j; j<end_j; j++) {
						sudoku_celda_correcta(i, j);
				}
    }
}

/* Comprueba que una celda tiene el mismo valor que una con una en la
 * misma fila, columna o cuadrante
 *
 * Función con poco coste computacional (constante) y usada en esta
 * práctica 2
 */
boolean sudoku_celda_correcta(uint8_t fila, uint8_t columna)
{
		uint8_t j, i, valor, init_i, init_j, end_i, end_j;
		const uint8_t init_region[NUM_FILAS] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
		boolean correcto = TRUE;

		valor = celda_leer_valor(cuadricula[fila][columna]);
		celda_quitar_error(&cuadricula[fila][columna]);
		
		if (valor == 0)
		{
				return TRUE;
		}
		
		/* recorrer fila descartando valor de listas candidatos */
		for(i=0;i<NUM_FILAS;i++)
		{
				if(i!=fila)
				{
						if(valor == celda_leer_valor(cuadricula[i][columna]))
						{
								celda_marcar_error(&cuadricula[fila][columna]);
								celda_marcar_error(&cuadricula[i][columna]);
								correcto = FALSE;
						} 
				}				
		}

		for(j=0;j<NUM_COLUMNAS;j++)
		{
				if(j!=columna)
				{
						if(valor == celda_leer_valor(cuadricula[fila][j]))
						{
								celda_marcar_error(&cuadricula[fila][columna]);
								celda_marcar_error(&cuadricula[fila][j]);
								correcto = FALSE;
						} 
				}	
		}

		init_i = init_region[fila];
		init_j = init_region[columna];
		end_i = init_i + 3;
		end_j = init_j + 3;

		 /* recorrer region descartando valor de listas candidatos */
		for (i=init_i; i<end_i; i++) {
				for(j=init_j; j<end_j; j++) {
						if(i!= fila && j!=columna)
						{
								if(valor == celda_leer_valor(cuadricula[i][j]))
								{
										celda_marcar_error(&cuadricula[fila][columna]);
										celda_marcar_error(&cuadricula[i][j]);
										correcto = FALSE;
								} 
						}	
				}
		}
		
		return correcto;
}

void sudoku_introducir_valor(uint8_t fila, uint8_t columna, int valor)
{
		if( sudoku_es_pista(fila,columna) == FALSE )
		{
				cuadricula[fila][columna] = valor;
				comprobar_errores(fila, columna);
		}	
}
	
void sudoku_eliminar_valor(uint8_t fila, uint8_t columna)
{
		if( sudoku_es_pista(fila,columna) == FALSE )
		{
				cuadricula[fila][columna] = 0;
		}
}	
uint8_t sudoku_leer_valor(uint8_t fila, uint8_t columna)
{
		return celda_leer_valor(cuadricula[fila][columna]);
}

uint16_t sudoku_leer_candidatos(uint8_t fila, uint8_t columna)
{
		return celda_leer_candidatos(cuadricula[fila][columna]);
}

uint8_t sudoku_es_pista(uint8_t fila, uint8_t columna)
{
		return celda_es_pista(cuadricula[fila][columna]);
}

uint8_t sudoku_hay_error(uint8_t fila, uint8_t columna)
{
		return celda_hay_error(cuadricula[fila][columna]);
}
	
void sudoku_reiniciar(void)
{
		// Variables de iteración
		uint8_t i;
		uint8_t j;	
		
		// Recorremos todas las celdas
		for (i = 0; i < 9; ++i){
				for (j = 0; j < 9; ++j){
						// Si no es pista
						if(sudoku_es_pista(i, j) == FALSE)
						{				
								celda_vaciar(&cuadricula[i][j]); // Le quitamos el valor
						}
				}
		}

		candidatos_actualizar();
}

boolean sudoku_celdaAccesible(uint8_t fila, uint8_t columna)
{
		boolean retVal = FALSE;
		
		if(fila <= 8 && columna <= 8)
				retVal = TRUE;
		
		return retVal;
}

int main (void) {
		temporizador_iniciar();		
		temporizador_periodo(1);	// Timer0
		IO_init();
		uart_init(pantalla_add_to_buffer, pantalla_write_buffer);
		RTC_init();
		eint_init();
		WD_init(20);
		alarma_inicializarAlarmasDefault(10);
		partida_preprar();
		scheduler();
}
