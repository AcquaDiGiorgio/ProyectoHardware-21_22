#include "gestor_output.h"
#include "sudoku_p2.h"
#include "cola.h"
#include "temporizador.h"
#include "pantalla.h"
	
#define LEN_FILA 								29
#define TOT_FILAS 							9
#define TOT_CUADRANTES  				3
#define LEN_CANDIDATOS					13
#define FILAS_POR_CUADRANTE 		3
#define LEN_CABECERA_CANDIDATOS 30
#define LEN_FIN		 							35
#define LEN_DUR_PARTDIA 				23
#define LEN_VOLVER_A_JUGAR 			36
#define LEN_COSTE_ACTUALIZAR		33

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
static char informacion_juego[INFO_SIZE] 								= {"Este juego es el sudoku, tras apretar una tecla, se le mostrará un tablero con\nciertos valores predefinidos y posibles candidatos\n\nPara introducir una jugada, usa el comando #FCVS!\nSiendo F la fila, C la columna, V el valor y S el resto de\nla división de la suma de los 3 anteriores valores entre 8 => (F+C+V)%8\n\nPara reiniciar el tablero usa el comando #RST!\n\nLa partida terminara tras pasar 1 hora o completar el sudoku sin errores\n\nPresione cualquier tecla para continuar\n"};
static char fin_partida[LEN_FIN] 												= {"\nLa partida ha terminado debido a: "};
static char razon_fin[LEN_DUR_PARTDIA] 									= {"\nLa partida ha durado: "};
static char coste_actualizar[LEN_COSTE_ACTUALIZAR] 			= {"\nCoste de actualizar candidatos: "};
static char volver_a_jugar[LEN_VOLVER_A_JUGAR]					= {"\nEscriba #NEW! para volver a jugar: "};

static volatile char filas[TOT_FILAS][LEN_FILA];
static volatile char tableroCompleto[CUADRICULA_SIZE];	//Vector que guarda el tablero completo del sudoku
static volatile char candidatos[CANDIDATOS_SIZE];

static volatile boolean terminado = FALSE;
static volatile int index_candidatos = 1;

static volatile uint64_t actualizar_coste = 0;

void __swi(0xFF) enable_isr (void);

void enviar_info(void)
{
		pantalla_add_to_buffer(informacion_juego, INFO_SIZE);
}

void mostrar_final(uint8_t minutos, uint8_t segundos, volatile char *razon, int len_razon)
{
		int i;
		char tiempo[8], actualizar_tiempo[9];
	
		tiempo[0] = to_string(minutos / 10);
		tiempo[1] = to_string(minutos % 10);
		tiempo[2] = 'm';
		tiempo[3] = ' ';
		tiempo[4] = to_string(segundos / 10);
		tiempo[5] = to_string(segundos % 10);
		tiempo[6] = 's';
		tiempo[7] = '\n';
	
		for (i = 0; i < 6; i++)
		{
				actualizar_tiempo[i] = to_string(actualizar_coste % (potencia(10, 6-i)) / (potencia(10, 5-i)) );
		}
		
		actualizar_tiempo[6] = 'm';
		actualizar_tiempo[7] = 's';
		actualizar_tiempo[8] = '\n';
	
		pantalla_add_to_buffer(fin_partida, LEN_FIN);
		pantalla_add_to_buffer(razon, len_razon);
		pantalla_add_to_buffer(razon_fin,LEN_DUR_PARTDIA);
		pantalla_add_to_buffer(tiempo, 8);
		pantalla_add_to_buffer(coste_actualizar, LEN_COSTE_ACTUALIZAR);
		pantalla_add_to_buffer(actualizar_tiempo, 9);
		pantalla_add_to_buffer(volver_a_jugar, LEN_VOLVER_A_JUGAR);
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
	
		candidatos_celda = sudoku_leer_candidatos(fila,columna);
	
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
	uint64_t time;
	
	time = clock_gettime();
	candidatos_actualizar(); 
	actualizar_coste = actualizar_coste + clock_gettime() - time;
	
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
					else if ((j - 1) % 3 == 0)		// Estamos en un espacio vacío
					{				
							valor = sudoku_leer_valor(fila,columna);
							chr = to_string(valor);
							
							if (valor == 0)
							{
									chr = ' ';
							}
						
							if(sudoku_es_pista(fila,columna) == FALSE)
							{
									gestor_candidatos(fila, columna);
							}
					}
					else									
					{
							chr = ' ';	
							if(sudoku_es_pista(fila,columna) == TRUE)
							{
									chr = 'P';
							}
							else if (sudoku_hay_error(fila,columna) == TRUE)
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
	
	pantalla_add_to_buffer(tableroCompleto, CUADRICULA_SIZE);
	pantalla_add_to_buffer(leyenda, LEYENDA_SIZE); 
	pantalla_add_to_buffer(cabeceraCandidatos,LEN_CABECERA_CANDIDATOS);
	pantalla_add_to_buffer(candidatos, index_candidatos);
	index_candidatos = 1;
	pantalla_add_to_buffer(comando, COMANDO_SIZE);
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
