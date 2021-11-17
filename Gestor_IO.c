#include "gestor_IO.h"
#include "gpio_control.h"
#include "eventos.h"
#include "gestor_alarmas.h"
#include "sudoku_p2.h"
#include "temporizador.h"

uint32_t estadoAnterior = 0x0;

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
	// Obtenemos la fila y columna pedida
	uint8_t fila = GPIO_leer(16,4);
	uint8_t columna = GPIO_leer(20,4);
	
	// Obtenemos del sudoku el valor de esa celda
	uint8_t valor = leer_celda(fila, columna);
	// Obtenemos los candidatos
	uint16_t candidatos = leer_candidatos(fila, columna);
	
	// Marcamos el valor de la celda en la salida
	GPIO_escribir(0,3,valor);
	// Marcamos los candidatos de la celda en la salida
	GPIO_escribir(4,9,candidatos);
	
	// Comprobamos el estado anterior de las entradas
	uint32_t estadoActual = GPIO_leer(16,14);
	if (estadoActual != estadoAnterior){
		estadoAnterior = estadoActual;
		crear_alarma_unica(POW_DOWN,EV_POWER,15000);
	}
}

void escribirValor(void){
	uint64_t ini = temporizador_leer(); // inicio cuenta del tiempo de escritura
	
	uint8_t fila = GPIO_leer(16,4);
	uint8_t columna = GPIO_leer(20,4);
	uint8_t valor = GPIO_leer(24,4);
	
	if (es_pista(fila,columna)){
		GPIO_escribir(13,1,1);
	}else{
		introducirValorCelda(fila,columna,valor);
		candidatos_propagar(fila,columna);
		// check error
	}
	
	uint64_t fin = temporizador_leer() - ini; // fin cuenta del tiempo de escritura
}
void eliminarValor(void){
	uint64_t ini = temporizador_leer(); // inicio cuenta del tiempo de eliminación
	
	uint8_t fila = GPIO_leer(16,4);
	uint8_t columna = GPIO_leer(20,4);
	
	if (es_pista(fila,columna)){
		GPIO_escribir(13,1,1);
		crear_alarma_unica(LED_ERROR,EV_LED_ERR,1000);
	}else{
		eliminarValorCelda(fila,columna);
		candidatos_propagar(fila,columna);
		// check error
	}
	
	uint64_t fin = temporizador_leer() - ini; // fin cuenta del tiempo de eliminación
}

void checkFinPartida(uint8_t fila, uint8_t columna, uint8_t valor){
	if(fila == columna == valor == 0){
		// PowerDown o Idle
		// Reiniciar Sudoku
	}
}

void quitarLedErr(void){
	GPIO_escribir(13,1,0);
}