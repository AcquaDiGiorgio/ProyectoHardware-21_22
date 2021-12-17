#include "sudoku_p2.h"
#include "tableros.h"
#include "cola.h"
#include "temporizador.h"
#include "gestor_alarmas.h"
#include "gestor_IO.h"
#include "boton.h"
#include "uart0.h"

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

/* Función que comprueba que todos los valores de todas las celdas
 * no están ya puestos en esa fila, columna o cuadrado
 *
 * Esta función tiene un elvado coste en cálculo y no se ha usado
 * en esta versión (Práctica 2). O(n*T(Filas)-1*T(Columnas)-1*T(Cuadrado)-1)
 * en el caso peor, siendo n el número de celdas y T(x) el número de
 * celdas que contiene cada x [Filas|Colunmnas|Cuadrados]. En este tipo de
 * sudoku se puede simplificar el coste a O(n*(9^3-3)) = O(726n). 
 */
boolean todas_las_celdas_correctas()
{
    
		uint8_t j, i, k, init_i, init_j, end_i, end_j;
		uint8_t indice, cuadrado_filas, cuadrado_columnas;
		// Metodología parecida a la propagación de candidatos
		const uint8_t init_region[] = {0, 3, 6};

		// Guardamos las ocurrencias de cada numero (índice) en un vector
		// Si hay más de 1 se devuelve False
		uint8_t ocurrencias[NUM_FILAS] = {0,0,0,0,0,0,0,0,0};
		
		// Recorrer fila descartando valor de listas candidatos 
		for(i=0;i<NUM_FILAS;i++)
		{
			for (j=0;j<NUM_COLUMNAS;j++)
			{
				indice = celda_leer_valor(cuadricula[i][j])-1;
				ocurrencias[indice]++;
				if (ocurrencias[indice] > 1) return FALSE;	
			}
			
			// Limpiamos el vector
			for(k=0; k<NUM_FILAS; k++) ocurrencias[k] = 0;
		}
		
		// Recorrer columnas descartando valor de listas candidatos 
		for(j=0;j<NUM_COLUMNAS;j++)
		{
			for (i=0;i<NUM_FILAS;i++)
			{
				indice = celda_leer_valor(cuadricula[i][j])-1;
				ocurrencias[indice]++;
				if (ocurrencias[indice] > 1) return FALSE;
			}
			
			// Limpiamos el vector
			for(k=0; k<NUM_FILAS; k++) ocurrencias[k] = 0;
		}
		
		// Recorrer cuadrados descartando valor de listas candidatos 
		for(cuadrado_filas=0;cuadrado_filas<3;cuadrado_filas++)
		{
			for(cuadrado_columnas=0;cuadrado_columnas<3;cuadrado_columnas++)
			{
				init_i = init_region[cuadrado_filas];
				init_j = init_region[cuadrado_columnas];
				end_i = init_i + 3;
				end_j = init_j + 3;
				
				// En cada cuadrado miramos todos las celdas
				for (i=init_i; i<end_i; i++) {
					for(j=init_j; j<end_j; j++) {
						indice = celda_leer_valor(cuadricula[i][j])-1;
						ocurrencias[indice]++;
						if (ocurrencias[indice] > 1) return FALSE;
					}
				}
				
				// Limpiamos el vector
				for(k=0; k<NUM_FILAS; k++) ocurrencias[k] = 0;
			}
		}
		
	 return TRUE;
}

/* Hace lo mismo que la función comprobar_todas_las_celdas pero
 * para solo 1 celda.
 *
 * Función con poco coste computacional (constante) y usada en esta
 * práctica 2
 */
boolean celda_correcta(uint8_t fila, uint8_t columna)
{
		uint8_t j, i, init_i, init_j, end_i, end_j;
    const uint8_t init_region[] = {0, 3, 6};
		const uint8_t valor = celda_leer_valor(cuadricula[fila][columna]);
		
    /* recorrer fila descartando valor de listas candidatos */
    for(i=0;i<NUM_FILAS;i++)
		{
			if(i!=fila)
			{
				if(valor == celda_leer_valor(cuadricula[i][columna]))
				{
					celda_marcar_error(&cuadricula[fila][columna]);
					return FALSE;
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
					return FALSE;
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
						return FALSE;
					} 
				}	
			}
		}

   return TRUE;
}

void introducirValorCelda(uint8_t fila, uint8_t columna, int valor)
{
		cuadricula[fila][columna] = valor;
}
	
void eliminarValorCelda(uint8_t fila, uint8_t columna)
{
		cuadricula[fila][columna] = 0;
}	
uint8_t leer_celda(uint8_t fila, uint8_t columna)
{
	return celda_leer_valor(cuadricula[fila][columna]);
}

uint16_t leer_candidatos(uint8_t fila, uint8_t columna)
{
	return celda_leer_candidatos(cuadricula[fila][columna]);
}

uint8_t es_pista(uint8_t fila, uint8_t columna)
{
	return celda_es_pista(cuadricula[fila][columna]);
}

uint8_t hay_error(uint8_t fila, uint8_t columna)
{
	return celda_hay_error(cuadricula[fila][columna]);
}
	
void sudokuReiniciar(void)
{
	// Variables de iteración
	uint8_t i;
  uint8_t j;	
	
	// Recorremos todas las celdas
	for (i = 0; i < 9; ++i){
		for (j = 0; j < 9; ++j){
			// Si no es pista
			if(es_pista(i, j) == FALSE)
			{				
				celda_poner_valor(&cuadricula[i][j],0); // Le quitamos el valor
			}
		}
	}

	candidatos_actualizar();
}

boolean celdaAccesible(uint8_t fila, uint8_t columna)
{
	boolean retVal = FALSE;
	
	if(fila <= 8 && columna <= 8)
		retVal = TRUE;
	
	return retVal;
}

int main (void) {
	temporizador_iniciar();		
	temporizador_periodo(1);	// Timer0
	initIO();
	init_serial();
	eint_init();
	inicializarAlarmasDefault();
	candidatos_actualizar();
	scheduler();
}
