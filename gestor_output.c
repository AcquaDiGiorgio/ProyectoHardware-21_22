#include "gestor_output.h"
#include "sudoku_p2.h"
#include "cola.h"

#include "uart0.h"
	
#define LEN_FILA 								29
#define TOT_FILAS 							9
#define TOT_CUADRANTES  				3
#define LEN_CANDIDATOS					13
#define FILAS_POR_CUADRANTE 		3
#define LEN_CABECERA_CANDIDATOS 30
#define LEN_FIN		 							34
#define LEN_DUR_PARTDIA 				23

#define CUADRICULA_SIZE 				552
#define LEYENDA_SIZE						64
#define COMANDO_SIZE 						10
#define INFO_SIZE								475
#define CANDIDATOS_SIZE					1134 // 13 * 9 * 9
																																				
static char outerRow[LEN_FILA] 	= {"############################\n"};
static char midRow[LEN_FILA] 		= {"#--+--+--#--+--+--#--+--+--#\n"};
static char hardRow[LEN_FILA] 	= {"#########+########+#########\n"};

static char cabeceraCandidatos[LEN_CABECERA_CANDIDATOS] = {"\n\nF C\tCandidatos\n- -\t---------"};
static char leyenda[LEYENDA_SIZE] 											= {"\nLEYENDA:\n\tReiniciar la partida: #RST!\n\tIntroducir Jugada #FCVS!"};	
static char comando[COMANDO_SIZE] 											= {"\nComando: "};
static char informacion_juego[INFO_SIZE] 								= {"Este juego es el sudoku, tras apretar una tecla, se le mostrar� un tablero con\nciertos valores predefinidos y posibles candidatos\n\nPara introducir una jugada, usa el comando #FCVS!\nSiendo F la fila, C la columna, V el valor y S el resto de\nla divisi�n de la suma de los 3 anteriores valores entre 8 => (F+C+V)%8\n\nPara reiniciar el tablero usa el comando #RST!\n\nLa partida terminara tras pasar 1 hora o completar el sudoku sin errores\n\nPresione cualquier tecla para continuar\n"};
static char fin_partida[LEN_FIN] 												= {"La partida ha terminado debido a: "};
static char razon_fin[LEN_DUR_PARTDIA] 									= {"\nLa partida ha durado: "};

static volatile char filas[TOT_FILAS][LEN_FILA];
static volatile char tableroCompleto[CUADRICULA_SIZE];	//Vector que guarda el tablero completo del sudoku
static volatile char candidatos[CANDIDATOS_SIZE];

static volatile boolean terminado = FALSE;
static volatile int index_candidatos = 1;

void __swi(0xFF) enable_isr (void);

void enviar_info(void)
{
		add_to_buffer(informacion_juego, INFO_SIZE);
}

void mostrar_final(uint8_t minutos, uint8_t segundos, char *razon, int len_razon)
{
		char tiempo[8];
		tiempo[0] = to_string((minutos>>1) & 0x1);
		tiempo[1] = to_string(minutos & 0x1);
		tiempo[2] = 'm';
		tiempo[3] = ' ';
		tiempo[4] = to_string((segundos>>1) & 0x1);
		tiempo[5] = to_string(segundos & 0x1);
		tiempo[6] = 's';
		tiempo[7] = '\n';
	
		add_to_buffer(fin_partida, LEN_FIN);
		add_to_buffer(razon, len_razon);
		add_to_buffer(razon_fin,LEN_DUR_PARTDIA);
		add_to_buffer(tiempo, 8);
		
}

void gestor_candidatos(int fila, int columna)
{
		int candidatos_celda, i;
			
		candidatos[index_candidatos] = to_string(fila+1);
		index_candidatos++;
		candidatos[index_candidatos] = ' ';
		index_candidatos++;
		candidatos[index_candidatos] = to_string(columna+1);
		index_candidatos++;
		candidatos[index_candidatos] = '\t';
		index_candidatos++;	
	
		candidatos_celda = leer_candidatos(fila,columna);
	
		for (i = 0; i < 9; i++)
		{
				if ((candidatos_celda & 0x1) == 0x0)
				{
						candidatos[index_candidatos] = to_string(i+1);
						index_candidatos++;
						candidatos[index_candidatos] = ' ';
						index_candidatos++;
				}
				candidatos_celda = candidatos_celda >> 0x1;
		}
		
		candidatos[index_candidatos] = '\n';
		index_candidatos++;
}

//													
//Inicializa el tablero para presentarlo por pantalla
//
void inicializar_tablero(void)
{
	int fila, valor, j, columna;
	char chr;
	
	for (fila = 0; fila < TOT_FILAS; fila++)
	{
			filas[fila][0] = '#';
			candidatos[0] = '\n';	
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
							valor = leer_celda(fila,columna);
							chr = to_string(valor);
							if(es_pista(fila,columna) == FALSE)
							{
									gestor_candidatos(fila, columna);
							}
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
	
	add_to_buffer(tableroCompleto, CUADRICULA_SIZE);
	add_to_buffer(leyenda, LEYENDA_SIZE); 
	add_to_buffer(cabeceraCandidatos,LEN_CABECERA_CANDIDATOS);
	add_to_buffer(candidatos, index_candidatos);
	index_candidatos = 1;
	add_to_buffer(comando, COMANDO_SIZE);
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
