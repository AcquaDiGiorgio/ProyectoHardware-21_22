#include <LPC210X.H>
#include <inttypes.h>
#include "gestor_comandos.h"
#include "constantes_comunes.h"

#define MAX_COMMAND_SIZE 5
#define WRITE_BUFFER_SIZE 10

volatile char comando[WRITE_BUFFER_SIZE];
int posicion_actual = 0;


// Limpia el buffer por completo
void reiniciar_comando(void)
{
	for(int i = 0; i < WRITE_BUFFER_SIZE; i++)
	{
		comando[i] = '\0';
	}
	posicion_actual = 0;
}

//
// Introduce un carácter al comando y detecta posibles errores
// 
// Está función debe ser llamada por el scheduler
//
void recibir_caracter(char chr)
{
	comando[posicion_actual] = chr; //Se escribe el caracter en el buffer
	posicion_actual ++;
	
	if (chr == '!')																	// Final de un comando
	{
		detectar_comando();
		reiniciar_comando();
	}
	else if (posicion_actual >= MAX_COMMAND_SIZE)		// Comando muy grande
	{
		reiniciar_comando();
		lanzar_error(OVERSIZED_COMMAND);
	}
}

//
//Devuelve true si el comando es valido y se corresponde con #RST!
//
boolean detectar_reset(int pos_limit){
	//El comando es valido si la posicion es mayor o igual a 4
	if((pos_limit - MAX_COMMAND_SIZE-1) >= 0){
		
		//Las posiciones tienen que coincidir
		if(comando[pos_limit-4] == '#' && comando[pos_limit-3] == 'R' && comando[pos_limit-2] == 'S' && comando[pos_limit-1] == 'T'){
			return TRUE;
		} else {
			return FALSE;
		}
		
	} else {
		return FALSE;
	}
}

//
//Devuelve true si el comando es valido y se corresponde con #NEW!
//
boolean detectar_new(int pos_limit){
	//El comando es valido si la posicion es mayor o igual a 4
	if((pos_limit - MAX_COMMAND_SIZE-1) >= 0){
		
		//Las posiciones tienen que coincidir
		if(comando[pos_limit-4] == '#' && comando[pos_limit-3] == 'N' && comando[pos_limit-2] == 'E' && comando[pos_limit-1] == 'W'){
			return TRUE;
		} else {
			return FALSE;
		}
		
	} else {
		return FALSE;
	}
}

//
//Devuelve true si el comando es valido y se corresponde con #FCVS!
//
boolean detectar_jugada(int pos_limit){
	//El comando es valido si la posicion es mayor o igual a 5
	if((pos_limit - MAX_COMMAND_SIZE) >= 0){
		
		//Las posiciones tienen que coincidir
		if(comando[pos_limit-5] == '#'){
			//Comprobamos que todos los valores son correctos
			int fila = to_uint(comando[pos_limit-4]), columna = to_uint(comando[pos_limit-3]), valor = to_uint(comando[pos_limit-2]), checksum = to_uint(comando[pos_limit-1]);
			if( (0 < fila <= 9) &&  (0 < columna <= 9) && (0 < valor <= 9) && ((fila+columna+valor % 8) == checksum)){
				return TRUE;
			}
			else{
				return FALSE;
			}
		} else {
			return FALSE;
		}
		
	} else {
		return FALSE;
	}
}
//
// Si se escribe el final de un comando "!"
// entonces se detecta el comando, si el comando no existe entonces hay un error
//
void detectar_comando(void)
{
	int int_command[MAX_COMMAND_SIZE];
	
	int fin_comando;
	boolean escribirVector = FALSE;
	boolean error = FALSE;

	//Obtenemos el inidice de fin de comando
	for(int i=0; i<WRITE_BUFFER_SIZE; i++) {
		if(comando[i] == '!'){
				fin_comando = i;
		}
	}
	
	//Comprobamos el tipo de jugada que es
	if(detectar_reset(fin_comando)) { //Si es un reset, reiniciamos partida
		reiniciar_partida();
		
	} else if (detectar_new(fin_comando)){ //Si es un new, iniciamos nueva partida
		nueva_partida();
		
	} else if (detectar_jugada(fin_comando)) { //Si es una nueva jugada, intriducimos la jugada
		introducir_jugada(int_command);
		
	} else { //SI el comando no existe, limpiamos el vector y lanzamos un error
		reiniciar_comando();
		lanzar_error(NOT_A_COMMAND);
	}
	
}

//
// Función extremadamente fea que cambia el valor de tipo carácter
// a entero sin signo
//
// Si no es un número, devuelve -1 (error)
//
int to_uint(char value)
{
	switch(value)
	{
		case '0':
			return 0;
		
		case '1':
			return 1;
		
		case '2':
			return 2;
		
		case '3':
			return 3;
		
		case '4':
			return 4;
		
		case '5':
			return 5;
		
		case '6':
			return 6;
		
		case '7':
			return 7;
		
		case '8':
			return 8;
		
		case '9':
			return 9;
		
		default:
			return -1;
	}
}

void reiniciar_partida(void)
{
	// Reiniciar Partida
}

void nueva_partida(void)
{
	// Nueva Partida
}

void introducir_jugada(int info[])
{
	uint8_t fila, columna, valor, checksum, checksum_real;
	
	fila 					= info[0];
	columna 			= info[1];
	valor 				= info[2];
	checksum 			= info[3];
	checksum_real = (fila + columna + valor) % 8;
	
	if (checksum == checksum_real)
	{
		// Meter el valor
	}
	else
	{
		lanzar_error(BAD_CHECKSUM);
	}
}
	
void lanzar_error(command_err err)
{
	switch(err)
	{
		case OVERSIZED_COMMAND:
			break;
		
		case NOT_A_COMMAND:
			break;
		
		case BAD_CHECKSUM:
			break;
	}
}