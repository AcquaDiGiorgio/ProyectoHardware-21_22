#include "gestor_output.h"
#include "sudoku_p2.h"
#include "cola.h"

#include "uart0.h"
	
#define LEN_FILA 				29
#define TOT_FILAS 			18
#define FILAS_POR_CUAD 	3
#define TOT_CUADRANTES  3
#define TOT_CHARS 			812 // 28*28 812
#define MAX_VALORES  		9
#define NEW_LINE				0x0A
#define TAB							0x09
#define LEYENDA_SIZE		0

//static const int leyenda[LEYENDA_SIZE] = {'L','E','Y','E','N','D','A',':',NEW_LINE,
//																					TAB,'A','c','a','b','a','r',' ','l','a',' ',
//																							'p','a','r','t','i','d','a',':',' ','#','R','S','T','!',NEW_LINE,
//																					TAB,'N','u','e','v','a',' ',
//																							'p','a','r','t','i','d','a',':',' ','#','N','E','W','!',NEW_LINE,
//																					TAB,'J','u','g','a','d','a',':',' ','#','F','C','V','S','!',NEW_LINE};

																			
static const int topRow[LEN_FILA] = {'+','-','-','+','-','-','+','-','-',
																			'+','-','-','+','-','-','+','-','-',
																			'+','-','-','+','-','-','+','-','-','+',NEW_LINE};

static const int midRow[LEN_FILA] = {'+','-','-','+','-','-','+','-','-',
																			'#','-','-','+','-','-','+','-','-',
																			'#','-','-','+','-','-','+','-','-','+',NEW_LINE};

static const int lowRow[LEN_FILA] = {'+','-','-','+','-','-','+','-','-',
																			 '+','-','-','+','-','-','+','-','-',
																			 '+','-','-','+','-','-','+','-','-','+',NEW_LINE};

static const int hardRow[LEN_FILA] = {'+','#','#','#','#','#','#','#','#',
																			 '+','#','#','#','#','#','#','#','#',
																			 '+','#','#','#','#','#','#','#','#','+',NEW_LINE};

static const int listaTipo[MAX_VALORES] = {1,4,7,10,13,16,19,22,25};

static volatile int sigchar = 0;

static volatile int filas[TOT_FILAS][LEN_FILA];

static volatile int tableroCompleto[1000];	//Vector que guarda el tablero completo del sudoku
																			 
static volatile boolean terminado = FALSE;

//Devuelve true si valor esta en la listaTipo
int inListTipo(int valor){
	int x;
  for(x=0; x<MAX_VALORES; x++){
		if(listaTipo[x]==valor){
			return TRUE;
    }
	}
    return FALSE;
}

//													
//Inicializa el tablero para presentarlo por pantalla
//
void inicializar_tablero()
{
	int i, j, fila = 0, columna;
	int chr;
	
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
				
			
			if (j % 9 == 0)						// Estamos en un separador de cuadrante
			{
				chr = '#';
			}
			else if (j % 3 == 0)			// Estamos en un separador de fila
			{
				chr = '|';
			}
			else if (i % 2 != 0)			// Estamos en un espacio vacío
			{
				chr = ' ';							
				
				if(!inListTipo(j))			// Se estamos en una casilla pista/error sobreescribimos
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
				
				if(inListTipo(j))			// Si estamos en una casilla número sobreescribimos
				{
					chr = leer_celda(fila,columna); // TODO: Pasar a string
				}
			}
			
			filas[i][j] = chr;
		}
		
		filas[i][LEN_FILA-2] = '|';
		filas[i][LEN_FILA-1] = '\n';
	}
	
	concat_tablero();
	
	cola_guardar_eventos(SET_CHAR, chr);
}

void concat_tablero()
{
	int i, j, cuadrante, fila, ini, pos;
	pos = 0;
	
	for (i = 0; i < LEN_FILA; i++)
	{
			tableroCompleto[pos] = topRow[i];
			pos++;
	}
	
	for (cuadrante = 0; cuadrante < 3; cuadrante++)	// Recorro los 3 cuadrantes
	{
			
			for (fila = 0; fila < 3; fila++)						// Recorro las 3 filas de este cuadrante
			{
					ini = cuadrante*FILAS_POR_CUAD + fila;
					
					for (i = ini; i < ini + 2; i++)			
					{
							for (j = 0; j < LEN_FILA; j++)
							{
								tableroCompleto[pos] = filas[i][j];
								pos++;
							}			
					}
					
					if (fila != 2)
					{	
							for (i = 0; i < LEN_FILA; i++)
							{
									tableroCompleto[pos] = midRow[i];
									pos = pos + 1;
							}
					}
			}	
		
			if (cuadrante != 2)
			{	
					for (i = 0; i < LEN_FILA; i++){
							tableroCompleto[pos] = hardRow[i];
							pos++;
					}					
			}
	}
	
	for (i = 0; i < LEN_FILA; i++)
	{
			tableroCompleto[pos] = lowRow[i];
			pos++;
	}
}

void pintar(void){
	int chr;
	if (terminado == FALSE)
	{
			chr = tableroCompleto[sigchar];
			sigchar++;
			
			if (sigchar == TOT_CHARS) // sigchar == TOT_CHARS
			{
				terminado = TRUE;
				sigchar = 0;
			}
			
			char_to_uart(chr);
	}
	else
	{
			terminado = FALSE;
	}
}
