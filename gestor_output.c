#include "gestor_output.h"
#include "sudoku_p2.h"
#include "cola.h"

#include "uart0.h"
	
#define LEN_FILA 							29
#define TOT_FILAS 						9
#define TOT_CUADRANTES  			3
#define FILAS_POR_CUADRANTE 	3

#define CUADRICULA_SIZE 			552
#define LEYENDA_SIZE					64
#define COMANDO_SIZE 					10
#define INFO_SIZE							311
#define CANDIDATOS_SIZE				100
																																				
static const char outerRow[LEN_FILA] 	= {"############################\n"};
static const char midRow[LEN_FILA] 		= {"#--+--+--#--+--+--#--+--+--#\n"};
static const char hardRow[LEN_FILA] 	= {"###+###+###+###+###+###+####\n"};

static char leyenda[LEYENDA_SIZE] = {"\nLEYENDA:\n\tReiniciar la partida: #RST!\n\tIntroducir Jugada #FCVS!"};	
static char comando[COMANDO_SIZE] = {"\nComando: "};

static volatile char informacion_juego[INFO_SIZE] = {"Este juego es el sudoku, tras apretar una tecla, se le mostrar� un tablero con ciertos valores predefinidos y posibles candidatos\n\nPara introducir una jugada, usa el comando #FCVS!\nSiendo F la fila, C la columna, V el valor y S el resto de la divisi�n de la suma de los 3 anteriores valores entre 8 => (F+C+V)%8"};


static volatile int filas[TOT_FILAS][LEN_FILA];

static volatile char tableroCompleto[CUADRICULA_SIZE];	//Vector que guarda el tablero completo del sudoku

static volatile int candidatos[CANDIDATOS_SIZE];

static volatile boolean terminado = FALSE;

void __swi(0xFF) enable_isr (void);

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
			else if ((j - 1) % 3 == 0)		// Estamos en un espacio vac�o
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
	recibir_buffer(tableroCompleto, CUADRICULA_SIZE);
	recibir_buffer(leyenda, LEYENDA_SIZE); 
	recibir_buffer(comando, COMANDO_SIZE);
}

void concat_tablero()
{
		int i, j, cuadrante, fila, filaReal, pos;
		pos = 1;
		
		tableroCompleto[0] = NEW_LINE;
	
		for (i = 0; i < LEN_FILA; i++)										// Guardo la fila de arriba
		{
				tableroCompleto[pos] = outerRow[i];
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
				tableroCompleto[pos] = outerRow[i];
				pos++;
		}
}
