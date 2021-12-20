#include "gestor_output.h"
#include "sudoku_p2.h"
#include "cola.h"

#include "uart0.h"
	
#define LEN_FILA 							29
#define TOT_FILAS 						9
#define TOT_CUADRANTES  			3
#define FILAS_POR_CUADRANTE 	3
#define TOT_CHARS 						551 // 19 * 29

#define LEYENDA_SIZE					0
#define COMANDO_SIZE 					10


//static const int leyenda[LEYENDA_SIZE] = {'L','E','Y','E','N','D','A',':',NEW_LINE,
//																					TAB,'A','c','a','b','a','r',' ','l','a',' ',
//																							'p','a','r','t','i','d','a',':',' ','#','R','S','T','!',NEW_LINE,
//																					TAB,'N','u','e','v','a',' ',
//																							'p','a','r','t','i','d','a',':',' ','#','N','E','W','!',NEW_LINE,
//																					TAB,'J','u','g','a','d','a',':',' ','#','F','C','V','S','!',NEW_LINE};

																			
static const int topRow[LEN_FILA] = {'#','#','#','#','#','#','#','#','#',
																		 '#','#','#','#','#','#','#','#','#',
																		 '#','#','#','#','#','#','#','#','#','#',NEW_LINE};

static const int midRow[LEN_FILA] = {'#','-','-','+','-','-','+','-','-',
																		 '#','-','-','+','-','-','+','-','-',
																		 '#','-','-','+','-','-','+','-','-','#',NEW_LINE};

static const int lowRow[LEN_FILA] = {'#','#','#','#','#','#','#','#','#',
																		 '#','#','#','#','#','#','#','#','#',
																		 '#','#','#','#','#','#','#','#','#','#',NEW_LINE};

static const int hardRow[LEN_FILA] = {'#','#','#','#','#','#','#','#','#',
																			'+','#','#','#','#','#','#','#','#',
																			'+','#','#','#','#','#','#','#','#','#',NEW_LINE};

static const int comando[COMANDO_SIZE] = {NEW_LINE,'C','o','m','a','n','d','o',':',' '};

static volatile int sigchar = 0;

static volatile int filas[TOT_FILAS][LEN_FILA];

static volatile int tableroCompleto[1000];	//Vector que guarda el tablero completo del sudoku
																			 
static volatile boolean terminado = FALSE;

//													
//Inicializa el tablero para presentarlo por pantalla
//
void inicializar_tablero()
{
	int fila, j, columna;
	int chr;
	
	for (fila = 0; fila < TOT_FILAS; fila++)
	{
		filas[fila][0] = '#';
		columna = -1;
		
		for (j = 1; j < LEN_FILA-1; j++)
		{
			if ((j - 1) % 3 == 0)
			{
				columna++;
			}				
			
			if (j % 9 == 0)							// Estamos en un separador de cuadrante
			{
				chr = '#';
			}
			else if (j % 3 == 0)				// Estamos en un separador de fila
			{
				chr = '|';
			}
			else if ((j - 1) % 3 == 0)		// Estamos en un espacio vacío
			{
				chr = to_string(leer_celda(fila,columna));
			}
			else									
			{
				chr = ' ';	
				if(es_pista(fila,columna) == TRUE)
				{
					chr = 'P';
				}
				else if (hay_error(fila,columna) == TRUE)
				{	
					chr = 'E';
				}
			}
			
			filas[fila][j] = chr;
		}
		
		filas[fila][LEN_FILA-2] = '#';
		filas[fila][LEN_FILA-1] = NEW_LINE;
	}
	
	concat_tablero();	
	cola_guardar_eventos(SET_INIT_SUDOKU, NO_AUX_DATA);
}



void concat_tablero()
{
		int i, j, cuadrante, fila, filaReal, pos;
		pos = 0;
		
		for (i = 0; i < LEN_FILA; i++)										// Guardo la fila de arriba
		{
				tableroCompleto[pos] = topRow[i];
				pos++;
		}
		
		for (cuadrante = 0; cuadrante < TOT_CUADRANTES; cuadrante++)		// Recorro los 3 cuadrantes
		{				
				for (fila = 0; fila < FILAS_POR_CUADRANTE; fila++)							// Recorro las 3 filas de este cuadrante
				{
						filaReal = cuadrante * TOT_CUADRANTES + fila;
						
						for (j = 0; j < LEN_FILA; j++)
						{
							tableroCompleto[pos] = filas[filaReal][j];	// Guardo la info de esta fila
							pos++;
						}			
						
						for (i = 0; i < LEN_FILA && fila < 2; i++)
						{
								tableroCompleto[pos] = midRow[i];			// Dibujo el separador de filas
								pos = pos + 1;
						}
						
				}
				
				for (i = 0; i < LEN_FILA && cuadrante < 2; i++){
						tableroCompleto[pos] = hardRow[i];
						pos++;
				}			
		}
		
		for (i = 0; i < LEN_FILA; i++)
		{
				tableroCompleto[pos] = lowRow[i];
				pos++;
		}
		
		for (i = 0; i < COMANDO_SIZE; i++)
		{
			tableroCompleto[pos] = comando[i];
			pos++;
		}
}

void pintar(void){
	int chr;
	if (terminado == FALSE)
	{
			chr = tableroCompleto[sigchar];
			sigchar++;
			
			if (sigchar == TOT_CHARS+COMANDO_SIZE) // sigchar == TOT_CHARS
			{
				terminado = TRUE;
				sigchar = 0;
			}
			
			cola_guardar_eventos(SET_CHAR, chr);
			// char_to_uart(chr);
	}
	else
	{
			terminado = FALSE;
	}
}
