#include "gestor_IO.h"
#include "gpio_control.h"
#include "eventos.h"
#include "gestor_alarmas.h"
#include "sudoku_p2.h"
#include "temporizador.h"
#include "gestor_energia.h"

uint32_t estadoAnterior = 0x0;

//Inicializa el gestor IO
void initIO(void){
	// Iniciamos la GPIO
	GPIO_iniciar();
	
	// Creamos la alarma que refresca la GPIO
	crear_alarma_periodica(GPIO_REFRESH,EV_GPIO,200);
	
	// Marcamos Salidas
	GPIO_marcar_salida(0,14);
	GPIO_marcar_salida(30,1);
}

void refrescarSalidas(void){
	uint32_t estadoActual;
	uint32_t candidatos;
	uint32_t fila, columna, valor;
	
	GPIO_clear_salida(0, 14);
	
	// Obtenemos la fila y columna pedida
	fila = GPIO_leer(16,4)-1;
	columna = GPIO_leer(20,4)-1;
	
	// Obtenemos del sudoku el valor de esa celda
	if(fila < 8 && columna < 8)
	{
		valor = leer_celda(fila, columna);
		// Obtenemos los candidatos
		candidatos = leer_candidatos(fila, columna);
		
		// Marcamos el valor de la celda en la salida
		GPIO_escribir(0,3,valor);
		
		// Marcamos los candidatos de la celda en la salida
		GPIO_escribir(4,9,candidatos);
		
		if (es_pista(fila,columna))
			GPIO_escribir(13,1,1);
	}
	
	// Comprobamos el estado anterior de las entradas
	estadoActual = GPIO_leer(16,14);
	if (estadoActual != estadoAnterior){
		estadoAnterior = estadoActual;
		crear_alarma_unica(POW_DOWN,EV_POWER,15000);
	}
}

void escribirValor(void){
	uint64_t ini, fin;
	uint8_t fila,columna,valor;
	int error;
	
	ini = temporizador_leer(); // inicio cuenta del tiempo de escritura
	
	fila = GPIO_leer(16,4)-1;
	columna = GPIO_leer(20,4)-1;
	valor = GPIO_leer(24,4);
	
	checkFinPartida(fila+1,columna+1,valor);
	
	if (!es_pista(fila,columna)){
		introducirValorCelda(fila,columna,valor);
		candidatos_actualizar();
		error = comprobar_una_celda(fila,columna);
		if(error == 1)
		{
			GPIO_escribir(13,1,1);
			crear_alarma_unica(LED_ERROR,EV_LED_ERR,1000);
		}			
	}
	
	fin = temporizador_leer() - ini; // fin cuenta del tiempo de escritura
}

void eliminarValor(void){	
	uint64_t ini, fin;
	uint8_t fila,columna,valor;
	
	ini = temporizador_leer(); // inicio cuenta del tiempo de eliminación
	
	fila = GPIO_leer(16,4)-1;
	columna = GPIO_leer(20,4)-1;
	valor = GPIO_leer(24,4);
	 
	checkFinPartida(fila+1,columna+1,valor);
	
	if (!es_pista(fila,columna)){
		eliminarValorCelda(fila,columna);
		candidatos_actualizar();
	}
	
	fin = temporizador_leer() - ini; // fin cuenta del tiempo de eliminación
}

void checkFinPartida(uint8_t fila, uint8_t columna, uint8_t valor){
	if(fila == columna == valor == 0){
		actualizar_estado_energia(); // PowerDown o Idle
		sudokuReiniciar();
	}
}

void quitarLedErr(void){
	GPIO_escribir(13,1,0);
}

void overflow(void)
{
	GPIO_escribir(30,1,1);
	while(1)	
		actualizar_estado_energia(); // PowerDown o Idle
}
