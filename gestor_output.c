#include "gestor_output.h"
#include "sudoku_p2.h"
#include "cola.h"

#define LEN_FILA 				28
#define TOT_FILAS 			18
#define FILAS_POR_CUAD 	3
#define TOT_CHARS 			784 // 28*28

static const char topRow[LEN_FILA] = {'/','-','-','+','-','-','+','-','-',
																			'+','-','-','+','-','-','+','-','-',
																			'+','-','-','+','-','-','+','-','-','\\'};

static const char midRow[LEN_FILA] = {'+','-','-','+','-','-','+','-','-',
																			'#','-','-','+','-','-','+','-','-',
																			'#','-','-','+','-','-','+','-','-','+'};

static const char lowRow[LEN_FILA] = {'\\','-','-','+','-','-','+','-','-',
																			 '+','-','-','+','-','-','+','-','-',
																			 '+','-','-','+','-','-','+','-','-','/'};

static const char hardRow[LEN_FILA] = {'+','#','#','#','#','#','#','#','#',
																			 '+','#','#','#','#','#','#','#','#',
																			 '+','#','#','#','#','#','#','#','#','+'};

static volatile int sigchar = 0;

static volatile char filas[TOT_FILAS][LEN_FILA];
	
static volatile char tableroCompleto[TOT_CHARS];
																			 
static volatile boolean terminado = FALSE;
																			 
void inicializar_tablero()
{
	int i, j, fila = 0, columna;
	char chr;
	for (i = 0; i < TOT_FILAS; i++)
	{
		filas[i][0] = '|';
		
		if (i % 2 == 0)
		{
			fila++;
		}
		
		columna = 0;
		
		for (j = 1; j < LEN_FILA-1; j++)
		{
			if ((j - 1) % 2 == 0)
			{
				columna++;
			}
				
			
			if (j % 9 == 0)				// Estamos en un separador de cuadrante
			{
				chr = '#';
			}
			else if (j % 3 == 0)	// Estamos en un separador de fila
			{
				chr = '|';
			}
			else if (j % 2 == 0)	
			{
				chr = ' ';					// Estamos en un espacio vacío
				
				if(i % 2 == 0)			// Se estamos en una casilla pista/error sobreescribimos
				{
					if (es_pista(fila, columna) == TRUE)
					{
						chr = 'P';
					}
					else if (hay_error(fila,columna) == TRUE)
					{
						chr = 'E';
					}			
				}
			}
			else									
			{
				chr = ' ';					// Estamos en un espacio vacío
				
				if(i % 2 != 0)			// Si estamos en una casilla número sobreescribimos
				{
					chr = to_uint(leer_celda(fila,columna));
				}
			}
			
			filas[i][j] = chr;
		}
		
		filas[i][LEN_FILA-1] = '|';
	}
}

void concat_fila(int ini, int *pos_ptr)
{
	int fila, i;
	for (fila = ini; fila < ini + 2; fila++)
	{
			for (i = 0; i < LEN_FILA; i++)
			{
				tableroCompleto[*pos_ptr] = filas[fila][i];
				*pos_ptr = *pos_ptr + 1;
			}			
	}
}

void concat_cuadrante(int cuad, int *pos_ptr)
{
	int fila = 0, i;
	for (fila = 0; fila < 3; fila++)
	{
			concat_fila(cuad*FILAS_POR_CUAD + fila, pos_ptr);
			
			if (fila != 2)
			{	
					for (i = 0; i < LEN_FILA; i++)
					{
						tableroCompleto[*pos_ptr] = midRow[i];
						*pos_ptr = *pos_ptr + 1;
					}
			}
	}	
	
	concat_tablero();
}

void concat_tablero()
{
	int i, pos, fila = 2;
	
	for (i = 0; i < LEN_FILA; i++)
	{
			tableroCompleto[pos] = topRow[i];
			pos++;
	}
	
	for (i = 0; i < 3; i++)
	{
			concat_cuadrante(i, &pos);
	}
	
	for (i = 0; i < LEN_FILA; i++)
	{
			tableroCompleto[pos] = lowRow[i];
			pos++;
	}
	
}

void pintar_siguiente_caracter()
{
	char chr;
	if (! terminado)
	{
			chr = tableroCompleto[sigchar];
			sigchar++;
			
			if (sigchar == TOT_CHARS)
			{
				terminado = TRUE;
			}
			
			cola_guardar_eventos();
			
	}
	else
	{
	
	}
}