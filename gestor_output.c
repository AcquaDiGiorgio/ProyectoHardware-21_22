#include "gestor_output.h"
#include "sudoku_p2.h"
#include "cola.h"

#define LEN_FILA 				29
#define TOT_FILAS 			18
#define FILAS_POR_CUAD 	3
#define TOT_CHARS 			812 // 28*28 812
#define MAX_VALORES  9

static const char leyenda[] = {'L','E','Y','E','N','D','A',':','\n',
																'\t','A','c','a','b','a','r',' ','l','a',' ','p','a','r','t','i','d','a',':',' ','#','R','S','T','!','\n',
																'\t','N','u','e','v','a',' ','p','a','r','t','i','d','a',':',' ','#','N','E','W','!','\n',
																'\t','J','u','g','a','d','a',':',' ','#','F','C','V','S','!','\n'};

																			
static const char topRow[LEN_FILA] = {'/','-','-','+','-','-','+','-','-',
																			'+','-','-','+','-','-','+','-','-',
																			'+','-','-','+','-','-','+','-','-','\\','\n'};

static const char midRow[LEN_FILA] = {'+','-','-','+','-','-','+','-','-',
																			'#','-','-','+','-','-','+','-','-',
																			'#','-','-','+','-','-','+','-','-','+','\n'};

static const char lowRow[LEN_FILA] = {'\\','-','-','+','-','-','+','-','-',
																			 '+','-','-','+','-','-','+','-','-',
																			 '+','-','-','+','-','-','+','-','-','/','\n'};

static const char hardRow[LEN_FILA] = {'+','#','#','#','#','#','#','#','#',
																			 '+','#','#','#','#','#','#','#','#',
																			 '+','#','#','#','#','#','#','#','#','+','\n'};

static const int listaTipo[MAX_VALORES] = {1,4,7,10,13,16,19,22,25};

static volatile int sigchar = 0;

static volatile char filas[TOT_FILAS][LEN_FILA];

static volatile char tableroCompleto[TOT_CHARS];	//Vector que guarda el tablero completo del sudoku
																			 
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
			else if (i % 2 != 0)	
			{
				chr = ' ';					// Estamos en un espacio vacío
				
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
	
	pintar_siguiente_caracter();
}

//
//Concatena el cuadrante entero
//
void concat_tablero()
{
    int fila_recorrida = 0;
	int i,x, pos = 0;
	
	//Añade la topRow al vector principal
	for (i = 0; i < LEN_FILA; i++)
	{
			tableroCompleto[pos] = topRow[i];
			pos++;
	}
	
	//Añade una hardRow para que quede mas bonito
	for (x = 0; x < LEN_FILA; x++)
	{
			tableroCompleto[pos] = hardRow[x];
			pos++;
  }
	
	//Añade las tres filas de cudarantes del sudoku
	for (i = 0; i < 3; i++)
	{
	        int fila = 0, i,x;
					//Añade las tres filas de un cuadrante
	        for (fila = 0; fila < 3; fila++)
	        {		
								//Añade los datos de un cuadrante
	             for (x=0; x<2 ;x++)
	             {
			           for (i = 0; i < LEN_FILA; i++)
			           {
			        	    tableroCompleto[pos] = filas[fila_recorrida][i];
				            pos = pos + 1;
		    	        }
		            	fila_recorrida++;
	             }
			
							//Si la fila no es la ultima del cuadrante, añade la midRow
		        	if (fila != 2)
		        	{	
			        		for (i = 0; i < LEN_FILA; i++)
		        			{
			        			tableroCompleto[pos] = midRow[i];
			        			pos = pos + 1;
			        		}
			        }
	        }	
	    //Añadimos la hardRow cuando se termina el cuadrante
			if (i != 2)
			{	
			    for (x = 0; x < LEN_FILA; x++)
	        {
			        tableroCompleto[pos] = hardRow[x];
			        pos++;
          }
			}
	}
	
	//Añade la fila lowRow
	for (i = 0; i < LEN_FILA; i++)
	{
			tableroCompleto[pos] = lowRow[i];
			pos++;
	}
	
}

//Envia el vector de caracteres del tablero a la uart
void pintar_siguiente_caracter()
{
	char chr;
	if (!terminado)
	{
			chr = tableroCompleto[sigchar];
			sigchar++;
			
			if (sigchar == TOT_CHARS)
			{
				terminado = TRUE;
				sigchar = 0;
			}
			
			cola_guardar_eventos(SET_CHAR, chr);
			
	}
	else
	{
			terminado = FALSE;
	}
}
